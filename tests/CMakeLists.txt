cmake_minimum_required(VERSION 3.10)

enable_testing()

find_package(Check REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-variable")

include_directories(. ../src)

set(test_LIBS ${LIBS} avl ${CHECK_LIBRARIES})

set(test_SRCS avl_test_utils.c)

add_executable(test_funcs avl_test_funcs.c ${test_SRCS})
target_link_libraries(test_funcs ${test_LIBS})
add_test(test_funcs ${TEST_PATH}/test_funcs)

add_executable(test_combos avl_test_combos.c ${test_SRCS})
target_link_libraries(test_combos ${test_LIBS})
add_test(test_combos ${TEST_PATH}/test_combos)
