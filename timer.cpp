#include <GLFW/glfw3.h>
#include <iostream>

#include "timer.h"

namespace game {

Timer::Timer(void)
{
    start_time_ = 0;
    end_time_ = -1;
    running_ = false;
}


Timer::~Timer(void)
{
}


void Timer::Start(float end_time)
{
    start_time_ = glfwGetTime();
    end_time_ = end_time;
    running_ = true;
}

bool Timer::Running(void) const {
    return running_;
}

bool Timer::Finished(void)
{
    if (running_) {
        if (glfwGetTime()-start_time_ > end_time_) {
            running_ = false;
            return true;
        }
    }
    
    return false;
}

} // namespace game
