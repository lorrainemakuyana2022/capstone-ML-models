/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_fir_decimate_f32.c
 * Description:  FIR decimation for floating-point sequences
 *
 * $Date:        18. March 2019
 * $Revision:    V1.6.0
 *
 * Target Processor: Cortex-M cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2019 ARM Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ei/CMSIS/DSP/Include/dsp/filtering_functions.h>

/**
  @ingroup groupFilters
 */

/**
  @defgroup FIR_decimate Finite Impulse Response (FIR) Decimator

  These functions combine an FIR filter together with a decimator.
  They are used in multirate systems for reducing the sample rate of a signal without introducing aliasing distortion.
  Conceptually, the functions are equivalent to the block diagram below:
  \image html FIRDecimator.gif "Components included in the FIR Decimator functions"
  When decimating by a factor of <code>M</code>, the signal should be prefiltered by a lowpass filter with a normalized
  cutoff frequency of <code>1/M</code> in order to prevent aliasing distortion.
  The user of the function is responsible for providing the filter coefficients.

  The FIR decimator functions provided in the CMSIS DSP Library combine the FIR filter and the decimator in an efficient manner.
  Instead of calculating all of the FIR filter outputs and discarding <code>M-1</code> out of every <code>M</code>, only the
  samples output by the decimator are computed.
  The functions operate on blocks of input and output data.
  <code>pSrc</code> points to an array of <code>blockSize</code> input values and
  <code>pDst</code> points to an array of <code>blockSize/M</code> output values.
  In order to have an integer number of output samples <code>blockSize</code>
  must always be a multiple of the decimation factor <code>M</code>.

  The library provides separate functions for Q15, Q31 and floating-point data types.

  @par           Algorithm:
                   The FIR portion of the algorithm uses the standard form filter:
  <pre>
      y[n] = b[0] * x[n] + b[1] * x[n-1] + b[2] * x[n-2] + ...+ b[numTaps-1] * x[n-numTaps+1]
  </pre>
                   where, <code>b[n]</code> are the filter coefficients.
  @par
                   The <code>pCoeffs</code> points to a coefficient array of size <code>numTaps</code>.
                   Coefficients are stored in time reversed order.
  @par
  <pre>
      {b[numTaps-1], b[numTaps-2], b[N-2], ..., b[1], b[0]}
  </pre>
  @par
                   <code>pState</code> points to a state array of size <code>numTaps + blockSize - 1</code>.
                   Samples in the state buffer are stored in the order:
  @par
  <pre>
      {x[n-numTaps+1], x[n-numTaps], x[n-numTaps-1], x[n-numTaps-2]....x[0], x[1], ..., x[blockSize-1]}
  </pre>
                   The state variables are updated after each block of data is processed, the coefficients are untouched.

  @par           Instance Structure
                   The coefficients and state variables for a filter are stored together in an instance data structure.
                   A separate instance structure must be defined for each filter.
                   Coefficient arrays may be shared among several instances while state variable array should be allocated separately.
                   There are separate instance structure declarations for each of the 3 supported data types.

 @par            Initialization Functions
                   There is also an associated initialization function for each data type.
                   The initialization function performs the following operations:
                   - Sets the values of the internal structure fields.
                   - Zeros out the values in the state buffer.
                   - Checks to make sure that the size of the input is a multiple of the decimation factor.
                   To do this manually without calling the init function, assign the follow subfields of the instance structure:
                   numTaps, pCoeffs, M (decimation factor), pState. Also set all of the values in pState to zero.
  @par
                   Use of the initialization function is optional.
                   However, if the initialization function is used, then the instance structure cannot be placed into a const data section.
                   To place an instance structure into a const data section, the instance structure must be manually initialized.
                   The code below statically initializes each of the 3 different data type filter instance structures
  <pre>
      arm_fir_decimate_instance_f32 S = {M, numTaps, pCoeffs, pState};
      arm_fir_decimate_instance_q31 S = {M, numTaps, pCoeffs, pState};
      arm_fir_decimate_instance_q15 S = {M, numTaps, pCoeffs, pState};
  </pre>
                   where <code>M</code> is the decimation factor; <code>numTaps</code> is the number of filter coefficients in the filter;
                   <code>pCoeffs</code> is the address of the coefficient buffer;
                   <code>pState</code> is the address of the state buffer.
                   Be sure to set the values in the state buffer to zeros when doing static initialization.

  @par           Fixed-Point Behavior
                   Care must be taken when using the fixed-point versions of the FIR decimate filter functions.
                   In particular, the overflow and saturation behavior of the accumulator used in each function must be considered.
                   Refer to the function specific documentation below for usage guidelines.
 */

/**
  @addtogroup FIR_decimate
  @{
 */

/**
  @brief         Processing function for floating-point FIR decimator.
  @param[in]     S         points to an instance of the floating-point FIR decimator structure
  @param[in]     pSrc      points to the block of input data
  @param[out]    pDst      points to the block of output data
  @param[in]     blockSize number of samples to process
  @return        none
 */
#if defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE)

#include <ei/CMSIS/DSP/Include/arm_helium_utils.h>

void arm_fir_decimate_f32(
  const arm_fir_decimate_instance_f32 * S,
  const float32_t * pSrc,
  float32_t * pDst,
  uint32_t blockSize)
{
    float32_t *pState = S->pState;  /* State pointer */
    const float32_t *pCoeffs = S->pCoeffs;    /* Coefficient pointer */
    float32_t *pStateCurnt;     /* Points to the current sample of the state */
    const float32_t *px, *pb;         /* Temporary pointers for state and coefficient buffers */
    uint32_t  numTaps = S->numTaps; /* Number of filter coefficients in the filter */
    uint32_t  i, tapCnt, blkCnt, outBlockSize = blockSize / S->M;   /* Loop counters */
    uint32_t  blkCntN4;
    const float32_t *px0, *px1, *px2, *px3;
    f32x4_t accv, acc0v, acc1v, acc2v, acc3v;
    f32x4_t x0v, x1v, x2v, x3v;
    f32x4_t c0v;

    /*
     * S->pState buffer contains previous frame (numTaps - 1) samples
     * pStateCurnt points to the location where the new input data should be written
     */
    pStateCurnt = S->pState + (numTaps - 1U);
    /*
     * Total number of output samples to be computed
     */
    blkCnt = outBlockSize / 4;
    blkCntN4 = outBlockSize - (4 * blkCnt);

    while (blkCnt > 0U)
    {
        /*
         * Copy 4 * decimation factor number of new input samples into the state buffer
         */
        i = (4 * S->M) >> 2;
        do
        {
            vst1q(pStateCurnt, vld1q((const float32_t *)pSrc));
            pSrc += 4;
            pStateCurnt += 4;
            i--;
        }
        while (i > 0U);

        /*
         * Set accumulators to zero
         */
        acc0v = vdupq_n_f32(0.0f);
        acc1v = vdupq_n_f32(0.0f);
        acc2v = vdupq_n_f32(0.0f);
        acc3v = vdupq_n_f32(0.0f);

        /*
         * Initialize state pointer for all the samples
         */
        px0 = pState;
        px1 = pState + S->M;
        px2 = pState + 2 * S->M;
        px3 = pState + 3 * S->M;
        /*
         * Initialize coeff pointer
         */
        pb = pCoeffs;
        /*
         * Loop unrolling.  Process 4 taps at a time.
         */
        tapCnt = numTaps >> 2;
        /*
         * Loop over the number of taps.  Unroll by a factor of 4.
         * Repeat until we've computed numTaps-4 coefficients.
         */
        while (tapCnt > 0U)
        {
            /*
             * Read the b[numTaps-1] coefficient
             */
            c0v = vld1q((const float32_t *)pb);
            pb += 4;
            /*
             * Read x[n-numTaps-1] sample for acc0
             */
            x0v = vld1q(px0);
            x1v = vld1q(px1);
            x2v = vld1q(px2);
            x3v = vld1q(px3);
            px0 += 4;
            px1 += 4;
            px2 += 4;
            px3 += 4;

            acc0v = vfmaq(acc0v, x0v, c0v);
            acc1v = vfmaq(acc1v, x1v, c0v);
            acc2v = vfmaq(acc2v, x2v, c0v);
            acc3v = vfmaq(acc3v, x3v, c0v);
            /*
             * Decrement the loop counter
             */
            tapCnt--;
        }

        /*
         * If the filter length is not a multiple of 4, compute the remaining filter taps
         * should be tail predicated
         */
        tapCnt = numTaps % 0x4U;
        if (tapCnt > 0U)
        {
            mve_pred16_t p0 = vctp32q(tapCnt);
            /*
             * Read the b[numTaps-1] coefficient
             */
            c0v = vldrwq_z_f32(pb, p0);
            pb += 4;
            /*
             * Read x[n-numTaps-1] sample for acc0
             */
            x0v = vld1q(px0);
            x1v = vld1q(px1);
            x2v = vld1q(px2);
            x3v = vld1q(px3);
            px0 += 4;
            px1 += 4;
            px2 += 4;
            px3 += 4;

            acc0v = vfmaq_f32(acc0v, x0v, c0v);
            acc1v = vfmaq_f32(acc1v, x1v, c0v);
            acc2v = vfmaq_f32(acc2v, x2v, c0v);
            acc3v = vfmaq_f32(acc3v, x3v, c0v);
        }

        /* reduction */
        accv[0] = vecAddAcrossF32Mve(acc0v);
        accv[1] = vecAddAcrossF32Mve(acc1v);
        accv[2] = vecAddAcrossF32Mve(acc2v);
        accv[3] = vecAddAcrossF32Mve(acc3v);

        /*
         * Advance the state pointer by the decimation factor
         * to process the next group of decimation factor number samples
         */
        pState = pState + 4 * S->M;
        /*
         * The result is in the accumulator, store in the destination buffer.
         */
        vst1q(pDst, accv);
        pDst += 4;

        /*
         * Decrement the loop counter
         */
        blkCnt--;
    }

    while (blkCntN4 > 0U)
    {
        /*
         * Copy decimation factor number of new input samples into the state buffer
         */
        i = S->M;
        do
        {
            *pStateCurnt++ = *pSrc++;
        }
        while (--i);
        /*
         * Set accumulator to zero
         */
        acc0v = vdupq_n_f32(0.0f);
        /*
         * Initialize state pointer
         */
        px = pState;
        /*
         * Initialize coeff pointer
         */
        pb = pCoeffs;
        /*
         * Loop unrolling.  Process 4 taps at a time.
         */
        tapCnt = numTaps >> 2;
        /*
         * Loop over the number of taps.  Unroll by a factor of 4.
         * Repeat until we've computed numTaps-4 coefficients.
         */
        while (tapCnt > 0U)
        {
            c0v = vldrwq_f32(pb);
            x0v = vldrwq_f32(px);
            pb += 4;
            px += 4;
            acc0v = vfmaq_f32(acc0v, x0v, c0v);
            /*
             * Decrement the loop counter
             */
            tapCnt--;
        }
        tapCnt = numTaps % 0x4U;
        if (tapCnt > 0U)
        {
            mve_pred16_t p0 = vctp32q(tapCnt);
            c0v = vldrwq_z_f32(pb, p0);
            x0v = vldrwq_f32(px);
            acc0v = vfmaq_f32(acc0v, x0v, c0v);
        }
        accv[0] = vecAddAcrossF32Mve(acc0v);

        /*
         * Advance the state pointer by the decimation factor
         * * to process the next group of decimation factor number samples
         */
        pState = pState + S->M;
        /*
         * The result is in the accumulator, store in the destination buffer.
         */
        *pDst++ = accv[0];
        /*
         * Decrement the loop counter
         */
        blkCntN4--;
    }

    /*
     * Processing is complete.
     * Now copy the last numTaps - 1 samples to the start of the state buffer.
     * This prepares the state buffer for the next function call.
     */

    pStateCurnt = S->pState;
    blkCnt =(numTaps - 1) >> 2;
    while (blkCnt > 0U)
    {
        vst1q(pStateCurnt, vldrwq_f32(pState));
        pState += 4;
        pStateCurnt += 4;
        blkCnt--;
    }
    blkCnt = (numTaps - 1) & 3;
    if (blkCnt > 0U)
    {
        mve_pred16_t p0 = vctp32q(blkCnt);
        vstrwq_p_f32(pStateCurnt, vldrwq_f32(pState), p0);
    }
}
#else
#if defined(ARM_MATH_NEON)
void arm_fir_decimate_f32(
  const arm_fir_decimate_instance_f32 * S,
  const float32_t * pSrc,
  float32_t * pDst,
  uint32_t blockSize)
{
  float32_t *pState = S->pState;                 /* State pointer */
  const float32_t *pCoeffs = S->pCoeffs;         /* Coefficient pointer */
  float32_t *pStateCurnt;                        /* Points to the current sample of the state */
  float32_t *px;                                 /* Temporary pointer for state buffer */
  const float32_t *pb;                           /* Temporary pointer for coefficient buffer */
  float32_t sum0;                                /* Accumulator */
  float32_t x0, c0;                              /* Temporary variables to hold state and coefficient values */
  uint32_t numTaps = S->numTaps;                 /* Number of filter coefficients in the filter */
  uint32_t i, tapCnt, blkCnt, outBlockSize = blockSize / S->M;  /* Loop counters */

  uint32_t blkCntN4;
  float32_t *px0, *px1, *px2, *px3;
  float32_t x1, x2, x3;

  float32x4_t accv,acc0v,acc1v,acc2v,acc3v;
  float32x4_t x0v, x1v, x2v, x3v;
  float32x4_t c0v;
  float32x2_t temp;
  float32x4_t sum0v;
 
  /* S->pState buffer contains previous frame (numTaps - 1) samples */
  /* pStateCurnt points to the location where the new input data should be written */
  pStateCurnt = S->pState + (numTaps - 1U);

  /* Total number of output samples to be computed */
  blkCnt = outBlockSize / 4;
  blkCntN4 = outBlockSize - (4 * blkCnt);

  while (blkCnt > 0U)
  {
    /* Copy 4 * decimation factor number of new input samples into the state buffer */
    i = 4 * S->M;

    do
    {
      *pStateCurnt++ = *pSrc++;

    } while (--i);

    /* Set accumulators to zero */
    acc0v = vdupq_n_f32(0.0);
    acc1v = vdupq_n_f32(0.0);
    acc2v = vdupq_n_f32(0.0);
    acc3v = vdupq_n_f32(0.0);

    /* Initialize state pointer for all the samples */
    px0 = pState;
    px1 = pState + S->M;
    px2 = pState + 2 * S->M;
    px3 = pState + 3 * S->M;

    /* Initialize coeff pointer */
    pb = pCoeffs;

    /* Process 4 taps at a time. */
    tapCnt = numTaps >> 2;

    /* Loop over the number of taps. 
     ** Repeat until we've computed numTaps-4 coefficients. */

    while (tapCnt > 0U)
    {
      /* Read the b[numTaps-1] coefficient */
      c0v = vld1q_f32(pb);
      pb += 4;

      /* Read x[n-numTaps-1] sample for acc0 */
      x0v = vld1q_f32(px0);
      x1v = vld1q_f32(px1);
      x2v = vld1q_f32(px2);
      x3v = vld1q_f32(px3);

      px0 += 4;
      px1 += 4;
      px2 += 4;
      px3 += 4;
     
      acc0v = vmlaq_f32(acc0v, x0v, c0v);
      acc1v = vmlaq_f32(acc1v, x1v, c0v);
      acc2v = vmlaq_f32(acc2v, x2v, c0v);
      acc3v = vmlaq_f32(acc3v, x3v, c0v);

      /* Decrement the loop counter */
      tapCnt--;
    }

    temp = vpadd_f32(vget_low_f32(acc0v),vget_high_f32(acc0v));
    accv = vsetq_lane_f32(vget_lane_f32(temp, 0) + vget_lane_f32(temp, 1),accv,0);

    temp = vpadd_f32(vget_low_f32(acc1v),vget_high_f32(acc1v));
    accv = vsetq_lane_f32(vget_lane_f32(temp, 0) + vget_lane_f32(temp, 1),accv,1);

    temp = vpadd_f32(vget_low_f32(acc2v),vget_high_f32(acc2v));
    accv = vsetq_lane_f32(vget_lane_f32(temp, 0) + vget_lane_f32(temp, 1),accv,2);

    temp = vpadd_f32(vget_low_f32(acc3v),vget_high_f32(acc3v));
    accv = vsetq_lane_f32(vget_lane_f32(temp, 0) + vget_lane_f32(temp, 1),accv,3);

    /* If the filter length is not a multiple of 4, compute the remaining filter taps */
    tapCnt = numTaps % 0x4U;

    while (tapCnt > 0U)
    {
      /* Read coefficients */
      c0 = *(pb++);

      /* Fetch  state variables for acc0, acc1, acc2, acc3 */
      x0 = *(px0++);
      x1 = *(px1++);
      x2 = *(px2++);
      x3 = *(px3++);

      /* Perform the multiply-accumulate */
      accv = vsetq_lane_f32(vgetq_lane_f32(accv, 0) + x0 * c0,accv,0);
      accv = vsetq_lane_f32(vgetq_lane_f32(accv, 1) + x1 * c0,accv,1);
      accv = vsetq_lane_f32(vgetq_lane_f32(accv, 2) + x2 * c0,accv,2);
      accv = vsetq_lane_f32(vgetq_lane_f32(accv, 3) + x3 * c0,accv,3);

      /* Decrement the loop counter */
      tapCnt--;
    }

    /* Advance the state pointer by the decimation factor
     * to process the next group of decimation factor number samples */
    pState = pState + 4 * S->M;

    /* The result is in the accumulator, store in the destination buffer. */
    vst1q_f32(pDst,accv);
    pDst += 4;

    /* Decrement the loop counter */
    blkCnt--;
  }

  while (blkCntN4 > 0U)
  {
    /* Copy decimation factor number of new input samples into the state buffer */
    i = S->M;

    do
    {
      *pStateCurnt++ = *pSrc++;

    } while (--i);

    /* Set accumulator to zero */
    sum0v =  vdupq_n_f32(0.0);

    /* Initialize state pointer */
    px = pState;

    /* Initialize coeff pointer */
    pb = pCoeffs;

    /* Process 4 taps at a time. */
    tapCnt = numTaps >> 2;

    /* Loop over the number of taps.
     ** Repeat until we've computed numTaps-4 coefficients. */
    while (tapCnt > 0U)
    {
      c0v = vld1q_f32(pb);
      pb += 4;

      x0v = vld1q_f32(px);
      px += 4;

      sum0v = vmlaq_f32(sum0v, x0v, c0v);
      
      /* Decrement the loop counter */
      tapCnt--;
    }

    temp = vpadd_f32(vget_low_f32(sum0v),vget_high_f32(sum0v));
    sum0 = vget_lane_f32(temp, 0) + vget_lane_f32(temp, 1);

    /* If the filter length is not a multiple of 4, compute the remaining filter taps */
    tapCnt = numTaps % 0x4U;

    while (tapCnt > 0U)
    {
      /* Read coefficients */
      c0 = *(pb++);

      /* Fetch 1 state variable */
      x0 = *(px++);

      /* Perform the multiply-accumulate */
      sum0 += x0 * c0;

      /* Decrement the loop counter */
      tapCnt--;
    }

    /* Advance the state pointer by the decimation factor
     * to process the next group of decimation factor number samples */
    pState = pState + S->M;

    /* The result is in the accumulator, store in the destination buffer. */
    *pDst++ = sum0;

    /* Decrement the loop counter */
    blkCntN4--;
  }

  /* Processing is complete.
   ** Now copy the last numTaps - 1 samples to the satrt of the state buffer.
   ** This prepares the state buffer for the next function call. */

  /* Points to the start of the state buffer */
  pStateCurnt = S->pState;

  i = (numTaps - 1U) >> 2;

  /* Copy data */
  while (i > 0U)
  {
    sum0v = vld1q_f32(pState);
    vst1q_f32(pStateCurnt,sum0v);
    pState += 4;
    pStateCurnt += 4;

    /* Decrement the loop counter */
    i--;
  }

  i = (numTaps - 1U) % 0x04U;

  /* Copy data */
  while (i > 0U)
  {
    *pStateCurnt++ = *pState++;

    /* Decrement the loop counter */
    i--;
  }
}
#else
void arm_fir_decimate_f32(
  const arm_fir_decimate_instance_f32 * S,
  const float32_t * pSrc,
        float32_t * pDst,
        uint32_t blockSize)
{
        float32_t *pState = S->pState;                 /* State pointer */
  const float32_t *pCoeffs = S->pCoeffs;               /* Coefficient pointer */
        float32_t *pStateCur;                          /* Points to the current sample of the state */
        float32_t *px0;                                /* Temporary pointer for state buffer */
  const float32_t *pb;                                 /* Temporary pointer for coefficient buffer */
        float32_t x0, c0;                              /* Temporary variables to hold state and coefficient values */
        float32_t acc0;                                /* Accumulator */
        uint32_t numTaps = S->numTaps;                 /* Number of filter coefficients in the filter */
        uint32_t i, tapCnt, blkCnt, outBlockSize = blockSize / S->M;  /* Loop counters */

#if defined (ARM_MATH_LOOPUNROLL)
        float32_t *px1, *px2, *px3;
        float32_t x1, x2, x3;
        float32_t acc1, acc2, acc3;
#endif

  /* S->pState buffer contains previous frame (numTaps - 1) samples */
  /* pStateCur points to the location where the new input data should be written */
  pStateCur = S->pState + (numTaps - 1U);

#if defined (ARM_MATH_LOOPUNROLL)

    /* Loop unrolling: Compute 4 samples at a time */
  blkCnt = outBlockSize >> 2U;

  /* Samples loop unrolled by 4 */
  while (blkCnt > 0U)
  {
    /* Copy 4 * decimation factor number of new input samples into the state buffer */
    i = S->M * 4;

    do
    {
      *pStateCur++ = *pSrc++;

    } while (--i);

    /* Set accumulators to zero */
    acc0 = 0.0f;
    acc1 = 0.0f;
    acc2 = 0.0f;
    acc3 = 0.0f;

    /* Initialize state pointer for all the samples */
    px0 = pState;
    px1 = pState + S->M;
    px2 = pState + 2 * S->M;
    px3 = pState + 3 * S->M;

    /* Initialize coeff pointer */
    pb = pCoeffs;

    /* Loop unrolling: Compute 4 taps at a time */
    tapCnt = numTaps >> 2U;

    while (tapCnt > 0U)
    {
      /* Read the b[numTaps-1] coefficient */
      c0 = *(pb++);

      /* Read x[n-numTaps-1] sample for acc0 */
      x0 = *(px0++);
      /* Read x[n-numTaps-1] sample for acc1 */
      x1 = *(px1++);
      /* Read x[n-numTaps-1] sample for acc2 */
      x2 = *(px2++);
      /* Read x[n-numTaps-1] sample for acc3 */
      x3 = *(px3++);

      /* Perform the multiply-accumulate */
      acc0 += x0 * c0;
      acc1 += x1 * c0;
      acc2 += x2 * c0;
      acc3 += x3 * c0;

      /* Read the b[numTaps-2] coefficient */
      c0 = *(pb++);

      /* Read x[n-numTaps-2] sample for acc0, acc1, acc2, acc3 */
      x0 = *(px0++);
      x1 = *(px1++);
      x2 = *(px2++);
      x3 = *(px3++);

      /* Perform the multiply-accumulate */
      acc0 += x0 * c0;
      acc1 += x1 * c0;
      acc2 += x2 * c0;
      acc3 += x3 * c0;

      /* Read the b[numTaps-3] coefficient */
      c0 = *(pb++);

      /* Read x[n-numTaps-3] sample acc0, acc1, acc2, acc3 */
      x0 = *(px0++);
      x1 = *(px1++);
      x2 = *(px2++);
      x3 = *(px3++);

      /* Perform the multiply-accumulate */
      acc0 += x0 * c0;
      acc1 += x1 * c0;
      acc2 += x2 * c0;
      acc3 += x3 * c0;

      /* Read the b[numTaps-4] coefficient */
      c0 = *(pb++);

      /* Read x[n-numTaps-4] sample acc0, acc1, acc2, acc3 */
      x0 = *(px0++);
      x1 = *(px1++);
      x2 = *(px2++);
      x3 = *(px3++);

      /* Perform the multiply-accumulate */
      acc0 += x0 * c0;
      acc1 += x1 * c0;
      acc2 += x2 * c0;
      acc3 += x3 * c0;

      /* Decrement loop counter */
      tapCnt--;
    }

    /* Loop unrolling: Compute remaining taps */
    tapCnt = numTaps % 0x4U;

    while (tapCnt > 0U)
    {
      /* Read coefficients */
      c0 = *(pb++);

      /* Fetch state variables for acc0, acc1, acc2, acc3 */
      x0 = *(px0++);
      x1 = *(px1++);
      x2 = *(px2++);
      x3 = *(px3++);

      /* Perform the multiply-accumulate */
      acc0 += x0 * c0;
      acc1 += x1 * c0;
      acc2 += x2 * c0;
      acc3 += x3 * c0;

      /* Decrement loop counter */
      tapCnt--;
    }

    /* Advance the state pointer by the decimation factor
     * to process the next group of decimation factor number samples */
    pState = pState + S->M * 4;

    /* The result is in the accumulator, store in the destination buffer. */
    *pDst++ = acc0;
    *pDst++ = acc1;
    *pDst++ = acc2;
    *pDst++ = acc3;

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Loop unrolling: Compute remaining samples */
  blkCnt = outBlockSize % 0x4U;

#else

  /* Initialize blkCnt with number of samples */
  blkCnt = outBlockSize;

#endif /* #if defined (ARM_MATH_LOOPUNROLL) */

  while (blkCnt > 0U)
  {
    /* Copy decimation factor number of new input samples into the state buffer */
    i = S->M;

    do
    {
      *pStateCur++ = *pSrc++;

    } while (--i);

    /* Set accumulator to zero */
    acc0 = 0.0f;

    /* Initialize state pointer */
    px0 = pState;

    /* Initialize coeff pointer */
    pb = pCoeffs;

#if defined (ARM_MATH_LOOPUNROLL)

    /* Loop unrolling: Compute 4 taps at a time */
    tapCnt = numTaps >> 2U;

    while (tapCnt > 0U)
    {
      /* Read the b[numTaps-1] coefficient */
      c0 = *pb++;

      /* Read x[n-numTaps-1] sample */
      x0 = *px0++;

      /* Perform the multiply-accumulate */
      acc0 += x0 * c0;

      /* Read the b[numTaps-2] coefficient */
      c0 = *pb++;

      /* Read x[n-numTaps-2] sample */
      x0 = *px0++;

      /* Perform the multiply-accumulate */
      acc0 += x0 * c0;

      /* Read the b[numTaps-3] coefficient */
      c0 = *pb++;

      /* Read x[n-numTaps-3] sample */
      x0 = *px0++;

      /* Perform the multiply-accumulate */
      acc0 += x0 * c0;

      /* Read the b[numTaps-4] coefficient */
      c0 = *pb++;

      /* Read x[n-numTaps-4] sample */
      x0 = *px0++;

      /* Perform the multiply-accumulate */
      acc0 += x0 * c0;

      /* Decrement loop counter */
      tapCnt--;
    }

    /* Loop unrolling: Compute remaining taps */
    tapCnt = numTaps % 0x4U;

#else

    /* Initialize tapCnt with number of taps */
    tapCnt = numTaps;

#endif /* #if defined (ARM_MATH_LOOPUNROLL) */

    while (tapCnt > 0U)
    {
      /* Read coefficients */
      c0 = *pb++;

      /* Fetch 1 state variable */
      x0 = *px0++;

      /* Perform the multiply-accumulate */
      acc0 += x0 * c0;

      /* Decrement loop counter */
      tapCnt--;
    }

    /* Advance the state pointer by the decimation factor
     * to process the next group of decimation factor number samples */
    pState = pState + S->M;

    /* The result is in the accumulator, store in the destination buffer. */
    *pDst++ = acc0;

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Processing is complete.
     Now copy the last numTaps - 1 samples to the satrt of the state buffer.
     This prepares the state buffer for the next function call. */

  /* Points to the start of the state buffer */
  pStateCur = S->pState;

#if defined (ARM_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 taps at a time */
  tapCnt = (numTaps - 1U) >> 2U;

  /* Copy data */
  while (tapCnt > 0U)
  {
    *pStateCur++ = *pState++;
    *pStateCur++ = *pState++;
    *pStateCur++ = *pState++;
    *pStateCur++ = *pState++;

    /* Decrement loop counter */
    tapCnt--;
  }

  /* Loop unrolling: Compute remaining taps */
  tapCnt = (numTaps - 1U) % 0x04U;

#else

  /* Initialize tapCnt with number of taps */
  tapCnt = (numTaps - 1U);

#endif /* #if defined (ARM_MATH_LOOPUNROLL) */

  /* Copy data */
  while (tapCnt > 0U)
  {
    *pStateCur++ = *pState++;

    /* Decrement loop counter */
    tapCnt--;
  }

}
#endif /* #if defined(ARM_MATH_NEON) */

#endif /*defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE) */
/**
  @} end of FIR_decimate group
 */
