#include <stdio.h>

#include "edge-impulse-sdk/classifier/ei_run_classifier.h"

// Callback function declaration
static int get_signal_data(size_t offset, size_t length, float *out_ptr);

// Raw features copied from test sample (Edge Impulse > Model testing)
static float input_buf[] = {
    /* Paste your raw features here! */ 
-6.3879, 1.8572, 28.4006, 12.7749, -3.3287, 21.1314, 10.5993, 0.8068, 13.3987, 11.8386, -3.6667, 13.1997, 11.0386, -2.0601, 11.7319, 10.4516, -0.7404, 12.6396, 6.9251, 0.2379, 12.2098, 4.0972, -0.9394, 10.5219, 3.1696, -2.2555, 8.3903, 1.8013, 1.0238, 10.2442, 0.5339, -1.1093, 6.4844, -0.9269, -2.2464, 10.2660, -0.9165, -0.5939, 15.2991, -1.3210, 0.5362, 15.2071, -2.5970, -2.2932, 14.4210, -1.9495, 0.9771, 20.1961, -5.3534, -2.1449, 16.4714, -5.6623, -1.3529, 19.4442, -3.0189, -0.7883, 30.4532, -6.5242, -1.2035, 20.3744, -4.9394, 0.8809, 33.3813, -5.1906, -3.0405, 21.9161, -5.4678, -0.6996, 30.6094, 1.2944, 2.8539, 27.2143, -4.4930, -2.6492, 26.3421, 3.3074, 0.0009, 21.7822, -0.5109, -3.2839, 21.6238, 5.7151, 0.9120, 27.6994, 7.0508, -4.6340, 18.4397, 7.9936, -0.3285, 23.3801, 11.2915, -2.7626, 21.8960, 13.4761, 2.3985, 25.9220, 14.8737, -2.7209, 29.8252, 13.3246, -4.5367, 17.7389, 13.5127, 4.2545, 24.5317, 12.7755, -6.9532, 20.5150, 15.6549, -2.6537, 21.2892, 12.6408, -4.5107, 16.5332, 12.1129, 5.2032, 16.3864, 13.6615, -5.0814, 12.3963, 14.9837, 2.7381, 18.7792, 9.5228, -3.2944, 5.7031, 13.3559, -0.9147, 8.8663, 9.7619, -1.1681, 9.1303, 9.6018, -0.0494, 9.9635, 6.0258, -1.7740, 9.8382, 4.5204, -0.3041, 11.1178, 3.7188, -0.6440, 9.7270, 1.7129, -1.2827, 10.3431, -0.0976, -1.2905, 13.0454, -0.4721, 1.1103, 14.3799, -0.1266, -0.0305, 17.4277, -5.1664, 0.1321, 17.8643, -5.9467, -2.0880, 16.1217, -6.3316, -1.7223, 18.8308, -6.3826, 4.5464, 24.0201, -8.5659, -2.5184, 20.2805, -8.7182, -2.1871, 18.1560, -2.1154, 3.9741, 31.8867, -7.5161, -2.7041, 18.5124, -2.7379, -0.1947, 30.6869, -2.0327, -5.4010, 20.7598, 3.7174, -2.0654, 33.9357, 0.9065, 0.5968, 20.1578, 2.4128, -1.9121, 26.5471, 6.4251, -3.3897, 27.1935, 4.9436, -1.0713, 18.0608, 10.6528, -0.3035, 24.5555, 12.1316, -5.0968, 18.8564, 12.9022, 1.6833, 18.7628, 14.4013, -1.4914, 20.9994, 14.5820, -3.5401, 20.2469, 14.0796, 3.6284, 22.1513, 13.1095, -3.8131, 16.6686, 17.6010, 2.7092, 20.3962, 15.3443, -0.2803, 24.4786, 11.9745, -3.4810, 16.3823, 14.4221, -0.7615, 13.2983, 11.3548, -4.0003, 11.3796, 14.0183, -0.3627, 18.3531, 9.6413, -1.9641, 8.4078, 11.0834, -0.2895, 10.1947, 6.8359, -0.8676, 8.4689, 3.0412, -3.9865, 5.4104, 3.0412, -3.9865, 5.4104, 2.9883, 0.2351, 8.6022, 2.3559, -2.3115, 5.2895, 1.5077, -3.5762, 7.0894, 0.3959, -1.2643, 12.5790, -0.9065, 0.9465, 17.9876, -1.1284, 1.1135, 12.8581, -3.2195, 1.2582, 16.5385, -5.8890, 2.4421, 17.6945, -5.8890, 2.4421, 17.6945, -7.4095, 3.4687, 26.5509, -5.6253, -0.0732, 30.1837, -7.0702, -1.3482, 20.6896, -4.0160, 3.4833, 24.9904, 0.5086, -1.7430, 21.4727, -4.0718, -1.7936, 24.6957, 0.7756, -0.7769, 22.9191, -3.1481, -1.4615, 22.6934, 3.6372, 0.8443, 23.0858, 3.2649, -1.7265, 21.6605, 6.4994, -2.0438, 20.7902, 9.6361, -1.7077, 18.1738, 13.5049, 1.6427, 26.4734, 10.6636, -4.4670, 18.3034, 15.4870, -1.3666, 22.4371, 14.6076, -5.1751, 23.9450, 12.9692, -2.2274, 13.3875, 16.5260, -1.7415, 23.4118, 15.0079, -3.7136, 21.6328, 13.8123, -0.5662, 14.5509, 14.8720, -0.8580, 20.0006, 13.9129, 2.4421, 14.0365, 15.8092, -4.4128, 20.8524, 12.2499, -6.9863, 12.9680, 16.3148, 3.9572, 16.6915, 10.9382, -1.7915, 15.0143, 8.7806, -1.8842, 11.5658, 7.4693, 0.8074, 9.9095, 5.2035, -0.6853, 14.7038, 5.8140, -1.2851, 10.0341, 3.0746, 1.1182, 12.4298};
int main(int argc, char **argv) {
    
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

    // Print the prediction results (object detection)
#if EI_CLASSIFIER_OBJECT_DETECTION == 1
    printf("Object detection bounding boxes:\r\n");
    for (uint32_t i = 0; i < EI_CLASSIFIER_OBJECT_DETECTION_COUNT; i++) {
        ei_impulse_result_bounding_box_t bb = result.bounding_boxes[i];
        if (bb.value == 0) {
            continue;
        }
        printf("  %s (%f) [ x: %u, y: %u, width: %u, height: %u ]\r\n", 
                bb.label, 
                bb.value, 
                bb.x, 
                bb.y, 
                bb.width, 
                bb.height);
    }

    // Print the prediction results (classification)
#else
    printf("Predictions:\r\n");
    for (uint16_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
        printf("  %s: ", ei_classifier_inferencing_categories[i]);
        printf("%.5f\r\n", result.classification[i].value);
    }
#endif

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