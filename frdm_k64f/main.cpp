#define ML_ON_K64F

#ifdef ML_ON_K64F
/**
 * @file    Final_ML_Project.cpp
 * @brief   Application entry point.
 */

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"

// Include files from Edge Impulse Library
#include <ei/classifier/ei_run_classifier.h>
#include <ei/porting/ei_classifier_porting.h>

#define RED_LED			(22)	// PTB22 red onboard LED
#define MASK(X)			(1UL << X)
#define LED_INTERVAL			(500) 	// 500ms restart

// CONFIGURE SYSTICK TIMER TO GET INFERENCE TIME
volatile unsigned long counter = 0;
void configure_SysTick();
unsigned long  millis(void);
void SysTick_Handler(void);

// Initialize LED to turn on RED during inference
void initialize_LED();

// Run inferencing
int run_ML_inferencing();

// Callback function declaration
static int get_signal_data(size_t offset, size_t length, float *out_ptr);

// Raw features copied from test sample (Edge Impulse > Model testing)
// To be always changed by Microcontroller Accelerometer
static float input_buf[] = {
		-0.6550, 2.7881, 19.1040, -3.7558, -0.4312, 18.2421, -0.8497, 4.0711, 19.9340, -0.2873, -1.2997, 11.1563, -0.9152, -1.2212, 9.0763, 1.1317, -1.4677, 16.4045, 3.1160, 0.4201, 8.8206, 3.6900, -2.1786, 11.8259, 3.8109, -0.9483, 5.5851, 5.8066, -2.6483, 7.8056, 5.2805, -1.5768, 5.3844, 12.4410, 5.6850, 11.4524, 7.5276, -2.4758, 13.0619, 10.8698, -1.9875, 5.3236, 11.4430, -3.5090, 12.1945, 13.7106, 3.3934, 15.7972, 13.0647, -5.3988, 12.1975, 18.6581, 2.4133, 23.7766, 18.3200, -0.1798, 14.9345, 18.3200, -0.1798, 14.9345, 19.4979, -5.2112, 29.7281, 18.0617, -4.7211, 24.8562, 23.6812, 1.9765, 29.1559, 19.2449, -5.0083, 34.8301, 19.2149, -3.0397, 24.3869, 17.7345, 1.4996, 28.7602, 14.8315, -6.6235, 29.0384, 14.2726, -4.4313, 26.2789, 13.0634, -0.9317, 27.8697, 10.0063, -1.8580, 22.6779, 6.8126, -2.6941, 23.4637, 3.3269, -2.1406, 19.9087, -0.4463, -0.1727, 16.8322, -2.8338, 0.6431, 20.8211, -4.2488, 1.3832, 20.9869, -4.8960, -0.8379, 20.6423, -4.4344, 0.6208, 19.7302, -2.9838, -0.1998, 15.7562, -2.8909, 2.8686, 18.5403, -1.0129, 3.1962, 19.0895, -2.4137, -0.3773, 10.6194, -0.4358, -1.1512, 14.8648, 3.8320, 1.1748, 10.3966, 4.6439, -0.7376, 9.9052, 3.9260, -1.7584, 7.2760, 5.0442, -2.3528, 5.9709, 6.4730, -1.8548, 4.2484, 6.4064, -3.3805, 4.2781, 8.8857, -2.4472, 5.4480, 9.6496, 0.4073, 15.2057, 7.5825, -3.3482, 9.5050, 13.0940, -3.2593, 6.5161, 13.7822, -1.9655, 13.9920, 10.4889, -4.2376, 14.1491, 18.6071, -1.3435, 14.7773, 14.1181, -8.2668, 19.0313, 17.2798, 0.5138, 13.6877, 16.5953, -3.3702, 24.4076, 19.1033, -3.2610, 26.8237, 15.7648, -4.2457, 26.7444, 20.3870, -2.0718, 27.7076, 11.3406, -9.8057, 32.1420, 16.0771, -1.2697, 25.4822, 13.0613, -4.7318, 32.9132, 15.1260, -3.0223, 35.1502, 13.3871, -3.4700, 28.9585, 9.9126, -3.1791, 33.1729, 11.4660, -2.7733, 24.9567, 9.0498, -4.1000, 22.2833, 7.1452, 1.3736, 22.6806, 4.7007, 0.0661, 19.8872, -2.8051, -1.2572, 15.3901, -5.1557, -1.9471, 22.1210, -1.8209, 0.0553, 23.5500, -0.8347, -0.2959, 15.7021, -1.5272, -1.2042, 14.8221, 0.2886, 0.7475, 13.1884, -0.9015, -0.1365, 12.3667, 3.1100, -0.2477, 12.9349, 2.9085, -0.5848, 8.4805, 4.0285, -1.4518, 8.9255, 6.1921, -2.0481, 6.6918, 8.7168, -0.7930, 8.4324, 9.7038, 0.1544, 6.0593, 8.6198, -1.1684, 9.3284, 9.8117, -2.5276, 8.3583, 9.9190, -3.5740, 6.3264, 12.3301, -2.5487, 10.6262, 12.2829, -3.5243, 10.4937, 15.6435, -2.2039, 21.1931, 17.6283, 0.6355, 18.5616, 17.1501, -4.4429, 14.1377, 20.1813, -2.5883, 21.2925, 19.4990, -4.1869, 21.3091, 20.3848, -3.3742, 23.5239, 21.9239, -7.3735, 19.9501, 16.3032, -5.8542, 31.2156, 19.3377, -1.7258, 20.5648, 14.7338, -1.9668, 25.4195, 17.5373, 0.8270, 28.6209, 11.1150, -3.5960, 19.7105, 9.9085, -0.4393, 24.2661, 10.4709, -1.8010, 23.4789, 6.3942, -1.3871, 18.6140, 4.1728, -1.5552, 23.7529, 0.9878, -1.9813, 19.7688, -2.0943, 1.1798, 20.6440, -2.7991, 0.4607, 20.7669, -5.5554, -0.7900, 18.2292, -5.2579, 1.1331, 20.0902, -5.2579, 1.1331, 20.0902, -4.2805, 0.5744, 21.4183, -2.8279, 2.9604, 18.5059, -3.6889, -1.1368, 20.8217, 0.2223, 0.9207, 19.4274, 2.2308, 1.2264, 14.3187, 1.7170, -1.3330, 15.2228, 2.9146, -4.4218, 7.1880, 10.5875, -1.5468, 10.8224, 7.5257, 0.0471, 8.0805, 8.6460, -1.2221, 6.6973, 9.9128, -1.1612, 5.7833, 12.6328, 0.3878, 6.1118, 9.9076, -0.7376, 13.1476, 11.5123, -4.1619, 6.1312
};

int main(){

	initialize_LED();

	configure_SysTick();

	unsigned long start_time = 0u;

	unsigned long end_time;

	while (1){

		run_ML_inferencing();

		unsigned long end_time = millis();

		printf("Time for inferencing is %d ms", end_time);

		end_time = 0u;

	}

}

int run_ML_inferencing() {
	signal_t signal;            // Wrapper for raw input buffer
	ei_impulse_result_t result; // Used to store inference output
	EI_IMPULSE_ERROR res;       // Return code from inference

	// Calculate the length of the buffer
	size_t buf_len = sizeof(input_buf) / sizeof(input_buf[0]);

	// Make sure that the length of the buffer matches expected input length
	if (buf_len != EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
		printf("ERROR: The size of the input buffer is not correct.\r\n");
		printf("Expected %d items, but got %d\r\n",
				EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE,
				(int)buf_len);
		return 1;
	}

	// Assign callback function to fill buffer used for preprocessing/inference
	signal.total_length = EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE;
	signal.get_data = &get_signal_data;

	// Perform DSP pre-processing and inference
	res = run_classifier(&signal, &result, false);

	// Print return code and how long it took to perform inference
	printf("run_classifier returned: %d\r\n", res);
	printf("Timing: DSP %d ms, inference %d ms, anomaly %d ms\r\n",
			result.timing.dsp,
			result.timing.classification,
			result.timing.anomaly);

	printf("Predictions:\r\n");
	for (uint16_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
		printf("  %s: ", ei_classifier_inferencing_categories[i]);
		printf("%.5f\r\n", result.classification[i].value);
	}

	// Print anomaly result (if it exists)
	#if EI_CLASSIFIER_HAS_ANOMALY == 1
		printf("Anomaly prediction: %.3f\r\n", result.anomaly);
	#endif

	return 0;
}

// Callback: fill a section of the out_ptr buffer when requested
static int get_signal_data(size_t offset, size_t length, float *out_ptr) {
    for (size_t i = 0; i < length; i++) {
        out_ptr[i] = (input_buf + offset)[i];
    }

    return EIDSP_OK;
}

void initialize_LED() {

	// Clock Gate
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;

	// Set the MUX
	PORTB->PCR[RED_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED] |= PORT_PCR_MUX(1);

	// Configure LED Port
	GPIOB->PDDR |= MASK(RED_LED);

	// Turn off LED
	GPIOB->PSOR |= MASK(RED_LED);
}

void configure_SysTick() {
	// Configure SysTick Timer
	SysTick->LOAD = (20971520u/1000u)-1 ;  //configure for every milli sec restart.
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk |SysTick_CTRL_TICKINT_Msk;
}

void SysTick_Handler(void){
	counter++;
}

unsigned long  millis(void){
	return (unsigned long) counter;
}

#endif
