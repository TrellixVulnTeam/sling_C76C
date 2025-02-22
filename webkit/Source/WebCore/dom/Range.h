/*
 * (C) 1999 Lars Knoll (knoll@kde.org)
 * (C) 2000 Gunnstein Lye (gunnstein@netcom.no)
 * (C) 2000 Frederik Holljen (frederik.holljen@hig.no)
 * (C) 2001 Peter Kelly (pmk@post.com)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef Range_h
#define Range_h

#include "ExceptionCodePlaceholder.h"
#include "FloatRect.h"
#include "FragmentScriptingPermission.h"
#include "IntRect.h"
#include "RangeBoundaryPoint.h"
#include <wtf/Forward.h>
#include <wtf/RefCounted.h>
#include <wtf/Vector.h>

namespace WebCore {

class ClientRect;
class ClientRectList;
class ContainerNode;
class Document;
class DocumentFragment;
class FloatQuad;
class Node;
class NodeWithIndex;
class SelectionRect;
class Text;
class VisiblePosition;

class Range : public RefCounted<Range> {
public:
    WEBCORE_EXPORT static Ref<Range> create(Document&);
    WEBCORE_EXPORT static Ref<Range> create(Document&, PassRefPtr<Node> startContainer, int startOffset, PassRefPtr<Node> endContainer, int endOffset);
    WEBCORE_EXPORT static Ref<Range> create(Document&, const Position&, const Position&);
    WEBCORE_EXPORT static Ref<Range> create(Document&, const VisiblePosition&, const VisiblePosition&);
    WEBCORE_EXPORT ~Range();

    Document& ownerDocument() const { return const_cast<Document&>(m_ownerDocument.get()); }

    Node& startContainer() const { ASSERT(m_start.container()); return *m_start.container(); }
    int startOffset() const { return m_start.offset(); }
    Node& endContainer() const { ASSERT(m_end.container()); return *m_end.container(); }
    int endOffset() const { return m_end.offset(); }
    bool collapsed() const { return m_start == m_end; }

    Node* commonAncestorContainer() const { return commonAncestorContainer(&startContainer(), &endContainer()); }
    WEBCORE_EXPORT static Node* commonAncestorContainer(Node* containerA, Node* containerB);
    WEBCORE_EXPORT void setStart(Ref<Node>&& container, int offset, ExceptionCode& = ASSERT_NO_EXCEPTION);
    WEBCORE_EXPORT void setEnd(Ref<Node>&& container, int offset, ExceptionCode& = ASSERT_NO_EXCEPTION);
    WEBCORE_EXPORT void collapse(bool toStart);
    WEBCORE_EXPORT bool isPointInRange(Node& refNode, int offset, ExceptionCode&);
    short comparePoint(Node& refNode, int offset, ExceptionCode&) const;
    enum CompareResults { NODE_BEFORE, NODE_AFTER, NODE_BEFORE_AND_AFTER, NODE_INSIDE };
    CompareResults compareNode(Node& refNode, ExceptionCode&) const;
    enum CompareHow { START_TO_START, START_TO_END, END_TO_END, END_TO_START };
    WEBCORE_EXPORT short compareBoundaryPoints(CompareHow, const Range& sourceRange, ExceptionCode&) const;
    short compareBoundaryPointsForBindings(unsigned short compareHow, const Range& sourceRange, ExceptionCode&) const;
    static short compareBoundaryPoints(Node* containerA, int offsetA, Node* containerB, int offsetB, ExceptionCode&);
    static short compareBoundaryPoints(const RangeBoundaryPoint& boundaryA, const RangeBoundaryPoint& boundaryB, ExceptionCode&);
    WEBCORE_EXPORT bool boundaryPointsValid() const;
    bool intersectsNode(Node& refNode, ExceptionCode&) const;
    void deleteContents(ExceptionCode&);
    RefPtr<DocumentFragment> extractContents(ExceptionCode&);
    RefPtr<DocumentFragment> cloneContents(ExceptionCode&);
    void insertNode(Ref<Node>&&, ExceptionCode&);
    String toString() const;

    String toHTML() const;
    WEBCORE_EXPORT String text() const;

    RefPtr<DocumentFragment> createContextualFragment(const String& html, ExceptionCode&);

    void detach();
    WEBCORE_EXPORT Ref<Range> cloneRange() const;

    WEBCORE_EXPORT void setStartAfter(Node&, ExceptionCode& = ASSERT_NO_EXCEPTION);
    WEBCORE_EXPORT void setEndBefore(Node&, ExceptionCode& = ASSERT_NO_EXCEPTION);
    WEBCORE_EXPORT void setEndAfter(Node&, ExceptionCode& = ASSERT_NO_EXCEPTION);
    WEBCORE_EXPORT void selectNode(Node&, ExceptionCode& = ASSERT_NO_EXCEPTION);
    WEBCORE_EXPORT void selectNodeContents(Node&, ExceptionCode&);
    void surroundContents(Node&, ExceptionCode&);
    WEBCORE_EXPORT void setStartBefore(Node&, ExceptionCode&);

    const Position startPosition() const { return m_start.toPosition(); }
    const Position endPosition() const { return m_end.toPosition(); }
    WEBCORE_EXPORT void setStart(const Position&, ExceptionCode& = ASSERT_NO_EXCEPTION);
    WEBCORE_EXPORT void setEnd(const Position&, ExceptionCode& = ASSERT_NO_EXCEPTION);

    WEBCORE_EXPORT Node* firstNode() const;
    WEBCORE_EXPORT Node* pastLastNode() const;

    ShadowRoot* shadowRoot() const;

    enum RangeInFixedPosition {
        NotFixedPosition,
        PartiallyFixedPosition,
        EntirelyFixedPosition
    };

    // Not transform-friendly
    WEBCORE_EXPORT void absoluteTextRects(Vector<IntRect>&, bool useSelectionHeight = false, RangeInFixedPosition* = nullptr) const;
    WEBCORE_EXPORT IntRect absoluteBoundingBox() const;

    // Transform-friendly
    WEBCORE_EXPORT void absoluteTextQuads(Vector<FloatQuad>&, bool useSelectionHeight = false, RangeInFixedPosition* = nullptr) const;
    WEBCORE_EXPORT FloatRect absoluteBoundingRect() const;
#if PLATFORM(IOS)
    WEBCORE_EXPORT void collectSelectionRects(Vector<SelectionRect>&);
#endif

    void nodeChildrenChanged(ContainerNode&);
    void nodeChildrenWillBeRemoved(ContainerNode&);
    void nodeWillBeRemoved(Node&);

    void textInserted(Node*, unsigned offset, unsigned length);
    void textRemoved(Node*, unsigned offset, unsigned length);
    void textNodesMerged(NodeWithIndex& oldNode, unsigned offset);
    void textNodeSplit(Text* oldNode);

    // Expand range to a unit (word or sentence or block or document) boundary.
    // Please refer to https://bugs.webkit.org/show_bug.cgi?id=27632 comment #5 
    // for details.
    void expand(const String&, ExceptionCode&);

    Ref<ClientRectList> getClientRects() const;
    Ref<ClientRect> getBoundingClientRect() const;

#if ENABLE(TREE_DEBUGGING)
    void formatForDebugger(char* buffer, unsigned length) const;
#endif

    WEBCORE_EXPORT bool contains(const Range&) const;
    bool contains(const VisiblePosition&) const;

private:
    explicit Range(Document&);
    Range(Document&, PassRefPtr<Node> startContainer, int startOffset, PassRefPtr<Node> endContainer, int endOffset);

    void setDocument(Document&);

    Node* checkNodeWOffset(Node&, int offset, ExceptionCode&) const;

    enum ActionType { Delete, Extract, Clone };
    RefPtr<DocumentFragment> processContents(ActionType, ExceptionCode&);
    static RefPtr<Node> processContentsBetweenOffsets(ActionType, PassRefPtr<DocumentFragment>, Node*, unsigned startOffset, unsigned endOffset, ExceptionCode&);
    static void processNodes(ActionType, Vector<Ref<Node>>&, Node* oldContainer, Node* newContainer, ExceptionCode&);
    enum ContentsProcessDirection { ProcessContentsForward, ProcessContentsBackward };
    static RefPtr<Node> processAncestorsAndTheirSiblings(ActionType, Node* container, ContentsProcessDirection, RefPtr<Node>&& clonedContainer, Node* commonRoot, ExceptionCode&);

    enum class CoordinateSpace { Absolute, Client };
    void getBorderAndTextQuads(Vector<FloatQuad>&, CoordinateSpace) const;
    FloatRect boundingRectInternal(CoordinateSpace) const;

    Ref<Document> m_ownerDocument;
    RangeBoundaryPoint m_start;
    RangeBoundaryPoint m_end;
};

WEBCORE_EXPORT Ref<Range> rangeOfContents(Node&);

WEBCORE_EXPORT bool areRangesEqual(const Range*, const Range*);
bool rangesOverlap(const Range*, const Range*);

inline bool documentOrderComparator(const Node* a, const Node* b)
{
    return Range::compareBoundaryPoints(const_cast<Node*>(a), 0, const_cast<Node*>(b), 0, ASSERT_NO_EXCEPTION) < 0;
}

} // namespace

#if ENABLE(TREE_DEBUGGING)
// Outside the WebCore namespace for ease of invocation from gdb.
void showTree(const WebCore::Range*);
#endif

#endif
