STRING (REGEX MATCH "EXE" _matchExe "${TARGET_TYPE}")
STRING (REGEX MATCH "STATIC_LIB" _matchStaticLib "${TARGET_TYPE}")
STRING (REGEX MATCH "SHARED_LIB" _matchSharedLib "${TARGET_TYPE}")
STRING (REGEX MATCH "PLUGIN" _matchPlugin "${TARGET_TYPE}")
	
# Get all include file
# incExcludeList : file to exclude
# incAppendList : file to append
FILE(GLOB incList "Inc/*.h*")
FOREACH(incExcludeItem ${incExcludeList})
	FILE(GLOB incExcludeFile "Inc/${incExcludeItem}")
	LIST(REMOVE_ITEM incList ${incExcludeFile})
ENDFOREACH(incExcludeItem)
LIST(APPEND incList ${incAppendList})

# Get source file
# srcExcludeItem : file to exclude
# srcAppendList : file to append
FILE(GLOB srcList "Src/*.c*")
FOREACH(srcExcludeItem ${srcExcludeList})
	FILE(GLOB srcExcludeFile "Src/${srcExcludeItem}")
	LIST(REMOVE_ITEM srcList ${srcExcludeFile})
ENDFOREACH(srcExcludeItem)
LIST(APPEND srcList ${srcAppendList})

# Create the target
IF(_matchExe)
	ADD_EXECUTABLE(${PROJECT_NAME} ${srcList} ${incList})
	ADD_DEFINITIONS(-D${PROJECT_NAME}_MAKEEXE)
ELSEIF(_matchStaticLib)
	ADD_LIBRARY(${PROJECT_NAME} STATIC ${srcList} ${incList})
	ADD_DEFINITIONS(-D${PROJECT_NAME}_MAKELIB)
ELSEIF(_matchSharedLib)
	ADD_LIBRARY(${PROJECT_NAME} SHARED ${srcList} ${incList})
	ADD_DEFINITIONS(-D${PROJECT_NAME}_MAKEDLL)
ELSEIF(_matchPlugin)
	ADD_LIBRARY(${PROJECT_NAME} MODULE ${srcList} ${incList})
	ADD_DEFINITIONS(-D${PROJECT_NAME}_MAKEDLL)
ELSE(_matchExe)
	MESSAGE(FATAL_ERROR "Unknown target type")
ENDIF(_matchExe)

# Make sure the compiler can find include files for our project library
# when other libraries or executables link to Hello
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Inc)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${projectLibList})

# configure a header file to pass some of the CMake settings
# to the source code
CONFIGURE_FILE (
  "${PROJECT_SOURCE_DIR}/../Config.h.in"
  "${PROJECT_SOURCE_DIR}/Inc/${PROJECT_NAME}Config.hpp"
  )
 
# Change the postfix
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ".d")
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES RELWITHDEBINFO_POSTFIX ".rd")

INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
		INCLUDES DESTINATION inc
		)
		
MACRO(POST_BUILD_COPY Src Dst)
	STRING(REPLACE "/" "\\" SrcNative ${Src})
	STRING(REPLACE "/" "\\" DstNative ${Dst})

	ADD_CUSTOM_COMMAND(	TARGET ${PROJECT_NAME}
						POST_BUILD			   
						COMMAND if EXIST \"${SrcNative}\" \( echo F | xcopy /d /y /i \"${SrcNative}\" \"${DstNative}\" & if NOT ERRORLEVEL 0 \( echo Error can't copy \"${SrcNative}\" to \"${DstNative}\" & exit 1 \) \)
						)
ENDMACRO(POST_BUILD_COPY)

# Post build copy
#FILE (TO_CMAKE_PATH ${LIBRARY_PATH} LIBRARY_PATH)
#ADD_CUSTOM_COMMAND(	TARGET ${PROJECT_NAME}
#					POST_BUILD 
#					COMMAND echo Copying files to release directory...)
#FILE( MAKE_DIRECTORY ${GENERATED_DIR}/Bin)
#POST_BUILD_COPY( ${PROJECT_BINARY_DIR}/Debug/${PROJECT_NAME}*.exe ${GENERATED_DIR}/Bin)
#POST_BUILD_COPY( ${PROJECT_BINARY_DIR}/Debug/${PROJECT_NAME}*.pdb ${GENERATED_DIR}/Bin)
#POST_BUILD_COPY( ${PROJECT_BINARY_DIR}/Debug/${PROJECT_NAME}*.dll ${GENERATED_DIR}/Bin)
#POST_BUILD_COPY( ${PROJECT_BINARY_DIR}/Release/${PROJECT_NAME}*.exe ${GENERATED_DIR}/Bin)
#POST_BUILD_COPY( ${PROJECT_BINARY_DIR}/Release/${PROJECT_NAME}*.pdb ${GENERATED_DIR}/Bin)
#POST_BUILD_COPY( ${PROJECT_BINARY_DIR}/Release/${PROJECT_NAME}*.dll ${GENERATED_DIR}/Bin)
#POST_BUILD_COPY( ${PROJECT_BINARY_DIR}/RelWithDebInfo/${PROJECT_NAME}*.exe ${GENERATED_DIR}/Bin)
#POST_BUILD_COPY( ${PROJECT_BINARY_DIR}/RelWithDebInfo/${PROJECT_NAME}*.pdb ${GENERATED_DIR}/Bin)
#POST_BUILD_COPY( ${PROJECT_BINARY_DIR}/RelWithDebInfo/${PROJECT_NAME}*.dll ${GENERATED_DIR}/Bin)