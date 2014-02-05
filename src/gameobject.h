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

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <string>

#include "graphicsobject.h"
#include "inputevents.h"

using std::string;
using std::vector;

///////////////////////////////////////////////////////////////////////////

class GameObject
{
    DISABLECOPY(GameObject);

    public:
        GameObject(const string &id, const bool &collision = true);
        virtual ~GameObject();

        inline bool operator==(const GameObject &rhs) const
        {
            return m_id == rhs.getId();
        }

        inline const vector<shared_ptr <GraphicsObject> >& getGraphicsObjects() const
        {
            return m_graphics_objects;
        }

        inline bool hasCollisionEnabled() const
        {
            return m_collision;
        }

        inline const string getId() const
        {
            return m_id;
        }

        virtual void update();
        virtual void drawAll();
        virtual bool handleKeyEvent(const InputEvent &event);
        void addGraphicsObject(shared_ptr<GraphicsObject> obj);

        virtual bool checkCollision(const GameObject &other) const;

    protected:
        //TODO: create vector<int> m_draw_objects_at?
        vector<shared_ptr <GraphicsObject> > m_graphics_objects;

        string m_id;

        //TODO: move to graphics object?
        bool m_collision;
};

///////////////////////////////////////////////////////////////////////////

#endif
