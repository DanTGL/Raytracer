cmake_minimum_required(VERSION 3.11)
project(Raytracer VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(OpenGL REQUIRED)

find_package(SDL2 REQUIRED)

find_package(OpenMP)


include_directories(Raytracer ${SDL2_INCLUDE_DIRS})

include_directories("./include")
file(GLOB SRC_FILES "src/**.cpp")

add_executable(Raytracer ${SRC_FILES})

if(NOT TARGET OpenMP::OpenMP_CXX)
    find_package(Threads REQUIRED)
    add_library(OpenMP::OpenMP_CXX IMPORTED INTERFACE)
    set_property(TARGET OpenMP::OpenMP_CXX
                 PROPERTY INTERFACE_COMPILE_OPTIONS ${OpenMP_CXX_FLAGS})
    # Only works if the same flag is passed to the linker; use CMake 3.9+ otherwise (Intel, AppleClang)
    set_property(TARGET OpenMP::OpenMP_CXX
                 PROPERTY INTERFACE_LINK_LIBRARIES ${OpenMP_CXX_FLAGS} Threads::Threads)

endif()

target_link_libraries(Raytracer PUBLIC GL GLU ${SDL2_LIBRARIES} OpenMP::OpenMP_CXX GLEW)

set(CPACK)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
