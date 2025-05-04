# Copyright (c) 2025. VG-Lab/URJC.
#
# Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
#
# This file is part of Mindset <https://gitlab.gmrv.es/g.rial/mindset>
#
# This library is free software; you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License version 3.0 as published
# by the Free Software Foundation.
#
# This library is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

find_package(Catch2 REQUIRED)

include(CTest)
include(Catch)

project(mindset-tests)
set(CMAKE_CXX_STANDARD 20)

add_executable(mindset-tests brion.cpp swc.cpp snudda.cpp)

add_dependencies(mindset-tests mindset)

target_link_libraries(mindset-tests PRIVATE mindset Catch2::Catch2WithMain)

catch_discover_tests(mindset-tests)

add_custom_command(
        TARGET mindset-tests PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/data
        ${CMAKE_CURRENT_BINARY_DIR}/data
)