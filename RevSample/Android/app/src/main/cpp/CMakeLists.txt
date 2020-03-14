cmake_minimum_required(VERSION 3.4.1)

# Choose Graphics system
set(VULKAN 1)
set(GLES 0)
set(DEBUG 1)

if(VULKAN)
    add_definitions(-D_USE_VULKAN)
elseif(GLES)
    add_definitions(-D_USE_GLES)
endif()

if(DEBUG)
    add_definitions(
            -D_DEBUG
            -D_ENABLE_LOG
    )
endif()

set(CORELIB_DIR  ../../../../../../CoreLib)
set(PLATFORMLIB_DIR  ../../../../../../PlatformLib)
set(ANDROIDLIB_DIR  ../../../../../../PlatformLib/AndroidLib)
set(REV_DIR  ../../../../../../Rev)

set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")

# TODO:check app size betweeen hidden and public
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

add_definitions(
        -D_ANDROID
        -DVK_USE_PLATFORM_ANDROID_KHR
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")


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
if(VULKAN)
    target_link_libraries(native-activity
        android
        native_app
        Rev
        AndroidLib
        CoreLib
        log)
elseif(GLES)
    target_link_libraries(native-activity
            android
            native_app
            Rev
            AndroidLib
            CoreLib
            EGL
            GLESv1_CM
            log)
endif()