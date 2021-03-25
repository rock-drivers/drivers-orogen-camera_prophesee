PROJECT(camera_prophesee)
cmake_minimum_required(VERSION 2.6)

# Currently the prophesee driver does not have a pkg-config file
find_package(prophesee_driver REQUIRED)
find_package(prophesee_core REQUIRED)

# The Prophesee driver uses Boost and since we don't cretae a wrapper we need to
# link against boost libraries in the orogen component
find_package(Boost COMPONENTS system filesystem program_options thread python REQUIRED)

# Print all the available variables
#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

SET (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/.orogen/config")
INCLUDE(camera_propheseeBase)

# FIND_PACKAGE(KDL)
# FIND_PACKAGE(OCL)

