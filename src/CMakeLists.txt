cmake_minimum_required(VERSION 3.10)

set(avl_LIBS ${LIBS})

set(avl_SRCS avl.c)

add_library(avl ${avl_SRCS})
target_link_libraries(avl ${avl_LIBS})
