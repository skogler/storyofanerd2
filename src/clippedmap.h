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

#ifndef CLIPPEDMAP_H
#define CLIPPEDMAP_H

#include "common.h"
#include "gameobject.h"

#include <SDL2/SDL.h>
#include <vector>

using std::vector;

class LoadedMap;

///////////////////////////////////////////////////////////////////////////

class ClippedMap : public GameObject
{
    DISABLECOPY(ClippedMap);

    public:
        explicit ClippedMap(LoadedMap* lmap);
        virtual ~ClippedMap();

//        virtual void update();
        void copyTilesToRender(int viewport_x, int viewport_y);
        void copyMapBorderToRender();

    private:
        void loadTexture();
        void createClips();
        void parseTileData();
        void createMapBorder();

        LoadedMap* m_loaded_map;
        int m_surface_width;
        int m_surface_height;

        vector<shared_ptr<SDL_Rect> > m_map_clips;
        vector<shared_ptr<SDL_Rect> > m_map_borders;
        vector<int> m_tile_data_parsed;

        shared_ptr<SDL_Surface> m_tile_set_surface;
        shared_ptr<SDL_Texture> m_tile_set;
};

///////////////////////////////////////////////////////////////////////////

#endif
