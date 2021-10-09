cmake_minimum_required(VERSION 3.5)

project(CVInstallTest LANGUAGES CXX)

set(ENV{PKG_CONFIG_PATH} ../../../../Opencv/cvlib/pc/lib/pkgconfig)

find_package(PkgConfig)

pkg_search_module(CV REQUIRED opencv)

include_directories(${CV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

LINK_DIRECTORIES("/Users/ych-caihui/Documents/Develop/Code/MediaCode/multimedia/Opencv/cvlib/pc/lib")


add_executable(CVInstallTest main.cpp)

#target_link_libraries(CVInstallTest  opencv_dnn opencv_highgui opencv_ml opencv_objdetect opencv_shape opencv_stitching opencv_superres opencv_videostab opencv_calib3d opencv_videoio opencv_imgcodecs opencv_features2d opencv_video opencv_photo opencv_imgproc opencv_flann opencv_core )
target_link_libraries(CVInstallTest ${CV_LDFLAGS})
