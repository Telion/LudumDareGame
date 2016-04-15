cmake_minimum_required(VERSION 3.5.1)
project(combined)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 --preload-file ../../src/client/resources -s USE_SDL=2 -s USE_ZLIB=1 -s USE_LIBPNG=1 -s USE_SDL_IMAGE=2 -s \"SDL2_IMAGE_FORMATS=['png']\"")

add_executable(combined
	../client/main.cpp
)
