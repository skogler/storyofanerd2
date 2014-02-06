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
    GameCore &core = GameCore::instance();
    core.logger().setLogLevel(LOG_DEBUG2);
    core.logger().addLoggingCategory(LOG_CORE);
    core.logger().addLoggingCategory(LOG_MAP);
    core.logger().addLoggingCategory(LOG_SDL2_GRAPHICS);
    core.logger().addLoggingCategory(LOG_PLAYER);

    LoadedMap lmap("../res/maps/testmap.tmx");
    ErrorCode file_loaded = lmap.loadFile();
    UNUSED(file_loaded);

    GraphicsCore &gcore = GraphicsCore::instance();
    gcore.initializeWindow();
    gcore.initializeRenderer();

    Objecthandler &handler = Objecthandler::instance();
    UNUSED(handler);

    Inputhandler &input = Inputhandler::instance();
    UNUSED(handler);

    shared_ptr<ClippedMap> clipped(new ClippedMap(&lmap));
    clipped.get()->copyTilesToRender(0, -100);

    shared_ptr<Player> player(new Player("../res/player.bmp", 20, 300));

    handler.addGameObject(clipped);
    handler.addGameObject(player);

    bool quit = false;
	const int MAX_FPS = 60;
	const int MAX_DELTA_MS = 1.0f / MAX_FPS * 1000;
	unsigned int startTicks = SDL_GetTicks();
	unsigned int deltaMs = MAX_DELTA_MS;
	unsigned int endTicks = SDL_GetTicks();

    while(quit == false)
    {
		startTicks = SDL_GetTicks();
        gcore.clearRenderer();
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
        handler.drawAll();
        gcore.presentRenderer();


		endTicks = SDL_GetTicks();
		deltaMs = endTicks - startTicks;
		if (deltaMs < MAX_DELTA_MS) {
			SDL_Delay(MAX_DELTA_MS - deltaMs);
			deltaMs = MAX_DELTA_MS;
		}
		Logger.logMessage(LOG_WARNING,LOG_CORE, "DELTA: %u\n", deltaMs);
    }

stop:
    return 0;
}
