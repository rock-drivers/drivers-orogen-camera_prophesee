# Generated from orogen/lib/orogen/templates/tasks/CMakeLists.txt

include(camera_propheseeTaskLib)

ADD_LIBRARY(${CAMERA_PROPHESEE_TASKLIB_NAME} SHARED 
    ${CAMERA_PROPHESEE_TASKLIB_SOURCES})
add_dependencies(${CAMERA_PROPHESEE_TASKLIB_NAME}
    regen-typekit)

include_directories(${PROPHESEE_DRIVER_INCLUDE_DIRS})
include_directories("/usr/include/prophesee_core/")
include_directories(${Boost_INCLUDE_DIRS})

TARGET_LINK_LIBRARIES(${CAMERA_PROPHESEE_TASKLIB_NAME}
    ${Boost_LIBRARIES}
    ${Boost_SYSTEM_LIBRARIES}
    ${Boost_FILESYSTEM_LIBRARIES}
    ${Boost_PROGRAM_OPTIONS_LIBRARIES}
    ${Boost_THREAD_LIBRARIES}
    ${Boost_PYTHON_LIBRARIES}
    ${PROPHESEE_DRIVER_CMAKE_DIR}
    ${PROPHESEE_DRIVER_LIBRARIES}
    ${OrocosRTT_LIBRARIES}
    ${QT_LIBRARIES}
    ${CAMERA_PROPHESEE_TASKLIB_DEPENDENT_LIBRARIES})
SET_TARGET_PROPERTIES(${CAMERA_PROPHESEE_TASKLIB_NAME}
    PROPERTIES LINK_INTERFACE_LIBRARIES "${CAMERA_PROPHESEE_TASKLIB_INTERFACE_LIBRARIES}")
SET_TARGET_PROPERTIES(${CAMERA_PROPHESEE_TASKLIB_NAME}
    PROPERTIES INTERFACE_LINK_LIBRARIES
    "${CAMERA_PROPHESEE_TASKLIB_INTERFACE_LIBRARIES}")

target_link_libraries(prophesee_driver)
target_link_libraries(prophesee_core)
target_link_libraries(prophesee_core_private)

INSTALL(TARGETS ${CAMERA_PROPHESEE_TASKLIB_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib/orocos)

INSTALL(FILES ${CAMERA_PROPHESEE_TASKLIB_HEADERS}
    DESTINATION include/orocos/camera_prophesee)

get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()
