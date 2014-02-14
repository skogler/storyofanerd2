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

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>

#include "common.h"

struct SDL_Window;
struct SDL_Renderer;

///////////////////////////////////////////////////////////////////////////

class GraphicsCore
{
    public:
        static GraphicsCore& instance()
        {
            static GraphicsCore instance;
            return instance;
        }

        SDL_Window& getMainWindow()
        {
            return *m_main_window;
        }

        SDL_Renderer& getRenderer()
        {
            return *m_renderer;
        }

        ErrorCode initializeWindow();
        ErrorCode initializeRenderer();
        void clearRenderer();
        void presentRenderer();

        SDL_Texture* createTextureFromBMP(const string& filename);
        void renderTexture(SDL_Texture* tex, int x, int y,
                           uint h = 0, uint w = 0);
        void renderTextureDstOnly(SDL_Texture* tex, SDL_Rect* dst);
        void renderTextureClip(SDL_Texture* tex, int x, int y,
                               SDL_Rect* clip);
        void renderTextureClip(SDL_Texture* tex, SDL_Rect* clip, SDL_Rect* dst);
        void renderRectange(SDL_Rect* rect);

        void updateViewportRelativeTo(uint x, uint y);

        inline const int getViewportXOffset() const
        {
            return m_viewport_x_offset;
        }

        inline const int getViewportYOffset() const
        {
            return m_viewport_y_offset;
        }

        //TODO! currently working here // getter/setter for viewport limit

    private:
        GraphicsCore();
        virtual ~GraphicsCore();

        void destroyWindow();
        void destroyRenderer();

        SDL_Window*      m_main_window;
        SDL_Renderer*    m_renderer;

        int            m_viewport_x_offset;
        int            m_viewport_y_offset;

        int            m_viewport_limit_top;
        int            m_viewport_limit_bottom;
        int            m_viewport_limit_left;
        int            m_viewport_limit_right;

        DISABLECOPY(GraphicsCore);
};

///////////////////////////////////////////////////////////////////////////

#define MainWindow GraphicsCore::instance().getMainWindow()
#define Renderer GraphicsCore::instance().getRenderer()

///////////////////////////////////////////////////////////////////////////

#endif
