#
# Copyright (c) 2020 Jamie Kenyon. All Rights Reserved.
#

cmake_minimum_required(VERSION 3.17.0 FATAL_ERROR)
project("PAK" VERSION 0.0.1.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_subdirectory("src")
