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
    /** Color encodeing: first color positive event, seconds color negative event **/
    enum COLOR_ENCODING{BLUE_RED, GREEN_RED, BLUE_BLACK};
}

#endif

