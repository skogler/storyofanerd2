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

#include <assert.h>

#include "logging.h"

///////////////////////////////////////////////////////////////////////////

Logger::Logger(enum LogLevel log_level, const string &logging_file_path) :
               m_logging_level(log_level), m_logging_file(NULL),
               m_logging_file_path(logging_file_path)
{
    addLoggingCategory(LOG_APP);
}

///////////////////////////////////////////////////////////////////////////

Logger::~Logger()
{
    closeLoggingFile();
}

///////////////////////////////////////////////////////////////////////////

ErrorCode Logger::openLoggingFile()
{
    if(m_logging_file_path != "")
    {
        m_logging_file = fopen(m_logging_file_path.c_str(), "wt");
        assert(m_logging_file);
        return OK;
    }
    return ERROR_OPENING_FILE;
}

///////////////////////////////////////////////////////////////////////////

void Logger::closeLoggingFile()
{
    if(m_logging_file != NULL)
    {
        fclose(m_logging_file);
        m_logging_file = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////

void Logger::logMessage(enum LogLevel log_level, enum LogCategory log_category,
                        const char *format, ...)
{
    if(log_level <= m_logging_level && 
      (m_logging_categories & log_category) == log_category)
    {
        char *current_time = getCurrentTime();
        va_list args;
        va_start(args, format);

        switch(log_level)
        {
            case LOG_FATAL:
                fprintf(stderr, "%s ", current_time);
                vfprintf(stderr, format, args);
                break;
            case LOG_ERROR:
                fprintf(stderr, "%s ", current_time);
                vfprintf(stderr, format, args);
                break;
            case LOG_WARNING:
                fprintf(stdout, "%s ", current_time);
                vfprintf(stdout, format, args);
                break;
            case LOG_INFO:
                fprintf(stdout, "%s ", current_time);
                vfprintf(stdout, format, args);
                break;
            case LOG_DEBUG:
                fprintf(stdout, "%s ", current_time);
                vfprintf(stdout, format, args);
                break;
            case LOG_DEBUG2:
                fprintf(stdout, "%s ", current_time);
                vfprintf(stdout, format, args);
                break;
            case LOG_STATE:
                fprintf(stdout, "%s ", current_time);
                vfprintf(stdout, format, args);
                break;
            default:
                break;
        }
        va_end(args);

        if(m_logging_file != NULL)
        {
            va_start(args, format);
            fprintf(m_logging_file, "%s ", current_time);
            vfprintf(m_logging_file, format, args);
            va_end(args);
        }

        free(current_time);
    }

    if(log_level == LOG_FATAL)
    {
        exit(EXIT_FAILURE);
    }
}

///////////////////////////////////////////////////////////////////////////

void Logger::addLoggingCategory(enum LogCategory category)
{
    m_logging_categories |= category;
}

