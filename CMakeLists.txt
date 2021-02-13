#
# BCJSON_SKIP_GTEST_DOWNLOAD    Allows to skip GTEST download (I.E. multiproytect scenario)
#
# === Coverage ===============================================================
# cmake .. -DBCJSON_TESTCOVERAGE=1
# rm *.info
# rm -rf cov-results
# lcov -t "result" -o coverage_01.info -c -d .
# lcov --remove coverage_01.info -o coverage.info \
#     '/usr/include/*' \
#     '*/googletest-src/*'
# genhtml -o cov-results coverage.info
#
#
# === Build Debug ============================================================
# cmake .. -DCMAKE_BUILD_TYPE=Debug
#
cmake_minimum_required (VERSION 3.8)

set (CMAKE_CXX_STANDARD 11)

project (BCJson CXX)
add_library(BCJson INTERFACE)
target_include_directories(BCJson INTERFACE include/)






if (DEFINED BCJSON_SKIP_GTEST_DOWNLOAD)

message(STATUS "BCJSON: GTest download skipped")

else()

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

endif()


# Set coverage options when needed
if (DEFINED BCJSON_TESTCOVERAGE)
set(COVERAGE_OPTIONS "--coverage")
endif (DEFINED BCJSON_TESTCOVERAGE)

enable_testing()
add_executable(
    BCJsonTest

    # Useful testing tools
    test/testTools.cpp

    # The tests
    test/testValue.cpp
    test/testObject.cpp
    test/testArray.cpp
    test/testParser.cpp
    test/testWriter.cpp
    )
target_link_libraries(BCJsonTest gtest_main)
target_link_libraries(BCJsonTest BCJson ${COVERAGE_OPTIONS})
if(MSVC)
  target_compile_options(BCJsonTest PRIVATE /W4 /WX)
else()
  target_compile_options(BCJsonTest PRIVATE -Wall -Wextra -pedantic -Werror ${COVERAGE_OPTIONS})
endif()
add_test(NAME BCJsonTestName COMMAND BCJsonTest)

# Coverage Update target
if (DEFINED BCJSON_TESTCOVERAGE)
add_custom_command(
  OUTPUT BCCoverage
  COMMENT "BCCoverage Command"
  COMMAND rm -f *.info
  COMMAND rm -rf cov-results
  COMMAND ./BCJsonTest
  COMMAND lcov -t "result" -o coverage_01.info -c -d .
  COMMAND lcov --remove coverage_01.info -o coverage.info '/usr/include/*' '*/googletest-src/*'
  COMMAND genhtml -o cov-results coverage.info
)

else ()

add_custom_command(
  OUTPUT BCCoverage
  COMMENT "BCCoverage Command"
  COMMAND echo "Coverage not set. Please rebuild with:"
  COMMAND echo "     cmake ${CMAKE_SOURCE_DIR} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DBCJSON_TESTCOVERAGE=1"
)

endif (DEFINED BCJSON_TESTCOVERAGE)

add_custom_target(coverage
  DEPENDS
    BCCoverage
    BCJsonTest
    )
    
