/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2000 Simon Hausmann <hausmann@kde.org>
 * Copyright (C) 2004, 2006, 2010 Apple Inc. All rights reserved.
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

#ifndef HTMLHtmlElement_h
#define HTMLHtmlElement_h

#include "HTMLElement.h"

namespace WebCore {

class HTMLHtmlElement final : public HTMLElement {
public:
    static Ref<HTMLHtmlElement> create(Document&);
    static Ref<HTMLHtmlElement> create(const QualifiedName&, Document&);

    void insertedByParser();

private:
    HTMLHtmlElement(const QualifiedName&, Document&);

    bool isURLAttribute(const Attribute&) const override;
};

} // namespace

#endif
