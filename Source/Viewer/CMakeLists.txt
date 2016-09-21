# PROJECT
PROJECT(Viewer)
SET(TARGET_TYPE "EXE")

# dependency
SET(projectLibList Core)

# INCLUDES
#____________________________
INCLUDE(GLEW)
INCLUDE(FreeGLUT)
INCLUDE(GLM)
#INCLUDE(GLI)
#INCLUDE(Assimp)

# COMPILATION
#____________________________
INCLUDE(CMakeCommon)