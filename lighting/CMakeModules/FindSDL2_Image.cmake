# - Try to find SDL2_Image
# Once done this will define
#
#  SDL2_IMAGE_FOUND - system has sdl2 image
#  SDL2_IMAGE_INCLUDE_DIR - the sdl2 image include directory
#  SDL2_IMAGE_LIBRARIES - Link these to use sdl2 image
#

SET(SDL2_IMAGE "SDL2_image")

FIND_PATH(SDL2_IMAGE_INCLUDE_DIR NAMES SDL2/SDL_image.h
        PATHS
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/include
        /usr/local/include
        /opt/local/include
        NO_DEFAULT_PATH
        )


FIND_LIBRARY(LIB_SDL2_IMAGE NAMES ${SDL2_IMAGE}
        PATHS
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        NO_DEFAULT_PATH
        )

SET(SDL2_IMAGE_LIBRARIES ${LIB_SDL2_IMAGE})

IF (SDL2_IMAGE_INCLUDE_DIR AND SDL2_IMAGE_LIBRARIES)
    SET(SDL2_IMAGE_FOUND TRUE)
ENDIF (SDL2_IMAGE_INCLUDE_DIR AND SDL2_IMAGE_LIBRARIES)

IF (SDL2_IMAGE_FOUND)
    MARK_AS_ADVANCED(SDL2_IMAGE_INCLUDE_DIR SDL2_IMAGE_LIBRARIES)
ENDIF (SDL2_IMAGE_FOUND)