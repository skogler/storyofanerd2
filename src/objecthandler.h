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

#ifndef OBJECTHANDLER_H
#define OBJECTHANDLER_H

#include <SDL2/SDL.h>
#include <vector>

#include "core.h"
#include "common.h"
#include "gameobject.h"

using std::vector;

///////////////////////////////////////////////////////////////////////////

class Objecthandler
{
    public:
        static Objecthandler& instance()
        {
            static Objecthandler instance;
            return instance;
        }

        void reactToKeyEvent(InputEvent event)
        {
            bool handled = false;
            for(uint i = 0; i < m_game_objects.size(); i++)
            {
                handled = m_game_objects.at(i).get()->handleKeyEvent(event);
                if(handled == true)
                {
                    break;
                }
            }
        }

        void updateAll()
        {
            for(uint i = 0; i < m_game_objects.size(); i++)
            {
                m_game_objects.at(i).get()->update();
            }
        }

        void drawAll()
        {
            for(uint i = 0; i < m_game_objects.size(); i++)
            {
                m_game_objects.at(i).get()->drawAll();
            }
        }

        void addGameObject(shared_ptr<GameObject> object)
        {
            m_game_objects.push_back(object);
        }

        bool checkCollision(const GameObject &object)
        {
            for(uint i = 0; i < m_game_objects.size(); i++)
            {
                //Cant collide with ourselves
                if(object == *(m_game_objects.at(i).get()))
                {
                    return false;
                }

                if(object.checkCollision(*(m_game_objects.at(i).get())) == true)
                {
                    return true;
                }
            }

            return false;
        }

    private:
        Objecthandler()
        {
        };
        virtual ~Objecthandler()
        {
        };

        vector<shared_ptr <GameObject> > m_game_objects;

        DISABLECOPY(Objecthandler);
};

///////////////////////////////////////////////////////////////////////////

#define Scene Objecthandler::instance()

#endif
