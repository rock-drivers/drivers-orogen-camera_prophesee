PROJECT(camera_prophesee)
cmake_minimum_required(VERSION 2.6)

# Currently the prophesee driver does not have a pkg-config file
find_package(prophesee_driver REQUIRED)
find_package(Boost COMPONENTS system filesystem program_options thread python REQUIRED)

get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()


SET (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/.orogen/config")
INCLUDE(camera_propheseeBase)

# FIND_PACKAGE(KDL)
# FIND_PACKAGE(OCL)

