/** \copyright
 * Copyright (c) 2013, Balazs Racz
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
 * \file NMRAnetEventTestHelper.hxx
 *
 * Helper utilities for testing event handlers.
 *
 * This file must only ever be included in unittests.
 *
 * @author Balazs Racz
 * @date 7 December 2013
 */

#ifndef _NMRAnetEventTestHelper_hxx_
#define _NMRAnetEventTestHelper_hxx_

#include "gmock/gmock.h"
#include "nmranet/NMRAnetEventRegistry.hxx"

namespace NMRAnet {

class MockEventHandler : public NMRAnetEventHandler {
 public:
  MOCK_METHOD2(HandleEventReport, void(EventReport* event, Notifiable* done));
  MOCK_METHOD2(HandleConsumerIdentified,
               void(EventReport* event, Notifiable* done));
  MOCK_METHOD2(HandleConsumerRangeIdentified,
               void(EventReport* event, Notifiable* done));
  MOCK_METHOD2(HandleProducerIdentified,
               void(EventReport* event, Notifiable* done));
  MOCK_METHOD2(HandleProducerRangeIdentified,
               void(EventReport* event, Notifiable* done));
  MOCK_METHOD2(HandleIdentifyGlobal,
               void(EventReport* event, Notifiable* done));
  MOCK_METHOD2(HandleIdentifyConsumer,
               void(EventReport* event, Notifiable* done));
  MOCK_METHOD2(HandleIdentifyProducer,
               void(EventReport* event, Notifiable* done));
};

}  // namespace NMRAnet

#endif // _NMRAnetEventHandlerTemplates_hxx_

