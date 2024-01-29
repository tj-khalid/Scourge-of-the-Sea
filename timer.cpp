#include <GLFW/glfw3.h>
#include <iostream>

#include "timer.h"

namespace game {

Timer::Timer(void)
{
    start_time_ = 0;
    end_time_ = -1;
    started_ = false;
}


Timer::~Timer(void)
{
}


void Timer::Start(float end_time)
{
    start_time_ = glfwGetTime();
    end_time_ = end_time;
    started_ = true;
}


bool Timer::Finished(void) const
{
    if (started_) {
        if (glfwGetTime()-start_time_ > end_time_) {
            return true;
        }
    }
    
    return false;
}

} // namespace game
