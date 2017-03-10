cmake_minimum_required(VERSION 3.6)
project(mpd_client)

set(CMAKE_C_STANDARD 11)

# ncurses stuff
set(CURSES_USE_NCURSES TRUE)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(SOURCE_FILES main.c)
add_executable(mpd_client ${SOURCE_FILES})
target_link_libraries(mpd_client ${CURSES_LIBRARIES})
target_link_libraries(mpd_client mpdclient)
