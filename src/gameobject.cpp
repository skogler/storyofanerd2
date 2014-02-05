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

#include "gameobject.h"

///////////////////////////////////////////////////////////////////////////

GameObject::GameObject(const string &id,
                       const bool &collision) :
    m_id(id),
    m_collision(collision)
{
}

///////////////////////////////////////////////////////////////////////////

void GameObject::update()
{
    //TODO
}

///////////////////////////////////////////////////////////////////////////

void GameObject::drawAll()
{
    for(uint i = 0; i < m_graphics_objects.size(); i++)
    {
        m_graphics_objects.at(i).get()->drawObject();
    }
}

///////////////////////////////////////////////////////////////////////////

void GameObject::addGraphicsObject(shared_ptr<GraphicsObject> obj)
{
    m_graphics_objects.push_back(obj);
}

///////////////////////////////////////////////////////////////////////////

bool GameObject::handleKeyEvent(const InputEvent &event)
{
    return false;
}

///////////////////////////////////////////////////////////////////////////

GameObject::~GameObject()
{
    //TODO
}

///////////////////////////////////////////////////////////////////////////

bool GameObject::checkCollision(const GameObject &other) const
{
    Logger.logMessage(LOG_STATE, LOG_CORE, "GameObject::checkCollision: start\n");

    if(this->hasCollisionEnabled() == false ||
       other.hasCollisionEnabled() == false)
    {
        Logger.logMessage(LOG_DEBUG2, LOG_CORE, "GameObject::checkCollision: Collision detection not enabled for both elements\n");
        return false;
    }

    const vector<shared_ptr <GraphicsObject> > other_objects = other.getGraphicsObjects();
    for(uint i = 0; i < m_graphics_objects.size(); i++)
    {
        for(uint j = 0; j < other_objects.size(); j++)
        {
            if(m_graphics_objects.at(i).get()->hasCollision(*(other_objects.at(j).get())))
            {
                return true;
            }
        }
    }

    Logger.logMessage(LOG_STATE, LOG_CORE, "GameObject::checkCollision: end\n");
    return false;
}

///////////////////////////////////////////////////////////////////////////
