/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Task.hpp"

using namespace camera_prophesee;

Task::Task(std::string const& name)
    : TaskBase(name)
{
    this->camera_is_opened = false;
}

Task::~Task()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Task.hpp for more detailed
// documentation about them.

bool Task::configureHook()
{
    if (! TaskBase::configureHook())
        return false;
    return true;
}
bool Task::startHook()
{
    if (! TaskBase::startHook())
        return false;

    /** Open the camera **/
    this->camera = Prophesee::Camera::from_first_available();
    if (!_biases_file.value().empty())
    {
        this->camera.biases().set_from_file(_biases_file.value());
    }
    this->camera_is_opened = true;

    return this->camera_is_opened;
}
void Task::updateHook()
{
    TaskBase::updateHook();
}
void Task::errorHook()
{
    TaskBase::errorHook();
}
void Task::stopHook()
{
    TaskBase::stopHook();
    this->camera.stop();
    this->camera_is_opened = false;
}
void Task::cleanupHook()
{
    TaskBase::cleanupHook();
}
