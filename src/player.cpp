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

#include "player.h"

///////////////////////////////////////////////////////////////////////////

Player::Player(const string &bmpfile, uint position_x, uint position_y) :
    GameObject("Player"),
    m_position_x(position_x),
    m_position_y(position_y)
{
    shared_ptr<SDL_Texture> texture(GraphicsCore::instance().createTextureFromBMP(bmpfile),
                                    SDL_DestroyTexture);

    shared_ptr<GraphicsObject> playergraphics(new GraphicsObject(texture, m_position_x, m_position_y));

    this->addGraphicsObject(playergraphics);
}

///////////////////////////////////////////////////////////////////////////

Player::~Player()
{
}

///////////////////////////////////////////////////////////////////////////

void Player::update()
{
    //Logger.logMessage(LOG_STATE, LOG_PLAYER, "Player::update start\n");
    m_graphics_objects.at(0).get()->setX(m_position_x);
    m_graphics_objects.at(0).get()->setY(m_position_y);
    //Logger.logMessage(LOG_STATE, LOG_PLAYER, "Player::update end\n");
}

///////////////////////////////////////////////////////////////////////////

bool Player::handleKeyEvent(const InputEvent &event)
{
    assert(event);

    //Logger.logMessage(LOG_STATE, LOG_PLAYER, "Player::handleKeyEvent start\n");

    switch(event)
    {
        case PLAYER_RIGHT:
            m_position_x = m_position_x + 10;
            return true;
        case PLAYER_LEFT:
            m_position_x = m_position_x - 10;
            return true;
        case PLAYER_DOWN:
            m_position_y = m_position_y + 10;
            return true;
        case PLAYER_UP:
            m_position_y = m_position_y - 10;
            return true;
        default:
            break;
    }

    //Logger.logMessage(LOG_STATE, LOG_PLAYER, "Player::handleKeyEvent end\n");
    return false;
}
