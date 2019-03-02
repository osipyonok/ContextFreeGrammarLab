cmake_minimum_required(VERSION 3.0.0)

project(ContextFreeGrammar)

add_definitions(-DCONTEXTFREEGRAMMAR_LIB)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")

file(GLOB ContextFreeGrammar_SRC
    "*.h"
    "*.cpp"
)

add_library(ContextFreeGrammar SHARED ${ContextFreeGrammar_SRC})

find_package(Qt5Core REQUIRED)

target_link_libraries(ContextFreeGrammar Qt5::Core)

target_include_directories(ContextFreeGrammar PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)