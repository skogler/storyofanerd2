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

#include "objecthandler.h"

#include "gameobject.h"
#include "clippedmap.h"
#include "player.h"

///////////////////////////////////////////////////////////////////////////

Objecthandler::Objecthandler()
{
}

///////////////////////////////////////////////////////////////////////////

Objecthandler::~Objecthandler()
{
}

///////////////////////////////////////////////////////////////////////////

void Objecthandler::reactToKeyEvent(InputEvent event)
{
    for(const auto& object : m_game_objects)
    {
        if(object->handleKeyEvent(event) == true)
        {
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////

void Objecthandler::updateAll()
{
    for(const auto& object : m_game_objects)
    {
        object->update();
    }
}

///////////////////////////////////////////////////////////////////////////

void Objecthandler::drawAll()
{
    for(const auto& object : m_game_objects)
    {
        object->drawAll();
    }
}

///////////////////////////////////////////////////////////////////////////

void Objecthandler::addGameObject(shared_ptr<GameObject> object)
{
    m_game_objects.push_back(object);
}

///////////////////////////////////////////////////////////////////////////

bool Objecthandler::checkCollision(const GameObject& object)
{
    for(const auto& object_it : m_game_objects)
    {
        //Cant collide with ourselves
        if(object == *(object_it.get()))
        {
            continue;
        }

        if(object.checkCollision(*(object_it.get())) == true)
        {
            return true;
        }
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////

void Objecthandler::getMapBoundaries(int &left, int &right, 
                                     int &top, int &bottom) const
{
    left    = 0;
    right   = m_active_map->getMapWidth();
    top     = 0;
    bottom  = m_active_map->getMapHeight();
}

///////////////////////////////////////////////////////////////////////////

void Objecthandler::getPlayerPosition(uint &x, uint &y) const
{
    x = getPlayerPositionX();
    y = getPlayerPositionY();
}

///////////////////////////////////////////////////////////////////////////

uint Objecthandler::getPlayerPositionX() const
{
    return m_player->getPositionX();
}

///////////////////////////////////////////////////////////////////////////

uint Objecthandler::getPlayerPositionY() const
{
    return m_player->getPositionY();
}

///////////////////////////////////////////////////////////////////////////
