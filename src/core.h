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

#ifndef CORE_H
#define CORE_H

#include <memory>

#include "common.h"
#include "errorcodes.h"

#include "logging.h"

using std::shared_ptr;

///////////////////////////////////////////////////////////////////////////

class GameCore
{
    public:
        static GameCore& instance()
        {
            static GameCore instance;
            return instance;
        }

        Logger& logger()
        {
            return *m_default_logger;
        }

    private:
        GameCore()
        {
            m_default_logger = new Logger(LOG_INFO);
            assert(m_default_logger);
        };
        ~GameCore()
        {
            delete m_default_logger;
        };

        Logger *m_default_logger;

        DISABLECOPY(GameCore);

};

///////////////////////////////////////////////////////////////////////////

#define Logger GameCore::instance().logger()

///////////////////////////////////////////////////////////////////////////

#endif
