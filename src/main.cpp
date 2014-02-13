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

#include "common.h"
#include "core.h"
#include "graphics.h"
#include "player.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "xmlloader.h"
#include "clippedmap.h"
#include "objecthandler.h"
#include "inputhandler.h"

using std::dynamic_pointer_cast;

int main(int argc, char* argv[])
{
    GameCore& core = GameCore::instance();
    core.logger().setLogLevel(LOG_DEBUG);
    core.logger().addLoggingCategory(LOG_CORE);
    core.logger().addLoggingCategory(LOG_MAP);
    core.logger().addLoggingCategory(LOG_SDL2_GRAPHICS);
    core.logger().addLoggingCategory(LOG_PLAYER);

    LoadedMap lmap("../res/maps/testmapbig.tmx");
    ErrorCode file_loaded = lmap.loadFile();
    UNUSED(file_loaded);

    GraphicsCore& gcore = GraphicsCore::instance();
    gcore.initializeWindow();
    gcore.initializeRenderer();

    Objecthandler& handler = Objecthandler::instance();
    UNUSED(handler);

    Inputhandler& input = Inputhandler::instance();
    UNUSED(handler);

    shared_ptr<ClippedMap> clipped(new ClippedMap(&lmap));
    clipped.get()->copyTilesToRender(0, 0);
    clipped.get()->copyMapBorderToRender();

    //shared_ptr<Player> player(new Player("../res/player.bmp", 20, 300));
    shared_ptr<Player> player(new Player("../res/player.bmp", 128, 192, 32, 48, 80,
                                         300));

    handler.addGameObject(clipped);
    handler.addGameObject(player);

    MovingStateSequence seq_look_front;
    seq_look_front.possible_animations.push_back(0);
    seq_look_front.last_animation_index = 0;
    seq_look_front.revert_to = LOOK_FRONT;
    player->addAnimationSequenceFromSprite(LOOK_FRONT, seq_look_front);

    MovingStateSequence seq_look_left;
    seq_look_left.possible_animations.push_back(1);
    seq_look_left.last_animation_index = 0;
    seq_look_left.revert_to = LOOK_LEFT;
    player->addAnimationSequenceFromSprite(LOOK_LEFT, seq_look_left);

    MovingStateSequence seq_look_right;
    seq_look_right.possible_animations.push_back(2);
    seq_look_right.last_animation_index = 0;
    seq_look_right.revert_to = LOOK_RIGHT;
    player->addAnimationSequenceFromSprite(LOOK_RIGHT, seq_look_right);

    MovingStateSequence seq_look_back;
    seq_look_back.possible_animations.push_back(3);
    seq_look_back.last_animation_index = 0;
    seq_look_back.revert_to = LOOK_BACK;
    player->addAnimationSequenceFromSprite(LOOK_BACK, seq_look_back);

    MovingStateSequence seq_move_front;
    seq_move_front.possible_animations.push_back(4);
    seq_move_front.possible_animations.push_back(8);
    seq_move_front.possible_animations.push_back(12);
    seq_move_front.last_animation_index = 0;
    seq_move_front.revert_to = LOOK_FRONT;
    player->addAnimationSequenceFromSprite(MOVE_FRONT, seq_move_front);

    MovingStateSequence seq_move_left;
    seq_move_left.possible_animations.push_back(5);
    seq_move_left.possible_animations.push_back(9);
    seq_move_left.possible_animations.push_back(13);
    seq_move_left.last_animation_index = 0;
    seq_move_left.revert_to = LOOK_LEFT;
    player->addAnimationSequenceFromSprite(MOVE_LEFT, seq_move_left);

    MovingStateSequence seq_move_right;
    seq_move_right.possible_animations.push_back(6);
    seq_move_right.possible_animations.push_back(10);
    seq_move_right.possible_animations.push_back(14);
    seq_move_right.last_animation_index = 0;
    seq_move_right.revert_to = LOOK_RIGHT;
    player->addAnimationSequenceFromSprite(MOVE_RIGHT, seq_move_right);

    MovingStateSequence seq_move_back;
    seq_move_back.possible_animations.push_back(7);
    seq_move_back.possible_animations.push_back(11);
    seq_move_back.possible_animations.push_back(15);
    seq_move_back.last_animation_index = 0;
    seq_move_back.revert_to = LOOK_BACK;
    player->addAnimationSequenceFromSprite(MOVE_BACK, seq_move_back);

    bool quit = false;

    while(quit == false)
    {
        gcore.clearRenderer();
        SDL_PumpEvents();
        InputEvent event = input.getNextEvent();

        while(event != NONE)
        {
            if(event == QUIT)
            {
                goto stop;
            }

            handler.reactToKeyEvent(event);
            event = input.getNextEvent();
        }

        handler.updateAll();
        gcore.updateViewportRelativeTo(player->getPositionX(), player->getPositionY());
        handler.drawAll();
        gcore.presentRenderer();

        SDL_Delay(42);
    }

stop:
    return 0;
}
