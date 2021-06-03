/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "FrameGenerator.hpp"

using namespace camera_prophesee;

FrameGenerator::FrameGenerator(std::string const& name)
    : FrameGeneratorBase(name)
{
}

FrameGenerator::~FrameGenerator()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See FrameGenerator.hpp for more detailed
// documentation about them.

bool FrameGenerator::configureHook()
{
    if (! FrameGeneratorBase::configureHook())
        return false;

    /** Set the event frame member **/
    ::base::samples::frame::Frame *events_frame = new ::base::samples::frame::Frame();
    this->events_frame_msg.reset(events_frame);
    events_frame = nullptr;


    return true;
}
bool FrameGenerator::startHook()
{
    if (! FrameGeneratorBase::startHook())
        return false;
    return true;
}
void FrameGenerator::updateHook()
{
    FrameGeneratorBase::updateHook();

    /** Read the events **/
    ::base::samples::EventArray events;
    _events.read(events, false);

    //std::cout<<"events size: "<<events.events.size()<<std::endl;

    /** Create the Frame **/
    cv::Mat event_frame = this->createFrame(events);

    /** Convert from cv mat to frame **/
    ::base::samples::frame::Frame *events_frame_msg_ptr = this->events_frame_msg.write_access();
    events_frame_msg_ptr->image.clear();
    frame_helper::FrameHelper::copyMatToFrame(event_frame, *events_frame_msg_ptr);

    /** Write the frame in the output port **/
    events_frame_msg_ptr->time = events.time;
    events_frame_msg_ptr->received_time = events.time;
    this->events_frame_msg.reset(events_frame_msg_ptr);
    _events_frame.write(this->events_frame_msg);

}

void FrameGenerator::errorHook()
{
    FrameGeneratorBase::errorHook();
}

void FrameGenerator::stopHook()
{
    FrameGeneratorBase::stopHook();
}

void FrameGenerator::cleanupHook()
{
    FrameGeneratorBase::cleanupHook();
}

cv::Mat FrameGenerator::createFrame (::base::samples::EventArray &events_array)
{
    cv::Mat event_frame;
    cv::Vec3b color_positive, color_negative;

    if (_color_encoding.value() == camera_prophesee::BLUE_RED)
    {
        color_positive = cv::Vec3b(255.0, 0.0, 0.0); //BGR
        color_negative = cv::Vec3b(0.0, 0.0, 255.0);
        event_frame = cv::Mat(events_array.height, events_array.width, CV_8UC3, cv::Scalar(0,0,0));
    }
    else if (_color_encoding.value() == camera_prophesee::GREEN_RED)
    {
        color_positive = cv::Vec3b(0.0, 255.0, 0.0);
        color_negative = cv::Vec3b(0.0, 0.0, 255.0);
        event_frame = cv::Mat(events_array.height, events_array.width, CV_8UC3, cv::Scalar(255.,255.,255.));

    }
    else if (_color_encoding.value() == camera_prophesee::BLUE_BLACK)
    {
        color_positive = cv::Vec3b(255.0, 0.0, 0.0);
        color_negative = cv::Vec3b(0.0, 0.0, 0.0);
        event_frame = cv::Mat(events_array.height, events_array.width, CV_8UC3, cv::Scalar(255.0,255.0,255.0));
    }

    for (std::vector<::base::samples::Event>::const_iterator it = events_array.events.begin();
            it != events_array.events.end(); ++it)
    {
        if ((*it).polarity)
        {
            event_frame.at<cv::Vec3b>(cv::Point((*it).x,(*it).y)) = color_positive;
        }
        else
        {
            event_frame.at<cv::Vec3b>(cv::Point((*it).x,(*it).y)) = color_negative;
        }
    }


    return event_frame;
}