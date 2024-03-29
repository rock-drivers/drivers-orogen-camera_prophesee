/* Generated from orogen/lib/orogen/templates/tasks/Task.hpp */

#ifndef CAMERA_PROPHESEE_TASK_TASK_HPP
#define CAMERA_PROPHESEE_TASK_TASK_HPP

/** Prophesee system driver **/
#include <prophesee_driver/prophesee_driver.h>
#include <prophesee_core/algos/core/activity_noise_filter_algorithm.h>

/** Base types **/
#include <base/samples/IMUSensors.hpp>
#include <base/samples/EventArray.hpp>

/** Task Base **/
#include "camera_prophesee/TaskBase.hpp"

namespace camera_prophesee{

    /*! \class Task
     * \brief The task context provides and requires services. It uses an ExecutionEngine to perform its functions.
     * Essential interfaces are operations, data flow ports and properties. These interfaces have been defined using the oroGen specification.
     * In order to modify the interfaces you should (re)use oroGen and rely on the associated workflow.
     * Maximum number of events per second
     * \details
     * The name of a TaskContext is primarily defined via:
     \verbatim
     deployment 'deployment_name'
         task('custom_task_name','camera_prophesee::Task')
     end
     \endverbatim
     *  It can be dynamically adapted when the deployment is called with a prefix argument.
     */
    class Task : public TaskBase
    {
    friend class TaskBase;
    protected:
        /** Control variables **/
        bool camera_is_opened;

        /** Mean gravity value at Earth surface [m/s^2] **/
        static constexpr float GRAVITY = 9.81;

        /** Time of cd events fixed by Prophesee driver
        The delta time is set to a fixed number of 64 microseconds (1e-06)
        **/
        static constexpr int64_t EVENT_DEFAULT_DELTA_T_MICROSECONDS = 64;

        /** Driver variables **/
        Prophesee::Camera camera; /** The ATIS Prophesee camera **/
        std::vector<Prophesee::EventCD> event_buffer;

        /** Activity Filter Instance **/
        std::shared_ptr<Prophesee::ActivityNoiseFilterAlgorithm<>> activity_filter;


        /**  Event buffer time stamps **/
        ::base::Time start_timestamp, event_delta_t;
        ::base::Time event_buffer_start_time, event_buffer_current_time;


        /** Output port variables **/
        ::base::samples::EventArray event_msg;

    public:
        /** TaskContext constructor for Task
         * \param name Name of the task. This name needs to be unique to make it identifiable via nameservices.
         * \param initial_state The initial TaskState of the TaskContext. Default is Stopped state.
         */
        Task(std::string const& name = "camera_prophesee::Task");

        /** Default deconstructor of Task
         */
        ~Task();

        /** This hook is called by Orocos when the state machine transitions
         * from PreOperational to Stopped. If it returns false, then the
         * component will stay in PreOperational. Otherwise, it goes into
         * Stopped.
         *
         * It is meaningful only if the #needs_configuration has been specified
         * in the task context definition with (for example):
         \verbatim
         task_context "TaskName" do
           needs_configuration
           ...
         end
         \endverbatim
         */
        bool configureHook();

        /** This hook is called by Orocos when the state machine transitions
         * from Stopped to Running. If it returns false, then the component will
         * stay in Stopped. Otherwise, it goes into Running and updateHook()
         * will be called.
         */
        bool startHook();

        /** This hook is called by Orocos when the component is in the Running
         * state, at each activity step. Here, the activity gives the "ticks"
         * when the hook should be called.
         *
         * The error(), exception() and fatal() calls, when called in this hook,
         * allow to get into the associated RunTimeError, Exception and
         * FatalError states.
         *
         * In the first case, updateHook() is still called, and recover() allows
         * you to go back into the Running state.  In the second case, the
         * errorHook() will be called instead of updateHook(). In Exception, the
         * component is stopped and recover() needs to be called before starting
         * it again. Finally, FatalError cannot be recovered.
         */
        void updateHook();

        /** This hook is called by Orocos when the component is in the
         * RunTimeError state, at each activity step. See the discussion in
         * updateHook() about triggering options.
         *
         * Call recover() to go back in the Runtime state.
         */
        void errorHook();

        /** This hook is called by Orocos when the state machine transitions
         * from Running to Stopped after stop() has been called.
         */
        void stopHook();

        /** This hook is called by Orocos when the state machine transitions
         * from Stopped to PreOperational, requiring the call to configureHook()
         * before calling start() again.
         */
        void cleanupHook();

        void eventsCallBack();

        void imuCallBack();
    };
}

#endif

