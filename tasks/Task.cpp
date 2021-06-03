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
            std::cout<<"bias file: "<<_biases_file.value()<<std::endl;
            this->camera.biases().set_from_file(_biases_file.value());
        }
        this->camera_is_opened = true;
        RTT::log(RTT::Info)<<"[PROPHESEE CONFIGURE HOOK]: CAMERA OPENED"<<RTT::endlog();
    }
    catch(Prophesee::CameraException &e)
    {
        this->camera_is_opened = false;
        RTT::log(RTT::Info)<<"[PROPHESEE CONFIGURE HOOK]: ERROR IN OPENING THE CAMERA "<<e.what()<<RTT::endlog();
    }

    if (this->camera_is_opened == true)
    {
        /** Set the maximum event rate, in kEv/s
        * Decreasing the max events rate will decrease the latency and also decrease the number of published events
        * Increasing the max events rate will increase the number of published events and also increase the latency **/
        if (!this->camera.set_max_event_rate_limit(_max_event_rate.value()))
        {
            RTT::log(RTT::Info)<<"[PROPHESEE CONFIGURE]: FAILED TO SET THE MAXIMUM RATE OF EVENTS "<<RTT::endlog();
        }

        /** Get the sensor config **/
        Prophesee::CameraConfiguration config = this->camera.get_camera_configuration();
        auto &geometry = this->camera.geometry();
        RTT::log(RTT::Info)<<"[PROPHESEE] Width: "<< geometry.width()<<", Height: "<< geometry.height()<<RTT::endlog();
        RTT::log(RTT::Info)<<"[PROPHESEE] Max event rate, in kEv/s:"<< config.max_drop_rate_limit_kEv_s<<RTT::endlog();
        RTT::log(RTT::Info)<<"[PROPHESEE] Serial number: "<< config.serial_number.c_str()<<RTT::endlog();

        /** Sensor geometry in header of the message **/
        event_msg.height = geometry.height();
        event_msg.width = geometry.width();

        /** Read the current stream rate of events **/
        if (this->_event_streaming_rate > 0)
        {
            this->event_delta_t = ::base::Time::fromSeconds(1.0/ this->_event_streaming_rate);
        }
        else
        {
            this->event_delta_t = ::base::Time::fromMicroseconds(EVENT_DEFAULT_DELTA_T_MICROSECONDS);
            this->_event_streaming_rate.set(1.0 / this->event_delta_t.toSeconds());
        }

        // Add runtime error callback
        camera.add_runtime_error_callback([](const Prophesee::CameraException &e){
            RTT::log(RTT::Info)<< e.what() << RTT::endlog();
        });

        if (this->_activity_filter_temporal_depth > 0)
        {
            this->activity_filter.reset(new Prophesee::ActivityNoiseFilterAlgorithm<>(camera.geometry().width(), camera.geometry().height(), this->_activity_filter_temporal_depth.value()));
        }

    }

    return this->camera_is_opened;
}

bool Task::startHook()
{
    if (! TaskBase::startHook())
        return false;

    /** Start the camera **/
    this->camera.start();
    this->start_timestamp = ::base::Time::now();


    /** Add CD events callback **/
    this->eventsCallBack();

    /** Add IMU callback **/
    this->imuCallBack();

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
    RTT::log(RTT::Info)<<"[PROPHESEE] CAMERA STOPPED "<<RTT::endlog();
}
void Task::cleanupHook()
{
    TaskBase::cleanupHook();

    /** Close the camera **/
    this->camera_is_opened = false;

    /** Reset the activity filter **/
    this->activity_filter.reset();
    RTT::log(RTT::Info)<<"[PROPHESEE] CAMERA CLOSED "<<RTT::endlog();
}

void Task::eventsCallBack()
{
    // Initialize and publish a buffer of CD events
    try {
        this->camera.cd().add_callback([this](const Prophesee::EventCD *ev_begin, const Prophesee::EventCD *ev_end)
        {
            if (ev_begin < ev_end)
            {
                // Get the current time
                event_buffer_current_time = this->start_timestamp + ::base::Time::fromMicroseconds(ev_begin->t);

                /** In case the buffer is empty we set the starting time stamp **/
                if (event_buffer.empty())
                {
                    // Get starting time
                    event_buffer_start_time = event_buffer_current_time;
                }

                /** Insert the events to the buffer **/
                auto inserter = std::back_inserter(event_buffer);

                if (this->_activity_filter_temporal_depth > 0)
                {
                    /** When there is activity filter **/
                    this->activity_filter->process_output(ev_begin, ev_end, inserter);
                }
                else
                {
                    /** When there is not activity filter **/
                    std::copy(ev_begin, ev_end, inserter);
                }

                /** Get the last time stamp **/
                event_buffer_current_time = this->start_timestamp + ::base::Time::fromMicroseconds((ev_end-1)->t);
            }

            if ((event_buffer_current_time - event_buffer_start_time) >= this->event_delta_t)
            {
                /** Set the buffer size for the msg **/
                event_msg.events.resize(event_buffer.size());

                /** Set the array time **/
                event_msg.time = event_buffer_current_time;

                // Copy the events to the ros buffer format
                auto array_it = event_msg.events.begin();
                for (const Prophesee::EventCD *it = std::addressof(event_buffer[0]); it != std::addressof(event_buffer[event_buffer.size()]); ++it, ++array_it)
                {
                    ::base::samples::Event &event = *array_it;
                    event.x = it->x;
                    event.y = it->y;
                    event.polarity = it->p;
                    event.ts = this->start_timestamp + ::base::Time::fromMicroseconds(it->t);
                }

                // Clean the buffer for the next itteration
                event_buffer.clear();


                // Publish the message
                this->_events.write(event_msg);
            }
        });

        }
        catch (Prophesee::CameraException &e)
        {
            RTT::log(RTT::Info)<<"[PROPHESEE]"<<e.what()<<RTT::endlog();
        }
}


void Task::imuCallBack()
{
    this->camera.imu_sensor().enable();
    Prophesee::CallbackId imu_id(std::numeric_limits<Prophesee::CallbackId>::max());
    imu_id = this->camera.imu().add_callback([this](const Prophesee::EventIMU *begin, const Prophesee::EventIMU *end)
    {
        for (auto it = begin; it != end; ++it)
        {
            ::base::samples::IMUSensors imusamples;
            imusamples.time = this->start_timestamp + ::base::Time::fromMicroseconds(it->t * 1.0);
            imusamples.acc << GRAVITY * it->ax, GRAVITY * it->ay, GRAVITY * it->az; //[m/s^2]
            imusamples.gyro << it->gx, it->gy, it->gz; //[rad/s]
            this->_imu.write(imusamples);
        }
    });
}

