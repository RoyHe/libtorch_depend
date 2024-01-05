// Auto-generated file. Do not edit!
//   Template: src/qs8-vcvt/sse4.c.in
//   Generator: tools/xngen
//
// Copyright 2022 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <immintrin.h>

#include <xnnpack/common.h>
#include <xnnpack/intrinsics-polyfill.h>
#include <xnnpack/vcvt.h>


void xnn_qu8_vcvt_ukernel__avx_x16(
    size_t batch,
    const uint8_t* input,
    uint8_t* output,
    const union xnn_qu8_cvt_params params[restrict XNN_MIN_ELEMENTS(1)]) XNN_OOB_READS
{
  assert(batch != 0);
  assert(batch % sizeof(uint8_t) == 0);
  assert(input != NULL);
  assert(output != NULL);

  const __m128i vinput_zero_point = _mm_load_si128((const __m128i*) params->ssse3.input_zero_point);
  const __m128i vmultiplier = _mm_load_si128((const __m128i*) params->ssse3.multiplier);
  const __m128i voutput_zero_point = _mm_load_si128((const __m128i*) params->ssse3.output_zero_point);
  for (; batch >= 16 * sizeof(uint8_t); batch -= 16 * sizeof(uint8_t)) {
    __m128i vacc0 = _mm_cvtepu8_epi16(_mm_loadl_epi64((const __m128i*) input));
    __m128i vacc1 = _mm_cvtepu8_epi16(_mm_loadl_epi64((const __m128i*) (input + 8)));
    input += 16;

    vacc0 = _mm_sub_epi16(vinput_zero_point, vacc0);
    vacc1 = _mm_sub_epi16(vinput_zero_point, vacc1);

    vacc0 = _mm_slli_epi16(vacc0, 7);
    vacc1 = _mm_slli_epi16(vacc1, 7);

    vacc0 = _mm_mulhrs_epi16(vacc0, vmultiplier);
    vacc1 = _mm_mulhrs_epi16(vacc1, vmultiplier);

    vacc0 = _mm_adds_epi16(vacc0, voutput_zero_point);
    vacc1 = _mm_adds_epi16(vacc1, voutput_zero_point);

    const __m128i vy0 = _mm_packus_epi16(vacc0, vacc1);

    _mm_storeu_si128((__m128i*) output, vy0);
    output += 16;
  }
  for (; batch >= 8 * sizeof(uint8_t); batch -= 8 * sizeof(uint8_t)) {
    __m128i vacc = _mm_cvtepu8_epi16(_mm_loadl_epi64((const __m128i*) input));
    vacc = _mm_sub_epi16(vinput_zero_point, vacc);
    vacc = _mm_slli_epi16(vacc, 7);
    vacc = _mm_mulhrs_epi16(vacc, vmultiplier);
    vacc = _mm_adds_epi16(vacc, voutput_zero_point);
    input += 8;

    const __m128i vy = _mm_packus_epi16(vacc, vacc);
    _mm_storel_epi64((__m128i*) output, vy);
    output += 8;
  }
  if XNN_UNLIKELY(batch != 0) {
    assert(batch >= 1 * sizeof(uint8_t));
    assert(batch <= 7 * sizeof(uint8_t));

    __m128i vacc = _mm_cvtepu8_epi16(_mm_loadl_epi64((const __m128i*) input));
    vacc = _mm_sub_epi16(vinput_zero_point, vacc);
    vacc = _mm_slli_epi16(vacc, 7);
    vacc = _mm_mulhrs_epi16(vacc, vmultiplier);
    vacc = _mm_adds_epi16(vacc, voutput_zero_point);

    __m128i vy = _mm_packus_epi16(vacc, vacc);
    if (batch & (4 * sizeof(uint8_t))) {
      _mm_storeu_si32(output, vy);
      vy = _mm_srli_epi64(vy, 32);
      output += 4;
    }
    if (batch & (2 * sizeof(uint8_t))) {
      _mm_storeu_si16(output, vy);
      vy = _mm_srli_epi32(vy, 16);
      output += 2;
    }
    if (batch & (1 * sizeof(uint8_t))) {
      *output = (uint8_t) _mm_extract_epi8(vy, 0);
    }
  }
}
