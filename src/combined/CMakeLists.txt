cmake_minimum_required(VERSION 3.5.1)
project(combined)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wfatal-errors -std=c++11 --preload-file ../../src/client/resources --preload-file ../../src/server/maps -s USE_SDL=2 -s USE_ZLIB=1 -s USE_LIBPNG=1 -s USE_SDL_IMAGE=2 -s \"SDL2_IMAGE_FORMATS=['png']\"")

add_executable(combined
	../client/Client.cpp ../client/ClientCharacter.cpp ../client/ClientEntity.cpp ../client/main.cpp ../client/UniformSpriteSheet.cpp
	../common/Chunk.cpp ../common/MyAssert.cpp ../common/ProcessSocket.cpp ../common/Tile.cpp
	../server/ConnectionManager.cpp ../server/Server.h
	../libs/tinyxml2.cpp
)
