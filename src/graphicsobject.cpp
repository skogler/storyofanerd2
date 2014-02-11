/*------------------------------------------------------------------------/
 *
 * Copyright (c) 2013 David Robin Cvetko
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the 
 * "Software"), to deal in the Software without restriction, including 
 * without limitation the rights to use, copy, modify, merge, publish, 
 * distribute, sublicense, and/or sell copies of the Software, and to 
 * permit persons to whom the Software is furnished to do so, subject to 
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *-----------------------------------------------------------------------*/

#include "graphicsobject.h"

///////////////////////////////////////////////////////////////////////////

GraphicsObject::GraphicsObject(shared_ptr<SDL_Texture> texture,
                               int x, int y, uint h, uint w) :
                m_enabled(true),
                m_texture(texture),
                m_clip(NULL)
{
    SDL_Rect *dst = new SDL_Rect();
    assert(dst);
    dst->x = x;
    dst->y = y;
    dst->h = h;
    dst->w = w;
    m_dst.reset(dst);
}

///////////////////////////////////////////////////////////////////////////

GraphicsObject::GraphicsObject(shared_ptr<SDL_Texture> texture,
                               shared_ptr<SDL_Rect> clip,
                               shared_ptr<SDL_Rect> dst) :
                m_enabled(true),
                m_texture(texture),
                m_clip(clip), m_dst(dst)
{
}

///////////////////////////////////////////////////////////////////////////

GraphicsObject::GraphicsObject(shared_ptr<SDL_Rect> dst_rct) :
    m_enabled(true),
    m_texture(NULL),
    m_clip(NULL),
    m_dst(dst_rct)
{
}

///////////////////////////////////////////////////////////////////////////

GraphicsObject::~GraphicsObject()
{
    //TODO: cleanup
}

///////////////////////////////////////////////////////////////////////////

void GraphicsObject::drawObject()
{
    if(m_enabled == false)
    {
        return;
    }

    if(m_texture == NULL && m_clip == NULL && m_dst != NULL)
    {
        GraphicsCore::instance().renderRectange(m_dst.get());
    }
    else if(m_clip != NULL)
    {
        GraphicsCore::instance().renderTextureClip(m_texture.get(), m_clip.get(), m_dst.get());
    }
    else
    {
        GraphicsCore::instance().renderTextureDstOnly(m_texture.get(), m_dst.get());
    }
}

///////////////////////////////////////////////////////////////////////////

bool GraphicsObject::hasCollision(const GraphicsObject &other)
{
    return SDL_HasIntersection(m_dst.get(), other.getDst().get());
}

///////////////////////////////////////////////////////////////////////////
