CMAKE_MINIMUM_REQUIRED (VERSION 3.19.4)
PROJECT(cameracalibrator CXX)


FIND_PACKAGE(OpenCV REQUIRED)


INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

MESSAGE( STATUS "OpenCV_VERSION = ${OpenCV_VERSION}")
MESSAGE( STATUS "OpenCV_LIBS = ${OpenCV_LIBS}")
MESSAGE( STATUS "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")

add_executable(checkerboard_generator checkerboard_generator.cc)
target_link_libraries(checkerboard_generator ${OpenCV_LIBS})

add_executable(calibrator_target_generator calibrator_target_generator.cc)
target_link_libraries(calibrator_target_generator ${OpenCV_LIBS})

add_executable(camera_calibrator camera_calibrator.cc)
target_link_libraries(camera_calibrator ${OpenCV_LIBS})




