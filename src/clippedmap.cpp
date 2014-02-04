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

#include "clippedmap.h"
#include <SDL2/SDL_image.h>
#include <sstream>

///////////////////////////////////////////////////////////////////////////

ClippedMap::ClippedMap(LoadedMap *lmap) :
    GameObject("map"),
    m_loaded_map(lmap),
    m_tile_set_surface(NULL),
    m_tile_set(NULL)
{
    loadTexture();
    createClips();
    parseTileData();
}

///////////////////////////////////////////////////////////////////////////

ClippedMap::~ClippedMap()
{
}

///////////////////////////////////////////////////////////////////////////

void ClippedMap::copyTilesToRender(int viewport_x, int viewport_y)
{
    Logger.logMessage(LOG_STATE, LOG_MAP, "ClippedMap::copyTilesToRender start\n");

    uint x_coord = 0;
    uint y_coord = 0;

    int current_clip = 0;
    for(vector<int>::iterator it = m_tile_data_parsed.begin();
        it != m_tile_data_parsed.end(); ++it)
    {
        current_clip = *it;

        shared_ptr<SDL_Rect> dst(new SDL_Rect());
        dst->x = x_coord - viewport_x;
        dst->y = y_coord - viewport_y;

        dst->w = m_map_clips.at(current_clip).get()->w;
        dst->h = m_map_clips.at(current_clip).get()->h;

        if(current_clip != 0)
        {
            shared_ptr<GraphicsObject> gobj(new GraphicsObject(m_tile_set,
                                            m_map_clips.at(current_clip),
                                            dst));
            addGraphicsObject(gobj);
        }

        x_coord = x_coord + m_loaded_map->getTileMap().tilewidth;
        if(x_coord == m_loaded_map->getTileMap().width * m_loaded_map->getTileMap().tilewidth)
        {
            x_coord = 0;
            y_coord = y_coord + m_loaded_map->getTileMap().tileheight;
        }
    }

    Logger.logMessage(LOG_STATE, LOG_MAP, "ClippedMap::copyTilesToRender end\n");
}

///////////////////////////////////////////////////////////////////////////

void ClippedMap::createClips()
{
    Logger.logMessage(LOG_STATE, LOG_MAP, "ClippedMap::createClips start\n");

    int tile_w  = m_loaded_map->getTileMap().tilewidth;
    int tile_h  = m_loaded_map->getTileMap().tileheight;

    //TODO: naming
    uint number_tiles_width  = m_surface_width / tile_w;
    uint number_tiles_height = m_surface_height / tile_h;

    for(uint i = 0; i < number_tiles_width; i++)
    {
        for(uint j = 0; j < number_tiles_height; j++)
        {
            shared_ptr<SDL_Rect> rect(new SDL_Rect());
            rect.get()->x  = i + tile_w;
            rect.get()->y  = j + tile_h;
            rect.get()->w  = tile_w;
            rect.get()->h  = tile_h;
            m_map_clips.push_back(rect);
        }
    }

    Logger.logMessage(LOG_DEBUG, LOG_MAP, "ClippedMap::createClips: Created %d clips\n",
                      m_map_clips.size());
    Logger.logMessage(LOG_STATE, LOG_MAP, "ClippedMap::createClips end\n");
}

///////////////////////////////////////////////////////////////////////////

void ClippedMap::loadTexture()
{
    Logger.logMessage(LOG_STATE, LOG_MAP, "ClippedMap::loadTexture start\n");

    SDL_Surface *tile_set_surface = IMG_Load(("../res/maps/" + m_loaded_map->getImageName(0)).c_str());
    assert(tile_set_surface);

    SDL_Texture *tile_set = SDL_CreateTextureFromSurface(&(GraphicsCore::instance().getRenderer()), tile_set_surface);
    assert(tile_set);

    m_tile_set_surface.reset(tile_set_surface, SDL_FreeSurface);
    m_tile_set.reset(tile_set, SDL_DestroyTexture);

    m_surface_width = m_tile_set_surface->w;
    m_surface_height = m_tile_set_surface->h;

    Logger.logMessage(LOG_STATE, LOG_MAP, "ClippedMap::loadTexture end\n");
}

///////////////////////////////////////////////////////////////////////////

void ClippedMap::parseTileData()
{
    Logger.logMessage(LOG_STATE, LOG_MAP, "ClippedMap::parseTileData start\n");

    string tile_data    = m_loaded_map->getLayerData(0);

    std::stringstream ss(tile_data);
    int vec_index;
    while(ss >> vec_index)
    {
        m_tile_data_parsed.push_back(vec_index);
        if(ss.peek() == ',')
        {
            ss.ignore();
        }
    }

    Logger.logMessage(LOG_STATE, LOG_MAP, "ClippedMap::parseTileData end\n");
}

///////////////////////////////////////////////////////////////////////////

