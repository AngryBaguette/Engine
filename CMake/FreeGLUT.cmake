ADD_DEFINITIONS(-DFREEGLUT_STATIC)
INCLUDE_DIRECTORIES(${THIRDPARTY_DIR}/freeglut/include)
LINK_DIRECTORIES(${THIRDPARTY_DIR}/freeglut/lib)
LINK_LIBRARIES(optimized freeglut_static debug freeglut_staticd)
