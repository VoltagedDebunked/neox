cmake_minimum_required(VERSION 3.10)
project(neox)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(WEBKIT2GTK REQUIRED webkit2gtk-4.0)
pkg_check_modules(GTK REQUIRED gtk+-3.0)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

include_directories(${WEBKIT2GTK_INCLUDE_DIRS} ${GTK_INCLUDE_DIRS} ${JSONCPP_INCLUDE_DIRS})
link_directories(${WEBKIT2GTK_LIBRARY_DIRS} ${GTK_LIBRARY_DIRS} ${JSONCPP_LIBRARY_DIRS})
add_definitions(${WEBKIT2GTK_CFLAGS_OTHER} ${GTK_CFLAGS_OTHER} ${JSONCPP_CFLAGS_OTHER})

add_executable(neox src/main.cpp)

target_link_libraries(neox ${WEBKIT2GTK_LIBRARIES} ${GTK_LIBRARIES} ${JSONCPP_LIBRARIES})
