cmake_minimum_required(VERSION 3.29)
project(TrainView C)

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
endif()

# Sources
file(GLOB SOURCES src/*.c src/client/*.c)

#include_directories(quake2 ${SDL2_INCLUDE_DIRS})

add_executable(TrainView ${SOURCES})

target_link_libraries(TrainView PRIVATE ${PLATFORM_LIBS} SDL2main libglew_static cglm stb cimgui cimplot)# )#vulkan glew gl glu)
target_link_libraries(TrainView PUBLIC SDL2)

target_compile_options(TrainView PRIVATE
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

add_dependencies(TrainView copy_data copy_info)