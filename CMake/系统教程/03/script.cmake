# 这是单行注释

#[[
这是块注释
可以跨多行
CMake 3.0+ 支持
]]

set(MY_NAME "Adorukw")
set(NUMBERS 1 2 3 4 5)
set(EMPTY_VAR "")

message(STATUS "My name is ${MY_NAME}")
message(STATUS "Numbers: ${NUMBERS}")

list(REMOVE_AT NUMBERS 0)
message(STATUS "After remove: ${NUMBERS}")

list(LENGTH NUMBERS len)
message(STATUS "List length: ${len}")

set(FLAG ON)
if(FLAG)
    message(STATUS "FLAG is true")
endif()

set(COUNT 0)
if(NOT COUNT)
    message(STATUS "COUNT is false (zero)")
endif()

set(BUILD_TESTS OFF CACHE BOOL "Enable unit tests")

message(STATUS "PATH = $ENV{PATH}")
