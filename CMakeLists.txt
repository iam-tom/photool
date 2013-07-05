cmake_minimum_required (VERSION 2.6)
project(Photool)
include_directories(include /opt/local/include)
link_directories (${DIR}/src/utils /opt/local/lib lib)
set(EXECUTABLE_OUTPUT_PATH bin)
set(LIBRARY_OUTPUT_PATH lib)


FIND_PACKAGE(Boost COMPONENTS system filesystem REQUIRED)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR}  )


add_library(photo_sort_viewer src/photo_sort_viewer.cpp)
target_link_libraries(photo_sort_viewer opencv_core opencv_imgproc opencv_highgui ${Boost_LIBRARIES})

add_executable(photosort src/photosort.cpp)
target_link_libraries(photosort opencv_core opencv_imgproc opencv_highgui ${Boost_LIBRARIES} photo_sort_viewer)

add_executable(watermarker src/watermarker.cpp)
target_link_libraries(watermarker opencv_core opencv_imgproc opencv_highgui ${Boost_LIBRARIES} )
