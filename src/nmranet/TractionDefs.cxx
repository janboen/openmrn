/** \copyright
 * Copyright (c) 2014, Balazs Racz
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are  permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \file TractionDefs.cxx
 *
 * Implementations for static functions related to traction protocol.
 *
 * @author Balazs Racz
 * @date 5 May 2014
 */

#include "nmranet/TractionDefs.hxx"

#include <string.h>
#include "utils/macros.h"

extern "C" {
extern int singles2halfp(void *target, void *source, int numel);
extern int halfp2singles(void *target, void *source, int numel);
}

namespace NMRAnet {

SpeedType fp16_to_speed(const void *fp16) {
    float speed;
    DASSERT(sizeof(speed) == 4);
    uint16_t input;
    const uint8_t* in_p = static_cast<const uint8_t*>(fp16);
    // We assemble the input assuming it is big-endian.
    input = *in_p++;
    input <<= 8;
    input |= *in_p;
    halfp2singles(&speed, &input, 1);
    return speed;
}

void speed_to_fp16(SpeedType speed, void *fp16) {
    DASSERT(sizeof(speed) == 4);
    uint16_t output;
    singles2halfp(&output, &speed, 1);
    uint8_t* o = static_cast<uint8_t*>(fp16);
    *o++ = output >> 8;
    *o = output & 0xff;
}

NanSpeedImpl::NanSpeedImpl() {
    uint32_t d = 0xFFFF0000;
    memcpy(&value, &d, 4);
}

NanSpeedImpl NAN_SPEED;

}  // namespace NMRAnet