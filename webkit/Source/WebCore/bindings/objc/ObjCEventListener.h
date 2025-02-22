/*
 * Copyright (C) 2004, 2006, 2008, 2010 Apple Inc. All rights reserved.
 * Copyright (C) 2006 Samuel Weinig <sam.weinig@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ObjCEventListener_h
#define ObjCEventListener_h

#include "EventListener.h"
#include <wtf/Forward.h>
#include <wtf/RetainPtr.h>

@protocol DOMEventListener;

namespace WebCore {

    class ObjCEventListener : public EventListener {
    public:
        typedef id<DOMEventListener> ObjCListener;
        static Ref<ObjCEventListener> wrap(ObjCListener);

        static const ObjCEventListener* cast(const EventListener* listener)
        {
            return listener->type() == ObjCEventListenerType
                ? static_cast<const ObjCEventListener*>(listener)
                : 0;
        }

    private:
        static ObjCEventListener* find(ObjCListener);

        ObjCEventListener(ObjCListener);
        virtual ~ObjCEventListener();
        virtual bool operator==(const EventListener&) const;
        virtual void handleEvent(ScriptExecutionContext*, Event*);

        RetainPtr<ObjCListener> m_listener;
    };

} // namespace WebCore

#endif
