cmake_minimum_required(VERSION 3.0.0)

project(ContextFreeGrammarLab)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")

set(CMAKE_AUTOMOC ON)
set(REPC_DIR "C:/Qt_/5.12.1/msvc2017_64/bin/repc.exe" CACHE STRING "Path to REPC" FORCE)
set(FAKE_TARGET_REPC_SERVER fakeTargetRepcServer)
add_custom_target(fakeTargetRepcServer
	${REPC_DIR} -o source -i rep "${CMAKE_CURRENT_SOURCE_DIR}/CfgAlgoRunner.rep" "${CMAKE_CURRENT_SOURCE_DIR}/rep_CfgAlgoRunner_source.h"	
)

file(GLOB ContextFreeGrammarLab_SRC
    "*.h"
    "*.cpp"
    "*.rep"
)

add_executable(ContextFreeGrammarLab ${ContextFreeGrammarLab_SRC})
add_dependencies(ContextFreeGrammarLab ${FAKE_TARGET_REPC_SERVER})

find_package(Qt5Core REQUIRED)
find_package(Qt5RemoteObjects REQUIRED)

target_link_libraries(ContextFreeGrammarLab Qt5::Core Qt5::RemoteObjects)
target_link_libraries(ContextFreeGrammarLab ContextFreeGrammar)
