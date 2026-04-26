cmake_minimum_required(VERSION 3.20)
project(order_book CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Aggressive optimization for the main library
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")

# Main library (order_book logic, no main())
add_library(ob_lib
    src/order_book.cpp
)
target_include_directories(ob_lib PUBLIC src)

# Executable
add_executable(order_book src/main.cpp)
target_link_libraries(order_book PRIVATE ob_lib)

# ── Tests ────────────────────────────────────────────────────────────────────
# We use a single-header Catch2 (v2) — no install needed.
# Download it once: cd tests && curl -LO https://github.com/catchorg/Catch2/releases/download/v2.13.10/catch.hpp
enable_testing()
add_executable(tests tests/test_order_book.cpp)
target_include_directories(tests PRIVATE src tests)
target_link_libraries(tests PRIVATE ob_lib)
add_test(NAME unit_tests COMMAND tests)

# ── Bench ────────────────────────────────────────────────────────────────────
add_executable(bench bench/bench.cpp)
target_include_directories(bench PRIVATE src)
target_link_libraries(bench PRIVATE ob_lib)
# Bench always compiles with full optimizations
target_compile_options(bench PRIVATE -O3 -march=native)
