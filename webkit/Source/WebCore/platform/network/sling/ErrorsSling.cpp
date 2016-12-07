/*
 * Copyright (C) 2013 Naver Corp. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "ErrorsSling.h"

#include "ResourceError.h"
#include "ResourceRequest.h"
#include "ResourceResponse.h"

namespace WebCore {

ResourceError cancelledError(const ResourceRequest& request)
{
    return ResourceError(errorDomainNetwork, NetworkErrorCancelled, request.url(), ASCIILiteral("Load request cancelled"), ResourceError::Type::AccessControl);
}

ResourceError blockedError(const ResourceRequest& request)
{
    return ResourceError(errorDomainPolicy, PolicyErrorCannotUseRestrictedPort, request.url(), ASCIILiteral("Not allowed to use restricted network port"), ResourceError::Type::AccessControl);
}

ResourceError cannotShowURLError(const ResourceRequest& request)
{
    return ResourceError(errorDomainPolicy, PolicyErrorCannotShowURL, request.url(), ASCIILiteral("URL cannot be shown"));
}

ResourceError interruptedForPolicyChangeError(const ResourceRequest& request)
{
    return ResourceError(errorDomainPolicy, PolicyErrorFrameLoadInterruptedByPolicyChange, request.url(), ASCIILiteral("Frame load was interrupted"), ResourceError::Type::AccessControl);
}

ResourceError cannotShowMIMETypeError(const ResourceResponse& response)
{
    return ResourceError(errorDomainPolicy, PolicyErrorCannotShowMimeType, response.url(), ASCIILiteral("Content with the specified MIME type cannot be shown"), ResourceError::Type::AccessControl);
}

ResourceError fileDoesNotExistError(const ResourceResponse& response)
{
    return ResourceError(errorDomainNetwork, NetworkErrorFileDoesNotExist, response.url(), ASCIILiteral("File does not exist"), ResourceError::Type::AccessControl);
}

ResourceError pluginWillHandleLoadError(const ResourceResponse& response)
{
    return ResourceError(errorDomainPlugin, PluginErrorWillHandleLoad, response.url(), ASCIILiteral("Plugin will handle load"), ResourceError::Type::AccessControl);
}

ResourceError downloadNetworkError(const ResourceError& networkError)
{
    return ResourceError(errorDomainDownload, DownloadErrorNetwork, networkError.failingURL(), networkError.localizedDescription(), ResourceError::Type::AccessControl);
}

ResourceError downloadCancelledByUserError(const ResourceResponse& response)
{
    return ResourceError(errorDomainDownload, DownloadErrorCancelledByUser, response.url(), ASCIILiteral("User cancelled the download"), ResourceError::Type::AccessControl);
}

ResourceError downloadDestinationError(const ResourceResponse& response, const String& errorMessage)
{
    return ResourceError(errorDomainDownload, DownloadErrorDestination, response.url(), errorMessage, ResourceError::Type::AccessControl);
}

ResourceError printError(const URL& failingURL, const String& errorMessage)
{
    return ResourceError(errorDomainPrint, PrintErrorGeneral, failingURL, errorMessage, ResourceError::Type::AccessControl);
}

ResourceError printerNotFoundError(const URL& failingURL)
{
    return ResourceError(errorDomainPrint, PrintErrorPrinterNotFound, failingURL, ASCIILiteral("Printer not found"), ResourceError::Type::AccessControl);
}

ResourceError invalidPageRangeToPrint(const URL& failingURL)
{
    return ResourceError(errorDomainPrint, PrintErrorInvalidPageRange, failingURL, ASCIILiteral("Invalid page range"), ResourceError::Type::AccessControl);
}

} // namespace WebCore