#include <cmath>

#include "Run.h"
void Run::PressButtons()
{
    //If we're starting the turn around animation, keep pressing that way or else we'll get stuck in the slow turnaround
    if(m_state->m_memory->player_two_action == TURNING &&
        m_state->m_memory->player_two_action_frame == 1)
    {
        return;
    }

    if(!m_isWavedashing)
    {
        switch(m_state->m_memory->player_two_action)
        {
            case WALK_SLOW:
            case DASHING:
            {
                m_controller->tiltAnalog(Controller::BUTTON_MAIN, m_isRight ? 1 : 0, .5);
                break;
            }
            case WALK_MIDDLE:
            case WALK_FAST:
            {
                //Turning around is fine. We'll dash. But we can't run forward. We need to wavedash
                if(m_state->m_memory->player_two_facing != m_isRight)
                {
                    m_controller->tiltAnalog(Controller::BUTTON_MAIN, m_isRight ? 1 : 0, .5);
                    break;
                }
                else
                {
                    m_wavedashFrameStart = m_state->m_memory->frame;
                    m_isWavedashing = true;
                    break;
                }
                break;
            }
            case RUNNING:
            {
                //Are we running the wrong way? If so, wavedash back to stop the run
                if(m_state->m_memory->player_two_facing != m_isRight)
                {
                    m_wavedashFrameStart = m_state->m_memory->frame;
                    m_isWavedashing = true;
                    break;
                }
                else
                {
                    m_controller->tiltAnalog(Controller::BUTTON_MAIN, m_isRight ? 1 : 0, .5);
                }
                break;
            }
            default:
            {
                m_controller->tiltAnalog(Controller::BUTTON_MAIN, m_isRight ? 1 : 0, .5);
                break;
            }
        }
    }

    if(m_isWavedashing)
    {
        uint frame = m_state->m_memory->frame - m_wavedashFrameStart;
        switch(frame)
        {
            case 0:
            {
                //Jump
                m_controller->pressButton(Controller::BUTTON_Y);
                break;
            }
            case 1:
            {
                m_controller->releaseButton(Controller::BUTTON_Y);
                break;
            }
            case 3:
            {
                m_controller->pressButton(Controller::BUTTON_L);
                m_controller->tiltAnalog(Controller::BUTTON_MAIN, m_isRight ? .8 : .2, 0);
                break;
            }
            case 4:
            {
                m_controller->releaseButton(Controller::BUTTON_L);
                m_controller->tiltAnalog(Controller::BUTTON_MAIN, .5, .5);
                m_isWavedashing = false;
                m_wavedashFrameStart = 0;
                break;
            }
        }
    }
}

//We're always interruptible during a Walk
bool Run::IsInterruptible()
{
    if(m_isWavedashing == true)
    {
        return false;
    }
    return true;
}

Run::Run(bool isRight)
{
    m_isRight = isRight;
    m_isWavedashing = false;
    m_wavedashFrameStart = 0;
}

Run::~Run()
{
}
