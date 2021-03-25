#ifndef camera_prophesee_TYPES_HPP
#define camera_prophesee_TYPES_HPP

#include <base/Time.hpp>

/* If you need to define types specific to your oroGen components, define them
 * here. Required headers must be included explicitly
 *
 * However, it is common that you will only import types from your library, in
 * which case you do not need this file
 */

namespace camera_prophesee
{
    struct Event
    {
        unsigned int x;
        unsigned int y;
        ::base::Time ts;
        bool polarity;
    };

    struct EventArray
    {
        ::base::Time time;
        unsigned int height;
        unsigned int width;

        std::vector<Event> events;
    };


}

#endif

