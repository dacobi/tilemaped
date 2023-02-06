cmake_minimum_required(VERSION 3.1)

project(Tilemaped)

file(GLOB Tilemaped_SRC "*.cpp" ".h")

add_executable(tilemaped ${Tilemaped_SRC})

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(CMAKE_CPP_COMPILER "/mingw64/bin/g++")
	set(CMAKE_CPP_FLAGS "-std=c++17 -mwindows")
	target_include_directories(tilemaped PUBLIC C:/msys64/mingw64/include/SDL2)
	target_link_directories(tilemaped PUBLIC C:/msys64/mingw64/lib)
	add_definitions(-Dmain=SDL_main)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	target_include_directories(tilemaped PUBLIC /usr/include/SDL2)	
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
set(CMAKE_CPP_FLAGS "-std=c++17")
target_link_libraries(tilemaped SDL2 SDL2_image SDL2_ttf)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
target_link_libraries(tilemaped mingw32 SDL2main SDL2 SDL2_image SDL2_ttf)
endif()



