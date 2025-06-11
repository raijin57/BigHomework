cmake_minimum_required(VERSION 3.31)
project(BDZ)
enable_testing()
set(CMAKE_CXX_STANDARD 20)
add_executable(run_tests
        tests/vector_public_test.cpp
        vector+.h
        sharedptr.h
)
target_compile_definitions(run_tests PRIVATE
        VECTOR_MEMORY_IMPLEMENTED
)
target_include_directories(run_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}  # Для vector.h и catch.hpp
)
add_test(NAME AllTests COMMAND run_tests)