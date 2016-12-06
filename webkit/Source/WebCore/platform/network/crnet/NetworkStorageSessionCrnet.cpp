/*
 * Copyright (C) 2013 Apple Inc. All rights reserved.
 * Copyright (C) 2013 University of Szeged. All rights reserved.
 * Copyright (C) 2015 NAVER Corp.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "NetworkStorageSession.h"

#include "ResourceHandle.h"
#include <crnet.h>
#include <wtf/MainThread.h>
#include <wtf/NeverDestroyed.h>

namespace WebCore {

NetworkStorageSession::NetworkStorageSession(SessionID, struct crnet_context* context)
    : m_crnet_context(context)
{
}

NetworkStorageSession::~NetworkStorageSession()
{
}

static std::unique_ptr<NetworkStorageSession>& defaultSession()
{
    ASSERT(isMainThread());
    static NeverDestroyed<std::unique_ptr<NetworkStorageSession>> session;
    return session;
}

NetworkStorageSession& NetworkStorageSession::defaultStorageSession()
{
    if (!defaultSession())
        defaultSession() = std::make_unique<NetworkStorageSession>(SessionID::defaultSessionID(), crnet_default_context());
    return *defaultSession();
}

std::unique_ptr<NetworkStorageSession> NetworkStorageSession::createPrivateBrowsingSession(SessionID sessionID, const String&)
{
    auto session = std::make_unique<NetworkStorageSession>(SessionID::defaultSessionID(), crnet_private_context());
    return session;
}

#if USE(CRNET)
void NetworkStorageSession::switchToPrivateBrowsingSession()
{
    defaultSession() = std::make_unique<NetworkStorageSession>(SessionID::defaultSessionID(), crnet_private_context());
}
#endif

void NetworkStorageSession::switchToNewTestingSession()
{
    // FIXME: Figure out what testing requires and adjust crnet's context.
    // For now, ignore this request and continue to use the default context.
}

struct crnet_context* NetworkStorageSession::crnetContext() const
{
    return m_crnet_context;
}

}
