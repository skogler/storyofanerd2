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

#include <SDL2/SDL_image.h>

#include "player.h"
#include "objecthandler.h"

#define MOVING_STEP 5

///////////////////////////////////////////////////////////////////////////

Player::Player(const string& bmpfile, uint position_x, uint position_y) :
    GameObject("Player"),
    m_position_x(position_x),
    m_position_y(position_y),
    m_next_position_x(position_x),
    m_next_position_y(position_y)
{
    Logger.logMessage(LOG_STATE, LOG_PLAYER, "Player::Player start\n");

    shared_ptr<SDL_Texture> texture(GraphicsCore::instance().createTextureFromBMP(
                                        bmpfile),
                                    SDL_DestroyTexture);

    shared_ptr<GraphicsObject> playergraphics(new GraphicsObject(texture,
            m_position_x, m_position_y));

    this->addGraphicsObject(playergraphics);

    Logger.logMessage(LOG_STATE, LOG_PLAYER, "Player::Player end\n");
}

///////////////////////////////////////////////////////////////////////////

Player::Player(const string& sprite,
               uint sprite_x, uint sprite_y,
               uint clip_x, uint clip_y,
               uint position_x, uint position_y) :
    GameObject("Player"),
    m_position_x(position_x),
    m_position_y(position_y),
    m_next_position_x(position_x),
    m_next_position_y(position_y),
    m_sprite_x(sprite_x), m_sprite_y(sprite_y),
    m_clip_x(clip_x), m_clip_y(clip_y),
    m_sequencer(LOOK_RIGHT)
{
    //TODO:fix
    //SDL_Surface *sprite_surface = IMG_Load(("../res/maps/" + sprite.c_str());
    SDL_Surface* sprite_surface = IMG_Load("../res/jael.png");
    assert(sprite_surface);

    SDL_Texture* sprite_texture = SDL_CreateTextureFromSurface(&
                                  (GraphicsCore::instance().getRenderer()), sprite_surface);
    assert(sprite_texture);

    m_sprite_surface.reset(sprite_surface, SDL_FreeSurface);
    m_sprite_texture.reset(sprite_texture, SDL_DestroyTexture);

    createClipsFromSprite();
    createGraphicsObjects();
}

///////////////////////////////////////////////////////////////////////////

Player::~Player()
{
    Logger.logMessage(LOG_STATE, LOG_PLAYER, "Player::~Player\n");
}

///////////////////////////////////////////////////////////////////////////

void Player::update()
{
    Logger.logMessage(LOG_STATE, LOG_PLAYER, "Player::update start\n");

    m_sequencer.applyNextState();

    int undraw_index    = m_sequencer.getLastAnimationIndex();
    int draw_index      = m_sequencer.updateCurrentAnimationIndex();

    //First, disable the old one
    m_graphics_objects.at(undraw_index).get()->setEnabled(false);

    //Then activate the new one
    m_graphics_objects.at(draw_index).get()->setEnabled(true);
    m_graphics_objects.at(draw_index).get()->setX(m_next_position_x);
    m_graphics_objects.at(draw_index).get()->setY(m_next_position_y);

    bool has_collision = Scene.checkCollision(*this);

    if(has_collision == true)
    {
        Logger.logMessage(LOG_DEBUG2, LOG_PLAYER,
                          "Player::update: Collided, resetting x/y.\n");

        //Don't update, reset position
        m_graphics_objects.at(draw_index).get()->setX(m_position_x);
        m_graphics_objects.at(draw_index).get()->setY(m_position_y);

        m_next_position_x = m_position_x;
        m_next_position_y = m_position_y;
    }
    else
    {
        Logger.logMessage(LOG_DEBUG2, LOG_PLAYER,
                          "Player::update: Did not collide with anything!\n");
        m_position_x = m_next_position_x;
        m_position_y = m_next_position_y;
    }

    Logger.logMessage(LOG_STATE, LOG_PLAYER, "Player::update end\n");
}

///////////////////////////////////////////////////////////////////////////

bool Player::handleKeyEvent(const InputEvent& event)
{
    Logger.logMessage(LOG_STATE, LOG_PLAYER, "Player::handleKeyEvent start\n");

    assert(event);

    switch(event)
    {
        case PLAYER_RIGHT:
            m_next_position_x = m_next_position_x + MOVING_STEP;
            m_sequencer.setNextState(MOVE_RIGHT);
            return true;

        case PLAYER_LEFT:
            m_next_position_x = m_next_position_x - MOVING_STEP;
            m_sequencer.setNextState(MOVE_LEFT);
            return true;

        case PLAYER_DOWN:
            m_next_position_y = m_next_position_y + MOVING_STEP;
            m_sequencer.setNextState(MOVE_FRONT);
            return true;

        case PLAYER_UP:
            m_next_position_y = m_next_position_y - MOVING_STEP;
            m_sequencer.setNextState(MOVE_BACK);
            return true;

        default:
            break;
    }

    Logger.logMessage(LOG_STATE, LOG_PLAYER, "Player::handleKeyEvent end\n");
    return false;
}

///////////////////////////////////////////////////////////////////////////

void Player::addAnimationSequenceFromSprite(MovingState state,
        MovingStateSequence seq)
{
    m_sequencer.addSequence(state, seq);
}

///////////////////////////////////////////////////////////////////////////

void Player::createClipsFromSprite()
{
    uint number_clips_width     = m_sprite_x / m_clip_x;
    uint number_clips_height    = m_sprite_y / m_clip_y;

    for(uint i = 0; i < number_clips_width; i++)
    {
        for(uint j = 0; j < number_clips_height; j++)
        {
            shared_ptr<SDL_Rect> rect(new SDL_Rect());
            rect.get()->x  = i * m_clip_x + 1;
            rect.get()->y  = j * m_clip_y + 1;
            rect.get()->w  = m_clip_x;
            rect.get()->h  = m_clip_y;
            m_player_clips.push_back(rect);
        }
    }

    Logger.logMessage(LOG_DEBUG, LOG_PLAYER,
                      "Player::createClipsFromSprite: Created %d clips\n",
                      m_player_clips.size());
}

///////////////////////////////////////////////////////////////////////////

void Player::createGraphicsObjects()
{
    for(uint i = 0; i < m_player_clips.size(); i++)
    {
        shared_ptr<SDL_Rect> dst(new SDL_Rect());
        dst.get()->x = m_position_x;
        dst.get()->y = m_position_y;
        dst.get()->w = m_clip_x;
        dst.get()->h = m_clip_y;
        shared_ptr<GraphicsObject> playergraphics(new GraphicsObject(m_sprite_texture,
                m_player_clips.at(i), dst));
        playergraphics.get()->setEnabled(false);
        this->addGraphicsObject(playergraphics);
    }
}

///////////////////////////////////////////////////////////////////////////
