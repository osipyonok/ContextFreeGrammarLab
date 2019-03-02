cmake_minimum_required(VERSION 3.0.0)

project(ContextFreeGrammarClient)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(REPC_DIR "C:/Qt_/5.12.1/msvc2017_64/bin/repc.exe" CACHE STRING "Path to REPC" FORCE)
set(FAKE_TARGET_REPC_CLIENT fakeTargetRepcClient)
add_custom_target(fakeTargetRepcClient
	${REPC_DIR} -o replica -i rep "${CMAKE_CURRENT_SOURCE_DIR}/CfgAlgoRunner.rep" "${CMAKE_CURRENT_SOURCE_DIR}/rep_CfgAlgoRunner_replica.h"	
)

file(GLOB ContextFreeGrammarClient_SRC
    "*.h"
    "*.cpp"
    "*.ui"
    "*.qrc"
    "*.rep"
    "Resources/*.*"
)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
add_executable(ContextFreeGrammarClient ${ContextFreeGrammarClient_SRC})
add_dependencies(ContextFreeGrammarClient ${FAKE_TARGET_REPC_CLIENT})



find_package(Qt5Core REQUIRED)
find_package(Qt5RemoteObjects REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

target_link_libraries(ContextFreeGrammarClient Qt5::Core Qt5::RemoteObjects Qt5::Gui Qt5::Widgets)
target_link_libraries(ContextFreeGrammarClient ContextFreeGrammar)