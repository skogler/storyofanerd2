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

#include "graphics.h"

///////////////////////////////////////////////////////////////////////////

GraphicsCore::GraphicsCore() :
    m_main_window(NULL),
    m_renderer(NULL)
{
}

///////////////////////////////////////////////////////////////////////////

GraphicsCore::~GraphicsCore()
{
    destroyRenderer();
    destroyWindow();

    SDL_Quit();
}

///////////////////////////////////////////////////////////////////////////

ErrorCode GraphicsCore::initializeWindow()
{
    Logger.logMessage(LOG_STATE, LOG_SDL2_GRAPHICS,
                     "GraphicsCore::initializeWindow start\n");

    assert(m_main_window == NULL);

    //TODO: move SDL_INIT_EVERYTHING somewhere else
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        Logger.logMessage(LOG_ERROR, LOG_SDL2_GRAPHICS,
                          "GraphicsCore::initializeWindow: %s", SDLERROR());
        return ERROR_SDL_INIT;
    }

    m_main_window = SDL_CreateWindow("TEST", 100, 100, 640, 480,
                                     SDL_WINDOW_SHOWN);

    if(m_main_window == nullptr)
    {
        Logger.logMessage(LOG_ERROR, LOG_SDL2_GRAPHICS,
                          "GraphicsCore::initializeWindow: %s", SDLERROR());
        return ERROR_SDL_INIT;
    }

    Logger.logMessage(LOG_STATE, LOG_SDL2_GRAPHICS,
                     "GraphicsCore::initializeWindow end\n");
    return OK;
}

///////////////////////////////////////////////////////////////////////////

ErrorCode GraphicsCore::initializeRenderer()
{
    Logger.logMessage(LOG_STATE, LOG_SDL2_GRAPHICS,
                     "GraphicsCore::initializeRenderer start\n");

    assert(m_renderer == NULL);

    m_renderer = SDL_CreateRenderer(m_main_window, -1,
                                    SDL_RENDERER_ACCELERATED |
                                    SDL_RENDERER_PRESENTVSYNC);

    if(m_renderer == nullptr)
    {
        Logger.logMessage(LOG_ERROR, LOG_SDL2_GRAPHICS,
                          "GraphicsCore::initializeRenderer: %s", SDLERROR());
        return ERROR_SDL_INIT;
    }

    Logger.logMessage(LOG_STATE, LOG_SDL2_GRAPHICS,
                     "GraphicsCore::initializeRenderer end\n");
    return OK;
}

///////////////////////////////////////////////////////////////////////////

void GraphicsCore::destroyWindow()
{
    Logger.logMessage(LOG_STATE, LOG_SDL2_GRAPHICS,
                     "GraphicsCore::destroyWindow start\n");

    if(m_main_window != NULL)
    {
        SDL_DestroyWindow(m_main_window);
    }

    Logger.logMessage(LOG_STATE, LOG_SDL2_GRAPHICS,
                     "GraphicsCore::destroyWindow end\n");
}

///////////////////////////////////////////////////////////////////////////

void GraphicsCore::destroyRenderer()
{
    Logger.logMessage(LOG_STATE, LOG_SDL2_GRAPHICS,
                     "GraphicsCore::destroyRenderer start\n");

    if(m_renderer != NULL)
    {
        SDL_DestroyRenderer(m_renderer);
    }

    Logger.logMessage(LOG_STATE, LOG_SDL2_GRAPHICS,
                     "GraphicsCore::destroyRenderer end\n");
}

///////////////////////////////////////////////////////////////////////////

SDL_Texture* GraphicsCore::createTextureFromBMP(const string& filename)
{
    SDL_Surface *bmp = SDL_LoadBMP(filename.c_str());

    if(bmp == nullptr)
    {
        Logger.logMessage(LOG_ERROR, LOG_SDL2_GRAPHICS,
                          "GraphicsCore::createTextureFromBMP: "
                          "Error loading file (%s)\n", SDLERROR());
        return NULL;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(m_renderer,
                                                    bmp);

    SDL_FreeSurface(bmp);

    if(tex == nullptr)
    {
        Logger.logMessage(LOG_ERROR, LOG_SDL2_GRAPHICS,
                          "GraphicsCore::createTextureFromBMP: "
                          "Error creating texture (%s)\n", SDLERROR());
        return NULL;
    }

    return tex;
}

///////////////////////////////////////////////////////////////////////////

void GraphicsCore::renderTextureClip(SDL_Texture *tex, int x, int y, SDL_Rect *clip)
{
    assert(tex);
    assert(clip);

    SDL_Rect dst;

    dst.x   = x;
    dst.y   = y;

    dst.w   = clip->w;
    dst.h   = clip->h;

    SDL_RenderCopy(m_renderer, tex, clip, &dst);
}

///////////////////////////////////////////////////////////////////////////

void GraphicsCore::renderTextureClip(SDL_Texture *tex, SDL_Rect *clip, SDL_Rect *dst)
{
    assert(tex);
    assert(clip);
    assert(dst);

    SDL_RenderCopy(m_renderer, tex, clip, dst);
}

///////////////////////////////////////////////////////////////////////////

void GraphicsCore::clearRenderer()
{
    SDL_RenderClear(m_renderer);
}

///////////////////////////////////////////////////////////////////////////

void GraphicsCore::presentRenderer()
{
    SDL_RenderPresent(m_renderer);
}

///////////////////////////////////////////////////////////////////////////

void GraphicsCore::renderTexture(SDL_Texture *tex, int x, int y,
                                 uint h, uint w)
{
    assert(tex);

    SDL_Rect dst;

    dst.x   = x;
    dst.y   = y;

    if(h != 0 && w != 0)
    {
        dst.h   = h;
        dst.w   = w;
    }
    else
    {
        if((h != 0) != (w != 0))
        {
            Logger.logMessage(LOG_WARNING, LOG_SDL2_GRAPHICS,
                              "GraphicsCore::renderTexture: "
                              "One of the texture scaling values has been "
                              "set to 0. Ignoring set values...\n");
        }
        SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    }

    SDL_RenderCopy(m_renderer, tex, NULL, &dst);
}

///////////////////////////////////////////////////////////////////////////

void GraphicsCore::renderTextureDstOnly(SDL_Texture *tex, SDL_Rect *dst)
{
    assert(tex);
    assert(dst);

    if(dst->h == 0 || dst->w == 0)
    {
        SDL_QueryTexture(tex, NULL, NULL, &(dst->w), &(dst->h));
    }

    SDL_RenderCopy(m_renderer, tex, NULL, dst);
}

///////////////////////////////////////////////////////////////////////////
