# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)
set(lib_src_DIR ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})
set(in_src_DIR ${CMAKE_SOURCE_DIR}/include)
include_directories(${in_src_DIR})

#link_directories(../../../../../../../fflib/android/arm64-v8a)
# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             native-lib.cpp )
include_directories()
message("CURRENT_DIR:" ${in_src_DIR})
# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.
#link_directories(../jniLibs/arm64-v8a)
find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )
#find_library(ffmpeg-lib ffmpeg)
add_library(ffmpeg-lib SHARED IMPORTED)
set_target_properties(ffmpeg-lib PROPERTIES IMPORTED_LOCATION
        ${lib_src_DIR}/libffmpeg.so)


# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries(native-lib ffmpeg-lib ${log-lib})