cmake_minimum_required(VERSION 3.6)
project(ncmpclone)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "-g")

# ncurses stuff
set(CURSES_USE_NCURSES TRUE)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(SOURCE_FILES
        main.c
        title_bar.c
        screen_queue.c
        screen_browse.c
        status_bar.c
        mpd_info.c
        screen_help.c
        list.c
        command.c
        command_player.c
        ncmpclone_ncurses.c
        )
add_executable(ncmpclone ${SOURCE_FILES})
target_link_libraries(ncmpclone -lmenu -lpanel ${CURSES_LIBRARIES})
target_link_libraries(ncmpclone mpdclient)
