cmake_minimum_required(VERSION 3.10)
project(ConnectN C)

set(CMAKE_C_STANDARD 99)

# Raylib directory
set(RAYLIB_PATH "C:/Users/aleks/Downloads/raylib-5.5_win32_mingw-w64") 

include_directories(${RAYLIB_PATH}/include)
link_directories(${RAYLIB_PATH}/lib)

# Aggiungi tutti i file sorgente
add_executable(ConnectN 
    main.c
    board.c
    player.c
    arbre.c
    item.c
    algorithm.c
    ui.c
)

# Aggiungi le librerie necessarie
target_link_libraries(ConnectN raylib opengl32 gdi32 winmm)