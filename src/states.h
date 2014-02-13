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

#ifndef STATES_H
#define STATES_H

#include <map>

using std::map;

///////////////////////////////////////////////////////////////////////////

enum MovingState
{
    INVALID         = 0,
    LOOK_FRONT      = 1,
    LOOK_LEFT       = 2,
    LOOK_RIGHT      = 3,
    LOOK_BACK       = 4,
    MOVE_FRONT      = 5,
    MOVE_LEFT       = 6,
    MOVE_RIGHT      = 7,
    MOVE_BACK       = 8
};

///////////////////////////////////////////////////////////////////////////

typedef struct MovingStateSequence MovingStateSequence;
struct MovingStateSequence
{
    vector<unsigned int>    possible_animations;
    unsigned int            last_animation_index;
    MovingState             revert_to;
};

///////////////////////////////////////////////////////////////////////////

class MovingStateSequencer
{
        DISABLECOPY(MovingStateSequencer);

    public:
        explicit MovingStateSequencer(MovingState initial_position = INVALID) :
            m_previous_state(initial_position),
            m_current_state(initial_position),
            m_next_state(initial_position)
        {
        }

        virtual ~MovingStateSequencer()
        {
        };

        inline void addSequence(MovingState state, MovingStateSequence sequence)
        {
            m_sequences.insert(std::pair<MovingState, MovingStateSequence>(state,
                               sequence));
        }

        inline const MovingStateSequence& getMovingStateSequence(
            MovingState state) const
        {
            return m_sequences.at(state);
        }

        inline const MovingState& getPreviousState() const
        {
            return m_previous_state;
        }

        inline const MovingState& getCurrentState() const
        {
            return m_current_state;
        }

        inline const MovingState& getNextState() const
        {
            return m_next_state;
        }

        inline void setNextState(MovingState next)
        {
            m_next_state = next;
        }

        inline int getLastAnimationIndex() const
        {
            return m_sequences.at(m_previous_state).possible_animations.at(m_sequences.at(
                        m_previous_state).last_animation_index);
        }

        inline void applyNextState()
        {
            m_previous_state    = m_current_state;

            if(m_next_state == INVALID)
            {
                m_current_state = m_sequences.find(m_previous_state)->second.revert_to;
            }
            else
            {
                m_current_state     = m_next_state;
                m_next_state        = INVALID;
            }
        }

        inline int updateCurrentAnimationIndex()
        {
            auto state_it = m_sequences.find(m_current_state);

            if(state_it->second.last_animation_index + 1 ==
               state_it->second.possible_animations.size())
            {
                state_it->second.last_animation_index = 0;
                return state_it->second.possible_animations.at(0);
            }
            else
            {
                state_it->second.last_animation_index = state_it->second.last_animation_index +
                                                        1;
                return state_it->second.possible_animations.at(
                           state_it->second.last_animation_index);
            }
        }

    private:
        map<MovingState, MovingStateSequence> m_sequences;

        MovingState     m_previous_state;
        MovingState     m_current_state;
        MovingState     m_next_state;
};

///////////////////////////////////////////////////////////////////////////

#endif
