project(Tilemaped)

set(CMAKE_CPP_COMPILER "/mingw64/bin/g++")
set(CMAKE_CPP_FLAGS "-std=c++17 -mwindows")

add_executable(tilemaped tilemaped.cpp)

target_include_directories(tilemaped PUBLIC C:/msys64/mingw64/include/SDL2)

target_link_directories(tilemaped PUBLIC C:/msys64/mingw64/lib)

target_link_libraries(tilemaped mingw32 SDL2main SDL2 SDL2_image SDL2_ttf)

add_definitions(-Dmain=SDL_main)
