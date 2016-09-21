SET(VULKAN_DIR 
INCLUDE_DIRECTORIES(${THIRDPARTY_DIR}/assimp-3.2/include)
LINK_DIRECTORIES(${THIRDPARTY_DIR}/assimp-3.2/Generated_VC14.x64/code)
LINK_LIBRARIES(optimized assimp debug assimpd)