# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(EXTRA_COMPONENT_DIRS components/assets)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
if (NOT DEFINED PROJECT_NAME)
	project(DINAISY)
endif (NOT DEFINED PROJECT_NAME)
