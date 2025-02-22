/*
 * Copyright (C) 2010 Google, Inc. All Rights Reserved.
 * Copyright (C) 2014 Apple, Inc. All Rights Reserved.
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

#ifndef ResourceLoadTiming_h
#define ResourceLoadTiming_h

#if PLATFORM(COCOA)
OBJC_CLASS NSDictionary;
#endif

namespace WebCore {
    
class ResourceLoadTiming {
public:
    ResourceLoadTiming()
        : domainLookupStart(-1)
        , domainLookupEnd(-1)
        , connectStart(-1)
        , connectEnd(-1)
        , requestStart(0)
        , responseStart(0)
        , secureConnectionStart(-1)
    {
    }
    
    ResourceLoadTiming(const ResourceLoadTiming& other)
        : domainLookupStart(other.domainLookupStart)
        , domainLookupEnd(other.domainLookupEnd)
        , connectStart(other.connectStart)
        , connectEnd(other.connectEnd)
        , requestStart(other.requestStart)
        , responseStart(other.responseStart)
        , secureConnectionStart(other.secureConnectionStart)
    {
    }
    
    ResourceLoadTiming& operator=(const ResourceLoadTiming& other)
    {
        domainLookupStart = other.domainLookupStart;
        domainLookupEnd = other.domainLookupEnd;
        connectStart = other.connectStart;
        connectEnd = other.connectEnd;
        requestStart = other.requestStart;
        responseStart = other.responseStart;
        secureConnectionStart = other.secureConnectionStart;
        return *this;
    }

    ResourceLoadTiming isolatedCopy() const
    {
        // There are currently no members that need isolated copies, so we can use the copy constructor.
        return *this;
    }
    
    bool operator==(const ResourceLoadTiming& other) const
    {
        return domainLookupStart == other.domainLookupStart
            && domainLookupEnd == other.domainLookupEnd
            && connectStart == other.connectStart
            && connectEnd == other.connectEnd
            && requestStart == other.requestStart
            && responseStart == other.responseStart
            && secureConnectionStart == other.secureConnectionStart;
    }

    bool operator!=(const ResourceLoadTiming& other) const
    {
        return !(*this == other);
    }

    template<class Encoder> void encode(Encoder&) const;
    template<class Decoder> static bool decode(Decoder&, ResourceLoadTiming&);

    // These are millisecond deltas from the navigation start.
    int domainLookupStart;
    int domainLookupEnd;
    int connectStart;
    int connectEnd;
    int requestStart;
    int responseStart;
    int secureConnectionStart;
};

#if PLATFORM(COCOA)
WEBCORE_EXPORT void copyTimingData(NSDictionary *timingData, ResourceLoadTiming&);
#endif

#if PLATFORM(COCOA) && !HAVE(TIMINGDATAOPTIONS)
WEBCORE_EXPORT void setCollectsTimingData();
#endif
    
template<class Encoder>
void ResourceLoadTiming::encode(Encoder& encoder) const
{
    encoder << domainLookupStart;
    encoder << domainLookupEnd;
    encoder << connectStart;
    encoder << connectEnd;
    encoder << requestStart;
    encoder << responseStart;
    encoder << secureConnectionStart;
}

template<class Decoder>
bool ResourceLoadTiming::decode(Decoder& decoder, ResourceLoadTiming& timing)
{
    return decoder.decode(timing.domainLookupStart)
        && decoder.decode(timing.domainLookupEnd)
        && decoder.decode(timing.connectStart)
        && decoder.decode(timing.connectEnd)
        && decoder.decode(timing.requestStart)
        && decoder.decode(timing.responseStart)
        && decoder.decode(timing.secureConnectionStart);
}

}

#endif
