/*
 * Copyright (C) 2009 Alex Milowski (alex@milowski.com). All rights reserved.
 * Copyright (C) 2012 David Barton (dbarton@mathscribe.com). All rights reserved.
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
 */

#include "config.h"

#if ENABLE(MATHML)

#include "RenderMathMLBlock.h"

#include "CSSHelper.h"
#include "GraphicsContext.h"
#include "LayoutRepainter.h"
#include "MathMLElement.h"
#include "MathMLNames.h"
#include "RenderView.h"

#if ENABLE(DEBUG_MATH_LAYOUT)
#include "PaintInfo.h"
#endif

namespace WebCore {

using namespace MathMLNames;

RenderMathMLBlock::RenderMathMLBlock(Element& container, RenderStyle&& style)
    : RenderBlock(container, WTFMove(style), 0)
    , m_mathMLStyle(MathMLStyle::create())
{
    setChildrenInline(false); // All of our children must be block-level.
}

RenderMathMLBlock::RenderMathMLBlock(Document& document, RenderStyle&& style)
    : RenderBlock(document, WTFMove(style), 0)
    , m_mathMLStyle(MathMLStyle::create())
{
    setChildrenInline(false); // All of our children must be block-level.
}

RenderMathMLBlock::~RenderMathMLBlock()
{
}

bool RenderMathMLBlock::isChildAllowed(const RenderObject& child, const RenderStyle&) const
{
    return is<Element>(child.node());
}

static LayoutUnit axisHeight(const RenderStyle& style)
{
    // If we have a MATH table we just return the AxisHeight constant.
    const auto& primaryFont = style.fontCascade().primaryFont();
    if (auto* mathData = primaryFont.mathData())
        return mathData->getMathConstant(primaryFont, OpenTypeMathData::AxisHeight);

    // Otherwise, the idea is to try and use the middle of operators as the math axis which we thus approximate by "half of the x-height".
    // Note that Gecko has a slower but more accurate version that measures half of the height of U+2212 MINUS SIGN.
    return style.fontMetrics().xHeight() / 2;
}

LayoutUnit RenderMathMLBlock::mathAxisHeight() const
{
    return axisHeight(style());
}

LayoutUnit RenderMathMLBlock::mirrorIfNeeded(LayoutUnit horizontalOffset, LayoutUnit boxWidth) const
{
    if (style().direction() == RTL)
        return logicalWidth() - boxWidth - horizontalOffset;

    return horizontalOffset;
}

int RenderMathMLBlock::baselinePosition(FontBaseline baselineType, bool firstLine, LineDirectionMode direction, LinePositionMode linePositionMode) const
{
    // mathml.css sets math { -webkit-line-box-contain: glyphs replaced; line-height: 0; }, so when linePositionMode == PositionOfInteriorLineBoxes we want to
    // return 0 here to match our line-height. This matters when RootInlineBox::ascentAndDescentForBox is called on a RootInlineBox for an inline-block.
    if (linePositionMode == PositionOfInteriorLineBoxes)
        return 0;

    return firstLineBaseline().valueOr(RenderBlock::baselinePosition(baselineType, firstLine, direction, linePositionMode));
}

#if ENABLE(DEBUG_MATH_LAYOUT)
void RenderMathMLBlock::paint(PaintInfo& info, const LayoutPoint& paintOffset)
{
    RenderBlock::paint(info, paintOffset);

    if (info.context().paintingDisabled() || info.phase != PaintPhaseForeground)
        return;

    IntPoint adjustedPaintOffset = roundedIntPoint(paintOffset + location());

    GraphicsContextStateSaver stateSaver(info.context());

    info.context().setStrokeThickness(1.0f);
    info.context().setStrokeStyle(SolidStroke);
    info.context().setStrokeColor(Color(0, 0, 255));

    info.context().drawLine(adjustedPaintOffset, IntPoint(adjustedPaintOffset.x() + pixelSnappedOffsetWidth(), adjustedPaintOffset.y()));
    info.context().drawLine(IntPoint(adjustedPaintOffset.x() + pixelSnappedOffsetWidth(), adjustedPaintOffset.y()), IntPoint(adjustedPaintOffset.x() + pixelSnappedOffsetWidth(), adjustedPaintOffset.y() + pixelSnappedOffsetHeight()));
    info.context().drawLine(IntPoint(adjustedPaintOffset.x(), adjustedPaintOffset.y() + pixelSnappedOffsetHeight()), IntPoint(adjustedPaintOffset.x() + pixelSnappedOffsetWidth(), adjustedPaintOffset.y() + pixelSnappedOffsetHeight()));
    info.context().drawLine(adjustedPaintOffset, IntPoint(adjustedPaintOffset.x(), adjustedPaintOffset.y() + pixelSnappedOffsetHeight()));

    int topStart = paddingTop();

    info.context().setStrokeColor(Color(0, 255, 0));

    info.context().drawLine(IntPoint(adjustedPaintOffset.x(), adjustedPaintOffset.y() + topStart), IntPoint(adjustedPaintOffset.x() + pixelSnappedOffsetWidth(), adjustedPaintOffset.y() + topStart));

    int baseline = roundToInt(baselinePosition(AlphabeticBaseline, true, HorizontalLine));

    info.context().setStrokeColor(Color(255, 0, 0));

    info.context().drawLine(IntPoint(adjustedPaintOffset.x(), adjustedPaintOffset.y() + baseline), IntPoint(adjustedPaintOffset.x() + pixelSnappedOffsetWidth(), adjustedPaintOffset.y() + baseline));
}
#endif // ENABLE(DEBUG_MATH_LAYOUT)

LayoutUnit toUserUnits(const MathMLElement::Length& length, const RenderStyle& style, const LayoutUnit& referenceValue)
{
    switch (length.type) {
    case MathMLElement::LengthType::Cm:
        return length.value * cssPixelsPerInch / 2.54f;
    case MathMLElement::LengthType::Em:
        return length.value * style.fontCascade().size();
    case MathMLElement::LengthType::Ex:
        return length.value * style.fontMetrics().xHeight();
    case MathMLElement::LengthType::In:
        return length.value * cssPixelsPerInch;
    case MathMLElement::LengthType::MathUnit:
        return length.value * style.fontCascade().size() / 18;
    case MathMLElement::LengthType::Mm:
        return length.value * cssPixelsPerInch / 25.4f;
    case MathMLElement::LengthType::Pc:
        return length.value * cssPixelsPerInch / 6;
    case MathMLElement::LengthType::Percentage:
        return referenceValue * length.value / 100;
    case MathMLElement::LengthType::Pt:
        return length.value * cssPixelsPerInch / 72;
    case MathMLElement::LengthType::Px:
        return length.value;
    case MathMLElement::LengthType::UnitLess:
        return referenceValue * length.value;
    case MathMLElement::LengthType::ParsingFailed:
        return referenceValue;
    default:
        ASSERT_NOT_REACHED();
        return referenceValue;
    }
}

bool parseMathMLLength(const String& string, LayoutUnit& lengthValue, const RenderStyle* style, bool allowNegative)
{
    MathMLElement::Length length = MathMLElement::parseMathMLLength(string);
    if (length.type == MathMLElement::LengthType::ParsingFailed)
        return false;

    LayoutUnit value = toUserUnits(length, *style, lengthValue);

    if (!allowNegative && value < 0)
        return false;

    lengthValue = value;
    return true;
}

Optional<int> RenderMathMLTable::firstLineBaseline() const
{
    // By default the vertical center of <mtable> is aligned on the math axis.
    // This is different than RenderTable::firstLineBoxBaseline, which returns the baseline of the first row of a <table>.
    return Optional<int>(logicalHeight() / 2 + axisHeight(style()));
}

void RenderMathMLBlock::layoutItems(bool relayoutChildren)
{
    LayoutUnit verticalOffset = borderBefore() + paddingBefore();
    LayoutUnit horizontalOffset = borderStart() + paddingStart();

    LayoutUnit preferredHorizontalExtent = 0;
    for (auto* child = firstChildBox(); child; child = child->nextSiblingBox()) {
        LayoutUnit childHorizontalExtent = child->maxPreferredLogicalWidth() - child->horizontalBorderAndPaddingExtent();
        LayoutUnit childHorizontalMarginBoxExtent = child->horizontalBorderAndPaddingExtent() + childHorizontalExtent;
        childHorizontalMarginBoxExtent += child->horizontalMarginExtent();

        preferredHorizontalExtent += childHorizontalMarginBoxExtent;
    }

    LayoutUnit currentHorizontalExtent = contentLogicalWidth();
    for (auto* child = firstChildBox(); child; child = child->nextSiblingBox()) {
        LayoutUnit childSize = child->maxPreferredLogicalWidth() - child->horizontalBorderAndPaddingExtent();

        if (preferredHorizontalExtent > currentHorizontalExtent)
            childSize = currentHorizontalExtent;

        LayoutUnit childPreferredSize = childSize + child->horizontalBorderAndPaddingExtent();

        if (childPreferredSize != child->width())
            child->setChildNeedsLayout(MarkOnlyThis);

        updateBlockChildDirtyBitsBeforeLayout(relayoutChildren, *child);
        child->layoutIfNeeded();

        LayoutUnit childVerticalMarginBoxExtent;
        childVerticalMarginBoxExtent = child->height() + child->verticalMarginExtent();

        setLogicalHeight(std::max(logicalHeight(), verticalOffset + borderAfter() + paddingAfter() + childVerticalMarginBoxExtent + horizontalScrollbarHeight()));

        horizontalOffset += child->marginStart();

        LayoutUnit childHorizontalExtent = child->width();
        LayoutPoint childLocation(style().isLeftToRightDirection() ? horizontalOffset : width() - horizontalOffset - childHorizontalExtent,
            verticalOffset + child->marginBefore());

        child->setLocation(childLocation);
        horizontalOffset += childHorizontalExtent + child->marginEnd();
    }
}

void RenderMathMLBlock::layoutBlock(bool relayoutChildren, LayoutUnit)
{
    ASSERT(needsLayout());

    if (!relayoutChildren && simplifiedLayout())
        return;

    LayoutRepainter repainter(*this, checkForRepaintDuringLayout());

    if (recomputeLogicalWidth())
        relayoutChildren = true;

    setLogicalHeight(borderAndPaddingLogicalHeight() + scrollbarLogicalHeight());

    layoutItems(relayoutChildren);

    updateLogicalHeight();

    repainter.repaintAfterLayout();

    clearNeedsLayout();
}

}

#endif
