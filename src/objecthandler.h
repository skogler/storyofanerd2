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

#include "common.h"
#include "inputevents.h"

class GameObject;
class ClippedMap;
class Player;

using std::vector;

///////////////////////////////////////////////////////////////////////////

class Objecthandler
{
    DISABLECOPY(Objecthandler);

    public:
        static Objecthandler& instance()
        {
            static Objecthandler instance;
            return instance;
        }

        void reactToKeyEvent(InputEvent event);
        void updateAll();
        void drawAll();
        void addGameObject(shared_ptr<GameObject> object);
        bool checkCollision(const GameObject& object);

        inline void setActiveMapRef(shared_ptr<ClippedMap> active_map)
        {
            m_active_map = active_map;
        }

        inline void setPlayerRef(shared_ptr<Player> player)
        {
            m_player = player;
        }

        void getMapBoundaries(int &left, int &right, int &top, int &bottom) const;
        void getPlayerPosition(uint &x, uint &y) const;
        uint getPlayerPositionX() const;
        uint getPlayerPositionY() const;

    private:
        explicit Objecthandler();
        virtual ~Objecthandler();

        vector<shared_ptr <GameObject> > m_game_objects;
        shared_ptr<ClippedMap> m_active_map;
        shared_ptr<Player> m_player;
};

///////////////////////////////////////////////////////////////////////////

#define Scene Objecthandler::instance()

///////////////////////////////////////////////////////////////////////////

#endif
