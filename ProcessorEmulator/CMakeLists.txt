# CMakeList.txt : CMake project for ProcessorEmulator, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("ProcessorEmulator" LANGUAGES CXX)

set(TEST_REGISTERSET_SRC "${CMAKE_SOURCE_DIR}/test_registerset.cpp")
set(TEST_REGISTERSET_EXE "${CMAKE_BINARY_DIR}/test_registerset")
set(TEST_REGISTERSET_PASS_FILE "${CMAKE_BINARY_DIR}/test_registerset.pass")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)


# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Add unit tests
add_executable (test_registerset "${TEST_REGISTERSET_SRC}" "RegisterSet.hpp" "RegisterSet.cpp")

# Add source to this project's executable.
add_executable (ProcessorEmulator "ProcessorEmulator.cpp" "ProcessorEmulator.h" "InstructionSet.cpp")

# TODO: Add tests and install targets if needed.

add_custom_command(
	OUTPUT ${TEST_REGISTERSET_PASS_FILE}
	COMMAND ${TEST_REGISTERSET_EXE} && ${CMAKE_COMMAND} -E touch ${TEST_REGISTERSET_PASS_FILE}
	DEPENDS ${TEST_REGISTERSET_EXE}
	COMMENT "Running RegisterSet unit tests"
)

add_custom_target(run_test_registerset ALL DEPENDS ${TEST_REGISTERSET_PASS_FILE})

