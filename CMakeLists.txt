cmake_minimum_required(VERSION 3.16)
project(sftd)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/test")
set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")

option(BUILD_TEST "build test modules" ON)
include_directories(.)

macro(LIST_HEADER path list)
    file(GLOB files RELATIVE ${path} ${path}/*.h*)
    foreach(file ${files})
        list(APPEND ${list} ${path}/${file})
    endforeach()
endmacro()

macro(LIST_SRC path list)
    file(GLOB files RELATIVE ${path} ${path}/*.cc)
    foreach(file ${files})
        list(APPEND ${list} ${path}/${file})
    endforeach()
endmacro()

if (BUILD_TEST)
    find_package(GTest REQUIRED)
    find_package(Threads REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
endif()

include(sugars/CMakeLists.txt)
include(lex/CMakeLists.txt)
include(tables/CMakeLists.txt)
include(exceptions/CMakeLists.txt)
include(grammar/CMakeLists.txt)
include(syntax/CMakeLists.txt)

add_library(sftd SHARED main.cc ${SUGAR_HEADER} ${SUGAR_SRC}
        ${LEX_HEADER} ${LEX_SRC}
        ${TABLES_HEADER} ${TABLES_SRC}
        ${EXCEPTIONS_HEADER} ${EXCEPTIONS_SRC}
        ${GRAMMAR_HEADER} ${GRAMMAR_SRC}
        ${SYNTAX_HEADER} ${SYNTAX_SRC})

if (BUILD_TEST)
    add_library(sftd_test main.cc ${SUGAR_HEADER} ${SUGAR_SRC} ${LEX_HEADER} ${LEX_SRC} ${TABLES_HEADER} ${TABLES_SRC} ${EXCEPTIONS_HEADER} ${EXCEPTIONS_SRC} ${GRAMMAR_HEADER} ${GRAMMAR_SRC} ${SYNTAX_HEADER} ${SYNTAX_SRC})
    file(GLOB test_files RELATIVE ${CMAKE_SOURCE_DIR}/tests ${CMAKE_SOURCE_DIR}/tests/*.cc)
    foreach(filename ${test_files})
        string(REPLACE ".cc" "" test_name ${filename})
        message("-- build test: ${test_name}")
        add_executable(${test_name} ${CMAKE_SOURCE_DIR}/tests/${filename})
        target_link_libraries(${test_name} ${LIBRARY_OUTPUT_PATH}/libsftd_test.a)
        target_link_libraries(${test_name} ${GTEST_BOTH_LIBRARIES})
        target_link_libraries(${test_name} ${CMAKE_THREAD_LIBS_INIT})
    endforeach()
endif()