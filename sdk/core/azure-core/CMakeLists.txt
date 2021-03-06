# Copyright (c) Microsoft Corporation. All rights reserved.
# SPDX-License-Identifier: MIT

cmake_minimum_required (VERSION 3.12)
set(TARGET_NAME "azure-core")
project(${TARGET_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(CURL CONFIG)
if(NOT CURL_FOUND)
  find_package(CURL REQUIRED)
endif()

add_library (
  ${TARGET_NAME}
  src/context.cpp
  src/credentials/credentials.cpp
  src/http/http.cpp
  src/http/policy.cpp
  src/http/request.cpp
  src/http/response.cpp
  src/http/curl/curl_transport.cpp
  src/http/winhttp/win_http_transport.cpp
  )

target_include_directories (${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc> $<INSTALL_INTERFACE:include/az_core>)

# make sure that users can consume the project as a library.
add_library (Azure::Core ALIAS ${TARGET_NAME})

target_include_directories(${TARGET_NAME} PUBLIC ${CURL_INCLUDE_DIRS})
target_link_libraries(${TARGET_NAME} PRIVATE CURL::libcurl)
