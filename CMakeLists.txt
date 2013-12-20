cmake_minimum_required (VERSION 2.6)
project(Photool)
include_directories(include /opt/local/include)
link_directories (${DIR}/src/utils /opt/local/lib ${DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH bin)
set(LIBRARY_OUTPUT_PATH lib)

find_package(OpenMP)
if (OPENMP_FOUND)
      set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()


FIND_PACKAGE(Boost COMPONENTS system filesystem REQUIRED)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR}  )

#########################################################
add_library(photo_utils src/photo_utils.cpp)
target_link_libraries(photo_utils opencv_core opencv_imgproc opencv_highgui ${Boost_LIBRARIES})

add_library(photo_sort_viewer src/photo_sort_viewer.cpp)
target_link_libraries(photo_sort_viewer opencv_core opencv_imgproc opencv_highgui ${Boost_LIBRARIES})

#########################################################
add_executable(photool-phosort src/phosort.cpp)
target_link_libraries(photool-phosort opencv_core opencv_imgproc opencv_highgui ${Boost_LIBRARIES} photo_sort_viewer)

add_executable(photool-phoview src/phoview.cpp)
target_link_libraries(photool-phoview opencv_core opencv_imgproc opencv_highgui ${Boost_LIBRARIES} photo_sort_viewer)

add_executable(photool-watermarker src/watermarker.cpp)
target_link_libraries(photool-watermarker opencv_core opencv_imgproc opencv_highgui ${Boost_LIBRARIES} )

add_executable(photool-photo_importer src/photo_importer.cpp)
target_link_libraries(photool-photo_importer photo_utils opencv_core opencv_imgproc opencv_highgui ${Boost_LIBRARIES} )
