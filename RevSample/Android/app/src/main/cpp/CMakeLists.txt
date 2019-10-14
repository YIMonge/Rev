cmake_minimum_required(VERSION 3.4.1)

message("Build Start")

set(CORELIB_DIR  ../../../../../../CoreLib)
set(PLATFORMLIB_DIR  ../../../../../../PlatformLib)
set(ANDROIDLIB_DIR  ../../../../../../PlatformLib/AndroidLib)
set(REV_DIR  ../../../../../../Rev)

set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")

# TODO:check app size betweeen hidden and public
set(CMAKE_CXX_VISIBILITY_PRESET hidden)


add_definitions(
        -D_ANDROID
        -D_USE_VULKAN
        -DVK_USE_PLATFORM_ANDROID_KHR
        #-D_USE_GLES
)

add_library(native_app STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

#------------------Build CoreLib-----------------------------
file(GLOB CORELIBSOURCES ${CORELIB_DIR}/cpp/*.cpp)
add_library(CoreLib STATIC
        ${CORELIBSOURCES}
        )

message("Build CoreLib Done")

#------------------Build AndroidLib-----------------------------
file(GLOB ANDROIDLIBSOURCES ${ANDROIDLIB_DIR}/cpp/*.cpp)
add_library(AndroidLib STATIC
        ${ANDROIDLIBSOURCES}
        ${ANDROIDLIB_DIR}/lib/vulkan_wrapper.cpp
        )
target_include_directories(AndroidLib PRIVATE
        ${CORELIB_DIR}/include
        ${PLATFORMLIB_DIR}/interface
        ${ANDROIDLIB_DIR}/include
        ${ANDROID_NDK}/sources/android/native_app_glue
        )

#target_include_directories(AndroidLib PRIVATE
#        ${CORELIB_DIR}/include
#        ${ANDROIDLIB_DIR}/include
#        ${PLATFORMLIB_DIR}/interface
#        ${ANDROID_NDK}/sources/android/native_app_glue   #if use glse
#        )

message("Build AndroidLib Done")

#------------------Build Rev-----------------------------
file(GLOB REVSOURCES  ${REV_DIR}/cpp/*.cpp)
add_library(Rev STATIC
        ${REVSOURCES})
target_include_directories(Rev PRIVATE
        ${CORELIB_DIR}/include
        ${PLATFORMLIB_DIR}/interface
        ${ANDROIDLIB_DIR}/include
        ${REV_DIR}/include
        )
message("Build Rev Done")

# now build app's shared lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall -Werror")

set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

add_library(native-activity SHARED main.cpp)

target_include_directories(native-activity PRIVATE
        ${CORELIB_DIR}/include
        ${ANDROIDLIB_DIR}/include
        ${PLATFORMLIB_DIR}/interface
        ${REV_DIR}/include
        ${ANDROID_NDK}/sources/android/native_app_glue)

# add lib dependencies
target_link_libraries(native-activity
        android
        native_app
        Rev
        AndroidLib
        CoreLib
        EGL
        GLESv1_CM
        log)