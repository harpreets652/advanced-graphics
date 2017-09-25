# - Try to find SDL2_Image
# Once done this will define
#
#  DevIL_FOUND - system has devIL
#  DevIL_INCLUDE_DIR - the devIL include directory
#  DevIL_LIBRARIES - Link these to use devIL
#

SET(DEV_IL "IL")

FIND_PATH(DEV_IL_INCLUDE_DIR NAMES IL/il.h
        PATHS
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/include
        /usr/local/include
        /opt/local/include
        NO_DEFAULT_PATH
        )


FIND_LIBRARY(LIB_DEV_IL NAMES ${DEV_IL}
        PATHS
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        NO_DEFAULT_PATH
        )

SET(DEV_IL_LIBRARIES ${LIB_DEV_IL})

IF (DEV_IL_INCLUDE_DIR AND DEV_IL_LIBRARIES)
    SET(DEV_IL_FOUND TRUE)
ENDIF (DEV_IL_INCLUDE_DIR AND DEV_IL_LIBRARIES)

IF (DEV_IL_FOUND)
    MARK_AS_ADVANCED(DEV_IL_INCLUDE_DIR DEV_IL_LIBRARIES)
ENDIF (DEV_IL_FOUND)