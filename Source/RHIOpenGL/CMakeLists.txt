# Project name
PROJECT(RHIOpenGL) 
SET(TARGET_TYPE "SHARED_LIB")

# dependency
SET(projectLibList Core)

# INCLUDES
#____________________________
INCLUDE(OpenGL)
INCLUDE(GLEW)
INCLUDE(GLM)

# Common rules
INCLUDE(CMakeCommon)