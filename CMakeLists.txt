cmake_minimum_required(VERSION 3.21)
project(memcpy_vectors)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(glm)

set(extern_libs
        glm
        )

add_executable(memcpy_vectors main.cpp)

target_link_libraries(memcpy_vectors
        ${extern_libs}
        )