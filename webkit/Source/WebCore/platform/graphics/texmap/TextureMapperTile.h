/*
 Copyright (C) 2013 Nokia Corporation and/or its subsidiary(-ies)

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Library General Public
 License as published by the Free Software Foundation; either
 version 2 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Library General Public License for more details.

 You should have received a copy of the GNU Library General Public License
 along with this library; see the file COPYING.LIB.  If not, write to
 the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 Boston, MA 02110-1301, USA.
 */

#ifndef TextureMapperTile_h
#define TextureMapperTile_h

#include "FloatRect.h"
#include "Image.h"
#include "TextureMapper.h"
#include "TextureMapperPlatformLayer.h"
#include <wtf/RefPtr.h>

namespace WebCore {

class GraphicsLayer;

class TextureMapperTile {
public:
    inline RefPtr<BitmapTexture> texture() const { return m_texture; }
    inline FloatRect rect() const { return m_rect; }
    inline FloatRect visibleRect() const { return m_visibleRect; }
    inline void setTexture(BitmapTexture* texture) { m_texture = texture; }
    inline void setRect(const FloatRect& rect) { m_rect = rect; }
    inline void setVisibleRect(const FloatRect& rect) { m_visibleRect = rect; }

    void updateContents(TextureMapper&, Image*, const IntRect&, BitmapTexture::UpdateContentsFlag UpdateCanModifyOriginalImageData);
    void updateContents(TextureMapper&, GraphicsLayer*, const IntRect&, BitmapTexture::UpdateContentsFlag UpdateCanModifyOriginalImageData, float scale = 1);
    virtual void paint(TextureMapper&, const TransformationMatrix&, float, const unsigned exposedEdges);
    virtual ~TextureMapperTile() { }

    explicit TextureMapperTile(const FloatRect& rect, const FloatRect& visibleRect)
        : m_rect(rect)
        , m_visibleRect(visibleRect)
    {
    }

private:
    RefPtr<BitmapTexture> m_texture;
    FloatRect m_rect;
    FloatRect m_visibleRect;
};

}

#endif
