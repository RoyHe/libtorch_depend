// Auto-generated file. Do not edit!
//   Template: src/qs8-vaddc/sse-mul16-ld64.c.in
//   Generator: tools/xngen
//
// Copyright 2020 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <emmintrin.h>

#include <xnnpack/unaligned.h>
#include <xnnpack/vadd.h>


void xnn_qs8_vaddc_minmax_ukernel__sse2_mul16_ld64_x24(
    size_t batch,
    const int8_t* input_a,
    const int8_t* input_b,
    int8_t* output,
    const union xnn_qs8_add_minmax_params params[restrict XNN_MIN_ELEMENTS(1)]) XNN_OOB_READS
{
  assert(batch != 0);
  assert(batch % sizeof(int8_t) == 0);
  assert(input_a != NULL);
  assert(input_b != NULL);
  assert(output != NULL);

  const __m128i vbias = _mm_add_epi32(
    _mm_shuffle_epi32(_mm_cvtsi32_si128(params->sse2.b_multiplier * (int32_t) *input_b), _MM_SHUFFLE(0, 0, 0, 0)),
    _mm_load_si128((const __m128i*) params->sse2.bias));
  const __m128i va_multiplier_lo = _mm_load_si128((const __m128i*) params->sse2.a_multiplier_lo);
  const __m128i va_multiplier_hi = _mm_load_si128((const __m128i*) params->sse2.a_multiplier_hi);
  const __m128i vshift = _mm_cvtsi32_si128((int) params->sse2.shift);
  const __m128i voutput_zero_point = _mm_load_si128((const __m128i*) params->sse2.output_zero_point);
  const __m128i voutput_min = _mm_load_si128((const __m128i*) params->sse2.output_min);
  const __m128i voutput_max = _mm_load_si128((const __m128i*) params->sse2.output_max);

  for (; batch >= 24 * sizeof(int8_t); batch -= 24 * sizeof(int8_t)) {
    __m128i va01234567 = _mm_loadl_epi64((const __m128i*) input_a);
    __m128i va89ABCDEF = _mm_loadl_epi64((const __m128i*) (input_a + 8));
    __m128i vaGHIJKLMN = _mm_loadl_epi64((const __m128i*) (input_a + 16));
    input_a += 24;

    va01234567 = _mm_srai_epi16(_mm_unpacklo_epi8(va01234567, va01234567), 8);
    va89ABCDEF = _mm_srai_epi16(_mm_unpacklo_epi8(va89ABCDEF, va89ABCDEF), 8);
    vaGHIJKLMN = _mm_srai_epi16(_mm_unpacklo_epi8(vaGHIJKLMN, vaGHIJKLMN), 8);

    __m128i vaprod01234567hi = _mm_mulhi_epu16(va01234567, va_multiplier_lo);
    const __m128i vaprod01234567lo = _mm_mullo_epi16(va01234567, va_multiplier_lo);
    __m128i vaprod89ABCDEFhi = _mm_mulhi_epu16(va89ABCDEF, va_multiplier_lo);
    const __m128i vaprod89ABCDEFlo = _mm_mullo_epi16(va89ABCDEF, va_multiplier_lo);
    __m128i vaprodGHIJKLMNhi = _mm_mulhi_epu16(vaGHIJKLMN, va_multiplier_lo);
    const __m128i vaprodGHIJKLMNlo = _mm_mullo_epi16(vaGHIJKLMN, va_multiplier_lo);

    vaprod01234567hi = _mm_add_epi16(vaprod01234567hi, _mm_mullo_epi16(va01234567, va_multiplier_hi));
    vaprod89ABCDEFhi = _mm_add_epi16(vaprod89ABCDEFhi, _mm_mullo_epi16(va89ABCDEF, va_multiplier_hi));
    vaprodGHIJKLMNhi = _mm_add_epi16(vaprodGHIJKLMNhi, _mm_mullo_epi16(vaGHIJKLMN, va_multiplier_hi));

    vaprod01234567hi = _mm_sub_epi16(vaprod01234567hi, _mm_and_si128(_mm_srai_epi16(va01234567, 15), va_multiplier_lo));
    vaprod89ABCDEFhi = _mm_sub_epi16(vaprod89ABCDEFhi, _mm_and_si128(_mm_srai_epi16(va89ABCDEF, 15), va_multiplier_lo));
    vaprodGHIJKLMNhi = _mm_sub_epi16(vaprodGHIJKLMNhi, _mm_and_si128(_mm_srai_epi16(vaGHIJKLMN, 15), va_multiplier_lo));

    __m128i vacc0123 = _mm_add_epi32(vbias, _mm_unpacklo_epi16(vaprod01234567lo, vaprod01234567hi));
    __m128i vacc4567 = _mm_add_epi32(vbias, _mm_unpackhi_epi16(vaprod01234567lo, vaprod01234567hi));
    __m128i vacc89AB = _mm_add_epi32(vbias, _mm_unpacklo_epi16(vaprod89ABCDEFlo, vaprod89ABCDEFhi));
    __m128i vaccCDEF = _mm_add_epi32(vbias, _mm_unpackhi_epi16(vaprod89ABCDEFlo, vaprod89ABCDEFhi));
    __m128i vaccGHIJ = _mm_add_epi32(vbias, _mm_unpacklo_epi16(vaprodGHIJKLMNlo, vaprodGHIJKLMNhi));
    __m128i vaccKLMN = _mm_add_epi32(vbias, _mm_unpackhi_epi16(vaprodGHIJKLMNlo, vaprodGHIJKLMNhi));

    vacc0123 = _mm_sra_epi32(vacc0123, vshift);
    vacc4567 = _mm_sra_epi32(vacc4567, vshift);
    vacc89AB = _mm_sra_epi32(vacc89AB, vshift);
    vaccCDEF = _mm_sra_epi32(vaccCDEF, vshift);
    vaccGHIJ = _mm_sra_epi32(vaccGHIJ, vshift);
    vaccKLMN = _mm_sra_epi32(vaccKLMN, vshift);

    __m128i vout01234567 = _mm_adds_epi16(_mm_packs_epi32(vacc0123, vacc4567), voutput_zero_point);
    __m128i vout89ABCDEF = _mm_adds_epi16(_mm_packs_epi32(vacc89AB, vaccCDEF), voutput_zero_point);
    __m128i voutGHIJKLMN = _mm_adds_epi16(_mm_packs_epi32(vaccGHIJ, vaccKLMN), voutput_zero_point);

    vout01234567 = _mm_max_epi16(vout01234567, voutput_min);
    vout89ABCDEF = _mm_max_epi16(vout89ABCDEF, voutput_min);
    voutGHIJKLMN = _mm_max_epi16(voutGHIJKLMN, voutput_min);

    vout01234567 = _mm_min_epi16(vout01234567, voutput_max);
    vout89ABCDEF = _mm_min_epi16(vout89ABCDEF, voutput_max);
    voutGHIJKLMN = _mm_min_epi16(voutGHIJKLMN, voutput_max);

    __m128i vout0123456789ABCDEF = _mm_packs_epi16(vout01234567, vout89ABCDEF);
    __m128i voutGHIJKLMNGHIJKLMN = _mm_packs_epi16(voutGHIJKLMN, voutGHIJKLMN);


    _mm_storeu_si128((__m128i*) output, vout0123456789ABCDEF);
    _mm_storel_epi64((__m128i*) (output + 16), voutGHIJKLMNGHIJKLMN);
    output += 24;
  }
  if XNN_UNLIKELY(batch != 0) {
    do {
      __m128i va01234567 = _mm_loadl_epi64((const __m128i*) input_a);
      input_a += 8;

      va01234567 = _mm_srai_epi16(_mm_unpacklo_epi8(va01234567, va01234567), 8);

      __m128i vaprod01234567hi = _mm_mulhi_epu16(va01234567, va_multiplier_lo);
      const __m128i vaprod01234567lo = _mm_mullo_epi16(va01234567, va_multiplier_lo);

      vaprod01234567hi = _mm_add_epi16(vaprod01234567hi, _mm_mullo_epi16(va01234567, va_multiplier_hi));

      vaprod01234567hi = _mm_sub_epi16(vaprod01234567hi, _mm_and_si128(_mm_srai_epi16(va01234567, 15), va_multiplier_lo));

      __m128i vacc0123 = _mm_add_epi32(vbias, _mm_unpacklo_epi16(vaprod01234567lo, vaprod01234567hi));
      __m128i vacc4567 = _mm_add_epi32(vbias, _mm_unpackhi_epi16(vaprod01234567lo, vaprod01234567hi));

      vacc0123 = _mm_sra_epi32(vacc0123, vshift);
      vacc4567 = _mm_sra_epi32(vacc4567, vshift);

      __m128i vout01234567 = _mm_adds_epi16(_mm_packs_epi32(vacc0123, vacc4567), voutput_zero_point);
      vout01234567 = _mm_max_epi16(vout01234567, voutput_min);
      vout01234567 = _mm_min_epi16(vout01234567, voutput_max);

      __m128i vout0123456701234567 = _mm_packs_epi16(vout01234567, vout01234567);

      if XNN_LIKELY(batch >= (8 * sizeof(int8_t))) {
        _mm_storel_epi64((__m128i*) output, vout0123456701234567);
        output += 8;
        batch -= 8 * sizeof(int8_t);
      } else {
        if (batch & (4 * sizeof(int8_t))) {
          unaligned_store_u32(output, (uint32_t) _mm_cvtsi128_si32(vout0123456701234567));
          vout0123456701234567 = _mm_srli_epi64(vout0123456701234567, 32);
          output += 4;
        }
        if (batch & (2 * sizeof(int8_t))) {
          unaligned_store_u16(output, (uint16_t) _mm_cvtsi128_si32(vout0123456701234567));
          vout0123456701234567 = _mm_srli_epi32(vout0123456701234567, 16);
          output += 2;
        }
        if (batch & (1 * sizeof(int8_t))) {
          *output = (int8_t) _mm_cvtsi128_si32(vout0123456701234567);
        }
        batch = 0;
      }
    } while (batch != 0);
  }
}
