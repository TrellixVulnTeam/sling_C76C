/*
 * Copyright (C) 2005, 2006, 2009 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer. 
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution. 
 * 3.  Neither the name of Apple Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "FrameLoaderTypes.h"
#include "ResourceLoader.h"

#include <wtf/text/WTFString.h>
 
namespace WebCore {

class CachedResource;
class CachedResourceLoader;
class Document;
class ResourceRequest;
class SecurityOrigin;

class SubresourceLoader final : public ResourceLoader {
public:
    WEBCORE_EXPORT static RefPtr<SubresourceLoader> create(Frame&, CachedResource&, const ResourceRequest&, const ResourceLoaderOptions&);

    virtual ~SubresourceLoader();

    void cancelIfNotFinishing();
    bool isSubresourceLoader() override;
    CachedResource* cachedResource();

#if PLATFORM(IOS)
    bool startLoading() override;

    // FIXME: What is an "iOS" original request? Why is it necessary?
    const ResourceRequest& iOSOriginalRequest() const override { return m_iOSOriginalRequest; }
#endif

private:
    SubresourceLoader(Frame&, CachedResource&, const ResourceLoaderOptions&);

    bool init(const ResourceRequest&) override;

    void willSendRequestInternal(ResourceRequest&, const ResourceResponse& redirectResponse) override;
    void didSendData(unsigned long long bytesSent, unsigned long long totalBytesToBeSent) override;
    void didReceiveResponse(const ResourceResponse&) override;
    void didReceiveData(const char*, unsigned, long long encodedDataLength, DataPayloadType) override;
    void didReceiveBuffer(Ref<SharedBuffer>&&, long long encodedDataLength, DataPayloadType) override;
    void didFinishLoading(double finishTime) override;
    void didFail(const ResourceError&) override;
    void willCancel(const ResourceError&) override;
    void didCancel(const ResourceError&) override;

#if PLATFORM(COCOA) && !USE(CFNETWORK)
    NSCachedURLResponse *willCacheResponse(ResourceHandle*, NSCachedURLResponse*) override;
#endif
#if PLATFORM(COCOA) && USE(CFNETWORK)
    CFCachedURLResponseRef willCacheResponse(ResourceHandle*, CFCachedURLResponseRef) override;
#endif

#if USE(NETWORK_CFDATA_ARRAY_CALLBACK)
    bool supportsDataArray() override { return true; }
    void didReceiveDataArray(CFArrayRef) override;
#endif
    void releaseResources() override;

#if USE(SOUP)
    char* getOrCreateReadBuffer(size_t requestedSize, size_t& actualSize) override;
#endif

    bool checkForHTTPStatusCodeError();
    bool checkCrossOriginAccessControl(const ResourceRequest&, const ResourceResponse&, ResourceRequest& newRequest);

    void didReceiveDataOrBuffer(const char*, int, RefPtr<SharedBuffer>&&, long long encodedDataLength, DataPayloadType);

    void notifyDone();

    enum SubresourceLoaderState {
        Uninitialized,
        Initialized,
        Finishing,
#if PLATFORM(IOS)
        CancelledWhileInitializing
#endif
    };

    class RequestCountTracker {
#if !COMPILER(MSVC)
        WTF_MAKE_FAST_ALLOCATED;
#endif
    public:
        RequestCountTracker(CachedResourceLoader&, const CachedResource&);
        ~RequestCountTracker();
    private:
        CachedResourceLoader& m_cachedResourceLoader;
        const CachedResource& m_resource;
    };

#if PLATFORM(IOS)
    ResourceRequest m_iOSOriginalRequest;
#endif
    CachedResource* m_resource;
    bool m_loadingMultipartContent;
    SubresourceLoaderState m_state;
    Optional<RequestCountTracker> m_requestCountTracker;
    RefPtr<SecurityOrigin> m_origin;
};

}
