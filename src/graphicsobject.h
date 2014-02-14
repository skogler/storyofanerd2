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

#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H

#include <SDL2/SDL.h>

#include "common.h"

///////////////////////////////////////////////////////////////////////////

class GraphicsObject
{
        DISABLECOPY(GraphicsObject);

    public:
        GraphicsObject(shared_ptr<SDL_Texture> texture,
                       int x, int y, uint h = 0, uint w = 0);
        GraphicsObject(shared_ptr<SDL_Texture> texture,
                       shared_ptr<SDL_Rect> clip,
                       shared_ptr<SDL_Rect> dst);
        GraphicsObject(shared_ptr<SDL_Rect> dst_rct);

        void drawObject();

        inline bool getEnabled() const
        {
            return m_enabled;
        }

        inline void setEnabled(bool enabled)
        {
            m_enabled = enabled;
        }

        inline void setX(int x)
        {
            m_dst.get()->x = x;
        }

        inline void setY(int y)
        {
            m_dst.get()->y = y;
        }

        inline int getX() const
        {
            return m_dst.get()->x;
        }

        inline int getY() const
        {
            return m_dst.get()->y;
        }

        inline int getXW() const
        {
            return m_dst.get()->x + m_dst.get()->w;
        }

        inline int getYH() const
        {
            return m_dst.get()->y + m_dst.get()->h;
        }

        inline shared_ptr<SDL_Rect> getDst() const
        {
            return m_dst;
        }

        bool hasCollision(const GraphicsObject& other);

        ~GraphicsObject();

    private:
        bool                    m_enabled;
        shared_ptr<SDL_Texture> m_texture;

        shared_ptr<SDL_Rect>        m_clip;
        shared_ptr<SDL_Rect>        m_dst;
};

///////////////////////////////////////////////////////////////////////////

#endif
