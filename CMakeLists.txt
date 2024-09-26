#
# Copyright 2024 Grzegorz Grzybek
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

cmake_minimum_required(VERSION 3.28)

project(sma VERSION 1.0.0
            DESCRIPTION "Sourcemap analyzer"
            HOMEPAGE_URL "https://github.com/grgrzybek/sourcemap-analyzer")

set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/_install")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O0 -g")

set(commands sma)

configure_file(config.h.in "${CMAKE_INSTALL_PREFIX}/include/config.h")
include_directories("${CMAKE_INSTALL_PREFIX}/include")
include_directories("${CMAKE_SOURCE_DIR}/include")

#find_package(SDL2 REQUIRED)
#include_directories(${SDL2_INCLUDE_DIRS})

#find_package(PNG REQUIRED)
#include_directories(${PNG_INCLUDE_DIRS})

add_library(utils SHARED src/utils/utils.cc)

foreach(cmd ${commands})
    add_executable(${cmd})
    target_sources(${cmd} PUBLIC src/${cmd}.cc)
    target_link_libraries(${cmd} utils)
endforeach(cmd)

install(TARGETS ${commands} RUNTIME DESTINATION bin)
