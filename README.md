# TilemapEd
TilemapEd is a tilemap editor for the Commander X16 retro computer.

For more information checkout this link [https://cx16forum.com/forum/viewtopic.php?t=6365](https://cx16forum.com/forum/viewtopic.php?t=6365)

It uses C++17, SDL2 and SDL2_image.

TilemapEd uses Dear ImGui for UI. A copy of the license is included in the repo [LICENSE.ImGui](https://raw.githubusercontent.com/ocornut/imgui/master/LICENSE.txt)     
[LICENSE.ImGuiFileDialog](https://raw.githubusercontent.com/aiekick/ImGuiFileDialog/Lib_Only/LICENSE)

A copy of Nerdfonts TTF fonts are included in the repo according to [LICENSE.nerdfonts](https://github.com/ryanoasis/nerd-fonts/blob/master/LICENSE)

Run "cmake CMakeLists.txt" to configure and then "make" to build in \*nix.      
In Windows in MSYS2 in MINGW64 Shell run "cmake --build . " to build.

To setup SDL2 in MSYS2 follow this guide [msys2-SDL2-Setup.md](https://gist.github.com/thales17/fb2e4cff60890a51d9dddd4c6e832ad2)

TilemapEd uses libxml2. To install in MSYS2 run "pacman -S mingw-w64-x86_64-libxml2".
