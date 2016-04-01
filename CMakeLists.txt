#
# Copyright 2016 Roberto García Carvajal
#
# This file is part of CAshley.
# CAshley is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# CAshley is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with CAshley. If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.8)
project(cashley)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi -Wall -Wextra -pipe -pedantic -O3 -std=c++11")

set(SOURCE_FILES
        include/cashley.h
        include/common.h
        include/cache.h
        include/engine.h src/engine.cpp
        include/entity.h src/entity.cpp
        include/component.h src/component.cpp
        include/processor.h src/processor.cpp
        include/exceptions.h src/exceptions.cpp
        include/family.h src/family.cpp
        include/entitylistener.h
        include/inmutablearray.h)

add_library(cashley SHARED ${SOURCE_FILES})
add_library(cashleystatic STATIC ${SOURCE_FILES})

set_target_properties(cashleystatic PROPERTIES OUTPUT_NAME cashley)

install(TARGETS cashley cashleystatic DESTINATION lib)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/" DESTINATION include/cashley)

# uninstall target
configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY)

add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

# Unit tests.
find_package(CxxTest)
if(CXXTEST_FOUND)
    include_directories(${CXXTEST_INCLUDE_DIR})
    enable_testing()
    cxxtest_add_test(unittest_cashley cashley_test.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/cachetests.h
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/componenttests.h
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/enginetests.h
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/entitylistenertests.h
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/entitytests.h
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/familytests.h
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/inmutablearraytests.h
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/processortests.h
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/common.h
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/set_support.h)
    target_link_libraries(unittest_cashley cashley)
endif(CXXTEST_FOUND)

# Doxygen doc.
find_package(Doxygen)
if (DOXYGEN_FOUND)
# Git revision checker
SET (GIT_CMD git)
SET (GIT_ARGS rev-parse HEAD)
EXECUTE_PROCESS (COMMAND ${GIT_CMD} ${GIT_ARGS}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE GITHASHCMD
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )

set (GITHASHCMD "Git revision: ${GITHASHCMD}")

set (DOXYGEN_INPUT "${CMAKE_BINARY_DIR}/doxygen_config")
set (DOXYGEN_OUTPUT "${CMAKE_BINARY_DIR}/doc/")

configure_file (
"${PROJECT_SOURCE_DIR}/doxygen_config.in"
"${PROJECT_BINARY_DIR}/doxygen_config"
)

add_custom_command(
OUTPUT ${DOXYGEN_OUTPUT}
COMMAND ${CMAKE_COMMAND} -E echo_append "Building CAshley Documentation..."
COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
COMMAND ${CMAKE_COMMAND} -E echo "Done."
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
DEPENDS ${DOXYGEN_INPUT}
)

add_custom_target(cashley_doc ALL DEPENDS ${DOXYGEN_OUTPUT})

add_custom_target(cashley_doc_forced
COMMAND ${CMAKE_COMMAND} -E echo_append "Building CAshley Documentation..."
COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
COMMAND ${CMAKE_COMMAND} -E echo "Done."
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

endif (DOXYGEN_FOUND)