cmake_minimum_required(VERSION 3.29.6)
project(cGTFS C)

set(CMAKE_C_STANDARD_REQUIRED 99)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/bin)

#option(VENDOR_LIBRARIES "Use vendored libraries" OFF)

set(SDL_STATIC OFF CACHE BOOL "" FORCE)
set(SDL_SHARED ON CACHE BOOL "" FORCE)

#set(CGLM_STATIC ON CACHE BOOL "" FORCE)
#set(CGLM_SHARED OFF CACHE BOOL "" FORCE)

include_directories(include)

add_subdirectory(ext)

if (WIN32)
    set(PLATFORM_LIBS mingw32)
    add_definitions(-DUNICODE -D_UNICODE)
endif()

# Sources
file(GLOB SOURCES src/*.c src/gtfs/*.c src/ui/*.c src/util/*.c)

#include_directories(quake2 ${SDL2_INCLUDE_DIRS})

add_executable(cGTFS ${SOURCES})

target_link_libraries(cGTFS PRIVATE ${PLATFORM_LIBS} SDL2main libglew_static cimgui cimplot uriparser minizip)# cglm stb)#vulkan glew gl glu)
target_link_libraries(cGTFS PUBLIC SDL2)

target_compile_options(cGTFS PRIVATE
        $<$<CONFIG:Debug>:
        #-pedantic
        #-Wall
        -Wextra
        -Wconversion
        -Wundef
        #-Werror
        -Wno-unused-parameter
        -Wno-unused-variable
        -Wno-unused-but-set-variable
        >
)

add_custom_target(copy_data
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/data/
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/data/)

add_custom_target(copy_info
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/LICENCE
        ${CMAKE_CURRENT_SOURCE_DIR}/CREDITS
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

add_dependencies(cGTFS copy_data copy_info)