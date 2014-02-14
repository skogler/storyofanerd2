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

#include "states.h"

///////////////////////////////////////////////////////////////////////////

MovingStateSequencer::MovingStateSequencer(MovingState initial_position,
                                          uint update_rate) :
    m_previous_state(initial_position),
    m_current_state(initial_position),
    m_next_state(initial_position),
    m_update_rate(update_rate),
    m_update_rate_counter(0),
    m_draw_next_animation(false)
{
}

///////////////////////////////////////////////////////////////////////////

MovingStateSequencer::~MovingStateSequencer()
{
}

///////////////////////////////////////////////////////////////////////////

void MovingStateSequencer::addSequence(MovingState state,
                                       MovingStateSequence sequence)
{
    m_sequences.insert(std::pair<MovingState, MovingStateSequence>(state,
                sequence));
}

///////////////////////////////////////////////////////////////////////////

int MovingStateSequencer::getLastAnimationIndex() const
{
    return m_sequences.at(m_previous_state).possible_animations.at(m_sequences.at(
                m_previous_state).last_animation_index);
}

///////////////////////////////////////////////////////////////////////////

bool MovingStateSequencer::doCheckUpdate()
{
    m_update_rate_counter = m_update_rate_counter + 1;

    if(m_update_rate_counter == m_update_rate)
    {
        m_update_rate_counter   = 0;
        m_draw_next_animation   = true;
        return true;
    }

    m_draw_next_animation = false;
    return false;
}

///////////////////////////////////////////////////////////////////////////

void MovingStateSequencer::applyNextState()
{
    doCheckUpdate();
    if(m_draw_next_animation == false)
    {
        return;
    }

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

///////////////////////////////////////////////////////////////////////////

int MovingStateSequencer::updateCurrentAnimationIndex()
{
    auto state_it = m_sequences.find(m_current_state);

    if(m_draw_next_animation == false)
    {
        return state_it->second.possible_animations.at(state_it->second.last_animation_index);
    }

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

///////////////////////////////////////////////////////////////////////////

