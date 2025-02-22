/*
 * Copyright (C) 2016 Igalia S.L. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "config.h"

#if ENABLE(MATHML)
#include "MathMLPaddedElement.h"

#include "RenderMathMLPadded.h"

namespace WebCore {

using namespace MathMLNames;

inline MathMLPaddedElement::MathMLPaddedElement(const QualifiedName& tagName, Document& document)
    : MathMLInlineContainerElement(tagName, document)
{
}

Ref<MathMLPaddedElement> MathMLPaddedElement::create(const QualifiedName& tagName, Document& document)
{
    return adoptRef(*new MathMLPaddedElement(tagName, document));
}

const MathMLElement::Length& MathMLPaddedElement::width()
{
    return cachedMathMLLength(MathMLNames::widthAttr, m_width);
}

const MathMLElement::Length& MathMLPaddedElement::height()
{
    return cachedMathMLLength(MathMLNames::heightAttr, m_height);
}

const MathMLElement::Length& MathMLPaddedElement::depth()
{
    return cachedMathMLLength(MathMLNames::depthAttr, m_depth);
}

const MathMLElement::Length& MathMLPaddedElement::lspace()
{
    return cachedMathMLLength(MathMLNames::lspaceAttr, m_lspace);
}

const MathMLElement::Length& MathMLPaddedElement::voffset()
{
    return cachedMathMLLength(MathMLNames::voffsetAttr, m_voffset);
}

void MathMLPaddedElement::parseAttribute(const QualifiedName& name, const AtomicString& value)
{
    if (name == widthAttr)
        m_width.dirty = true;
    else if (name == heightAttr)
        m_height.dirty = true;
    else if (name == depthAttr)
        m_depth.dirty = true;
    else if (name == lspaceAttr)
        m_lspace.dirty = true;
    else if (name == voffsetAttr)
        m_voffset.dirty = true;

    MathMLElement::parseAttribute(name, value);
}

RenderPtr<RenderElement> MathMLPaddedElement::createElementRenderer(RenderStyle&& style, const RenderTreePosition&)
{
    ASSERT(hasTagName(MathMLNames::mpaddedTag));
    return createRenderer<RenderMathMLPadded>(*this, WTFMove(style));
}

}

#endif // ENABLE(MATHML)
