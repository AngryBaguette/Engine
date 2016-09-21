CMAKE_MINIMUM_REQUIRED(VERSION 3.2 FATAL_ERROR)
CMAKE_POLICY(VERSION 3.2)

PROJECT(Pintar)

################################
# Set some directory
# Where to find included cmake file
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)
# Where to find external
SET(THIRDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/build_VC14.x64)
# The Generated directry
SET(GENERATED_DIR ${CMAKE_BINARY_DIR})

SET(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/build)

#set this variable to specify a common place where CMake should put all executable files (instead of CMAKE_CURRENT_BINARY_DIR)
#SET(EXECUTABLE_OUTPUT_PATH ${GENERATED_DIR}/bin)
#set this variable to specify a common place where CMake should put all libraries (instead of CMAKE_CURRENT_BINARY_DIR)
#SET(LIBRARY_OUTPUT_PATH ${GENERATED_DIR}/lib)

################################
# Add definitions
if(MSVC)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
endif(MSVC)

################################
# Add subdirectory
ADD_SUBDIRECTORY(Source/Core)
ADD_SUBDIRECTORY(Source/Viewer)
ADD_SUBDIRECTORY(Source/VulkanSimple)
