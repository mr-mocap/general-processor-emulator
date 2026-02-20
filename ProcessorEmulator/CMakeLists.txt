# CMakeList.txt : CMake project for ProcessorEmulator, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("ProcessorEmulator" LANGUAGES CXX)

set(TEST_REGISTERSET_SRC "${CMAKE_SOURCE_DIR}/test_registerset.cpp")
set(TEST_REGISTERSET_EXE "${CMAKE_BINARY_DIR}/test_registerset")
set(TEST_REGISTERSET_PASS_FILE "${CMAKE_BINARY_DIR}/test_registerset.pass")

set(TEST_INSTRUCTIONSET_SRC "${CMAKE_SOURCE_DIR}/test_instructionset.cpp")
set(TEST_INSTRUCTIONSET_EXE "${CMAKE_BINARY_DIR}/test_instructionset")
set(TEST_INSTRUCTIONSET_PASS_FILE "${CMAKE_BINARY_DIR}/test_instructionset.pass")

set(TEST_CONVERSIONS_SRC "${CMAKE_SOURCE_DIR}/test_conversions.cpp")
set(TEST_CONVERSIONS_EXE "${CMAKE_BINARY_DIR}/test_conversions")
set(TEST_CONVERSIONS_PASS_FILE "${CMAKE_BINARY_DIR}/test_conversions.pass")

set(TEST_IO_SRC "${CMAKE_SOURCE_DIR}/test_io.cpp")
set(TEST_IO_EXE "${CMAKE_BINARY_DIR}/test_io")
set(TEST_IO_PASS_FILE "${CMAKE_BINARY_DIR}/test_io.pass")

set(TEST_DISASSEMBLER_SRC "${CMAKE_SOURCE_DIR}/test_disassembler.cpp")
set(TEST_DISASSEMBLER_EXE "${CMAKE_BINARY_DIR}/test_disassembler")
set(TEST_DISASSEMBLER_PASS_FILE "${CMAKE_BINARY_DIR}/test_disassembler.pass")

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
add_executable (test_registerset "${TEST_REGISTERSET_SRC}" "RegisterSet.cpp")
add_executable (test_instructionset "${TEST_INSTRUCTIONSET_SRC}" "InstructionSet.cpp" "IO.cpp" "Conversions.cpp" "StringProcessing.cpp")
add_executable (test_conversions "${TEST_CONVERSIONS_SRC}" "Conversions.cpp")
add_executable (test_io "${TEST_IO_SRC}" "IO.cpp" "Conversions.cpp")
add_executable (test_disassembler "${TEST_DISASSEMBLER_SRC}" "Disassembler.cpp" "InstructionSet.cpp" "IO.cpp" "Conversions.cpp" "StringProcessing.cpp")

# Add source to this project's executable.
add_executable (ProcessorEmulator "ProcessorEmulator.cpp" "InstructionSet.cpp" "IO.cpp" "Conversions.cpp" "StringProcessing.cpp" "Disassembler.cpp" "RegisterSet.cpp")

# TODO: Add tests and install targets if needed.

add_custom_command(
	OUTPUT ${TEST_REGISTERSET_PASS_FILE}
	COMMAND ${TEST_REGISTERSET_EXE} && ${CMAKE_COMMAND} -E touch ${TEST_REGISTERSET_PASS_FILE}
	DEPENDS ${TEST_REGISTERSET_EXE}
	COMMENT "Running RegisterSet unit tests"
)
add_custom_target(run_test_registerset ALL DEPENDS ${TEST_REGISTERSET_PASS_FILE})

add_custom_command(
	OUTPUT ${TEST_INSTRUCTIONSET_PASS_FILE}
	COMMAND ${TEST_INSTRUCTIONSET_EXE} && ${CMAKE_COMMAND} -E touch ${TEST_INSTRUCTIONSET_PASS_FILE}
	DEPENDS ${TEST_INSTRUCTIONSET_EXE}
	COMMENT "Running InstructionSet unit tests"
)
add_custom_target(run_test_instructionset ALL DEPENDS ${TEST_INSTRUCTIONSET_PASS_FILE})

add_custom_command(
	OUTPUT ${TEST_CONVERSIONS_PASS_FILE}
	COMMAND ${TEST_CONVERSIONS_EXE} && ${CMAKE_COMMAND} -E touch ${TEST_CONVERSIONS_PASS_FILE}
	DEPENDS ${TEST_CONVERSIONS_EXE}
	COMMENT "Running Conversions unit tests"
)
add_custom_target(run_test_conversions ALL DEPENDS ${TEST_CONVERSIONS_PASS_FILE})

add_custom_command(
	OUTPUT ${TEST_IO_PASS_FILE}
	COMMAND ${TEST_IO_EXE} && ${CMAKE_COMMAND} -E touch ${TEST_IO_PASS_FILE}
	DEPENDS ${TEST_IO_EXE}
	COMMENT "Running IO unit tests"
)
add_custom_target(run_test_io ALL DEPENDS ${TEST_IO_PASS_FILE})

add_custom_command(
	OUTPUT ${TEST_DISASSEMBLER_PASS_FILE}
	COMMAND ${TEST_DISASSEMBLER_EXE} && ${CMAKE_COMMAND} -E touch ${TEST_DISASSEMBLER_PASS_FILE}
	DEPENDS ${TEST_DISASSEMBLER_EXE}
	COMMENT "Running Disassembler unit tests"
)
add_custom_target(run_test_disassembler ALL DEPENDS ${TEST_DISASSEMBLER_PASS_FILE})