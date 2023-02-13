cmake_minimum_required(VERSION 3.1)

project(Tilemaped)

file(GLOB Tilemaped_SRC "*.cpp" ".h")

add_executable(tilemaped ${Tilemaped_SRC})

find_package(SDL2 REQUIRED)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(CMAKE_CXX_COMPILER "g++")
	set(CMAKE_CXX_FLAGS "-std=c++17 -mwindows")
	target_include_directories(tilemaped PUBLIC ${SDL2_INCLUDE_DIRS})
	target_link_directories(tilemaped PUBLIC C:/msys64/mingw64/lib)
	add_definitions(-Dmain=SDL_main)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(CMAKE_CXX_COMPILER "g++")
	target_include_directories(tilemaped PUBLIC . ${SDL2_INCLUDE_DIRS})
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(CMAKE_CXX_FLAGS "-std=c++17")
	target_link_libraries(tilemaped ${SDL2_LIBRARIES} SDL2_image SDL2_ttf)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	#	target_link_libraries(tilemaped mingw32 SDL2main SDL2 SDL2_image SDL2_ttf)
	target_link_libraries(tilemaped mingw32 ${SDL2_LIBRARIES} SDL2_image SDL2_ttf)
endif()
