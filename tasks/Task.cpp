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

    try
    {
        /** Search for the camera **/
        this->camera = Prophesee::Camera::from_first_available();

        /** Configure the camera **/
        if (!_biases_file.value().empty())
        {
            this->camera.biases().set_from_file(_biases_file.value());
        }
        this->camera_is_opened = true;
        RTT::log(RTT::Info)<<"[CONFIGURE HOOK]: CAMERA OPENED"<<RTT::endlog();
    }
    catch(Prophesee::CameraException &e)
    {
        RTT::log(RTT::Info)<<"[CONFIGURE HOOK]: ERROR IN OPENING THE CAMERA "<<e.what()<<RTT::endlog();
    }

    if (this->camera_is_opened == true)
    {
        /** Set the maximum event rate, in kEv/s
        * Decreasing the max events rate will decrease the latency and also decrease the number of published events
        * Increasing the max events rate will increase the number of published events and also increase the latency **/
        if (!this->camera.set_max_event_rate_limit(_max_event_rate.value()))
        {
            RTT::log(RTT::Info)<<"[CONFIGURE HOOK]: FAILED TO SET THE MAXIMUM RATE OF EVENTS "<<RTT::endlog();
        }

        /** Get the sensor config **/
        Prophesee::CameraConfiguration config = this->camera.get_camera_configuration();
        auto &geometry = this->camera.geometry();
        RTT::log(RTT::Info)<<"[CONF] Width: "<< geometry.width()<<", Height: "<< geometry.height()<<RTT::endlog();
        RTT::log(RTT::Info)<<"[CONF] Max event rate, in kEv/s:"<< config.max_drop_rate_limit_kEv_s<<RTT::endlog();
        RTT::log(RTT::Info)<<"[CONF] Serial number: "<< config.serial_number.c_str()<<RTT::endlog();
    }

    return this->camera_is_opened;
}

bool Task::startHook()
{
    if (! TaskBase::startHook())
        return false;

    /** Start the camera **/
    this->camera.start();

    /** Add CD events callback **/

    /** Add EM events callback **/

    return true;
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

    /** Stop the camera **/
    this->camera.stop();
}
void Task::cleanupHook()
{
    TaskBase::cleanupHook();

    /** Close the camera **/
    this->camera_is_opened = false;
}
