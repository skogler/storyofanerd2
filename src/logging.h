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

#ifndef LOGGING_H
#define LOGGING_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "common.h"
#include "errorcodes.h"

///////////////////////////////////////////////////////////////////////////

enum LogLevel
{
    LOG_FATAL   = 0,
    LOG_ERROR   = 1,
    LOG_WARNING = 2,
    LOG_INFO    = 3,
    LOG_DEBUG   = 4,
    LOG_DEBUG2  = 5,
    LOG_STATE   = 6
};

enum LogCategory
{
    LOG_APP             = 1,
    LOG_CORE            = 2,
    LOG_OPENGL          = 4,
    LOG_SDL2_GRAPHICS   = 8,
    LOG_SDL2_AUDIO      = 16,
    LOG_MAP             = 32,
    LOG_PLAYER          = 64
};

///////////////////////////////////////////////////////////////////////////

class Logger
{
    public:
        Logger(enum LogLevel log_level = LOG_INFO,
               const string& logging_file_path = "");
        virtual ~Logger();

        void logMessage(enum LogLevel log_level, enum LogCategory log_category,
                        const char* format, ...);
        void addLoggingCategory(enum LogCategory category);

        enum LogLevel getLogLevel() const
        {
            return m_logging_level;
        }

        void setLogLevel(enum LogLevel log_level)
        {
            m_logging_level = log_level;
        }

        const string& getLoggingFilePath() const
        {
            return m_logging_file_path;
        }

    private:
        inline char* getCurrentTime() const
        {
            time_t rawtime;
            struct tm* timeinfo;

            char* buffer = (char*) malloc(sizeof(char) * 10);

            time(&rawtime);
            timeinfo = localtime(&rawtime);
            strftime(buffer, 10, "%T", timeinfo);
            return buffer;
        }

        ErrorCode openLoggingFile();
        void closeLoggingFile();

        enum LogLevel   m_logging_level;
        FILE*            m_logging_file;
        unsigned char   m_logging_categories;
        string          m_logging_file_path;

        DISABLECOPY(Logger);
};

///////////////////////////////////////////////////////////////////////////

#endif
