cmake_minimum_required(VERSION 3.6)
project(ncmpclone)

set(CMAKE_C_STANDARD 11)

# ncurses stuff
set(CURSES_USE_NCURSES TRUE)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(SOURCE_FILES
        main.c
        title_bar.c
        screen_queue.c
        status_bar.c
        mpd_info.c screen_help.c screen_help.h)
add_executable(ncmpclone ${SOURCE_FILES})
target_link_libraries(ncmpclone -lmenu -lpanel ${CURSES_LIBRARIES})
target_link_libraries(ncmpclone mpdclient)
