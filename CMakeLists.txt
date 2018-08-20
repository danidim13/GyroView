cmake_minimum_required(VERSION 2.8)

project( GyroView )

set( GyroView_VERSION_MAJOR 1 )
set( GyroView_VERSION_MINOR 0 )

find_package( OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

set( GYROVIEW_SOURCES
  src/main.cpp
)
set( MARKER_SOURCES
  src/make_markers.cpp
)

add_executable( GyroView ${GYROVIEW_SOURCES} )
add_executable( MakeMarkers ${MARKER_SOURCES} )

target_link_libraries( GyroView ${OpenCV_LIBS} )
target_link_libraries( MakeMarkers ${OpenCV_LIBS} )
