cmake_minimum_required(VERSION 3.1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

project(Tilemaped)

find_package(SDL2 2.0.17 REQUIRED)
find_package(SDL2_image REQUIRED)
#find_package(SDL2_ttf REQUIRED)
find_package(LibXml2 REQUIRED)

file(GLOB Tilemaped_SRC "*.cpp" ".h")

add_executable(tilemaped ${Tilemaped_SRC})

target_include_directories(tilemaped SYSTEM PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${LIBXML2_INCLUDE_DIRS})

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(CMAKE_CXX_COMPILER "g++")
	set(CMAKE_CXX_FLAGS "-std=c++17 -O2")
	#-mwindows
	target_link_directories(tilemaped PUBLIC C:/msys64/mingw64/lib)
	add_definitions(-Dmain=SDL_main)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(CMAKE_CXX_COMPILER "g++")
	set(CMAKE_CXX_FLAGS "-std=c++17 -ggdb -Wall -Wno-sign-compare -Wno-unused-variable")
	target_link_libraries(tilemaped ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${LIBXML2_LIBRARIES})
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	#mingw32 SDL2main SDL2 SDL2_image SDL2_ttf
	target_link_libraries(tilemaped mingw32 ${SDL2_LIBRARIES} SDL2_image ${LIBXML2_LIBRARIES})
endif()
