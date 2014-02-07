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

#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

#include <map>

#include "common.h"
#include "core.h"
#include "gameobject.h"
#include "states.h"

using std::map;

///////////////////////////////////////////////////////////////////////////

//TODO: Inheritance Hierachy: Gameobject --> Unit --> Player
class Player : public GameObject
{
    public:
        explicit Player(const string &bmpfile,
                        uint position_x = 0,
                        uint position_y = 0);
        explicit Player(const string &sprite,
                        uint sprite_x, uint sprite_y,
                        uint clip_x, uint clip_y,
                        uint position_x = 0, uint position_y = 0);
        virtual ~Player();

        virtual void update();
        virtual bool handleKeyEvent(const InputEvent &event);

        virtual void addAnimationSequenceFromSprite(MovingState state, MovingStateSequence seq);

    private:
        void createClipsFromSprite();
        void createGraphicsObjects();

        int findMatchingAnimationGraphicsObject();

        uint m_position_x;
        uint m_position_y;

        uint m_next_position_x;
        uint m_next_position_y;

        uint m_sprite_x;
        uint m_sprite_y;

        uint m_clip_x;
        uint m_clip_y;

        vector<shared_ptr<SDL_Rect> > m_player_clips;
        shared_ptr<SDL_Surface> m_sprite_surface;
        shared_ptr<SDL_Texture> m_sprite_texture;

        MovingState             m_previous_state;
        MovingState             m_current_state;

        int                     m_previous_animation_index;

        map<MovingState, MovingStateSequence>   m_moving_animations;

        DISABLECOPY(Player);
};

///////////////////////////////////////////////////////////////////////////

#endif
