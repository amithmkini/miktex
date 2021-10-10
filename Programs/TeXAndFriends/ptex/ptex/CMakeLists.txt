## CMakeLists.txt:
##
## Copyright (C) 2021 Christian Schenk
## 
## This file is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published
## by the Free Software Foundation; either version 2, or (at your
## option) any later version.
## 
## This file is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with this file; if not, write to the Free Software
## Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
## USA.

include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(ptex_target_name ptex)

set(tex_miktex_change_files
  ${MLTEX_MIKTEX_CH}
  ${TEX_MIKTEX_CH}
  ${TEX_MIKTEX_HASH_CH}
  ${TEX_MIKTEX_HYPH_CH}
  ${TEX_MIKTEX_POOL_CH}
  ${TEX_MIKTEX_QUIET_CH}
  ${TEX_MIKTEX_SRC_CH}
  ${TEX_MIKTEX_STAT_CH}
  ${TEX_MIKTEX_WRITE18_CH}
  ${TRACINGSTACKLEVELS_CH}
  ${PARTOKEN_CH}
  ${TEX_MIKTEX_FINISH_CH}
)

set(tex_web_file ${CMAKE_SOURCE_DIR}/${MIKTEX_REL_TEX_DIR}/source/tex.web)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/tex-miktex.web
    COMMAND
        ${MIKTEX_PREFIX}tie
        -m ${CMAKE_CURRENT_BINARY_DIR}/tex-miktex.web
        ${tex_web_file}
        ${tex_miktex_change_file}
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    MAIN_DEPENDENCY
        ${tex_web_file}
    DEPENDS
        ${MIKTEX_PREFIX}tie
        ${tex_miktex_change_files}
    VERBATIM
)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/pre-ptex.web
    COMMAND
        ${MIKTEX_PREFIX}tie
        -m ${CMAKE_CURRENT_BINARY_DIR}/pre-ptex.web
        ${CMAKE_CURRENT_BINARY_DIR}/tex-miktex.web
        ${CMAKE_CURRENT_SOURCE_DIR}/ptex-miktex-adapter.ch
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/ptex-miktex-adapter.ch
        ${MIKTEX_PREFIX}tie
        ${CMAKE_CURRENT_BINARY_DIR}/tex-miktex.web
    VERBATIM
)

add_custom_command(
    OUTPUT
         ${CMAKE_CURRENT_BINARY_DIR}/ptex-final.web
    COMMAND
        ${MIKTEX_PREFIX}tie
        -m  ${CMAKE_CURRENT_BINARY_DIR}/ptex-final.web
        ${CMAKE_CURRENT_BINARY_DIR}/pre-ptex.web
        ${projdir}/source/ptex.ch
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/pre-ptex.web
        ${MIKTEX_PREFIX}tie
        ${projdir}/source/ptex.ch
    VERBATIM
)

set(ptex_web_file ${CMAKE_CURRENT_BINARY_DIR}/ptex-final.web)

create_web_app(pTeX)

if(LINK_EVERYTHING_STATICALLY)
    target_link_libraries(${MIKTEX_PREFIX}ptex
        ptex_kanji
    )
else()
    target_link_libraries(${ptex_target_name}
        PRIVATE
            ptex_kanji
    )
endif()

# Last but not least: developer's convenience

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/tex-miktex-n.web
    COMMAND
        web-n
        < ${CMAKE_CURRENT_BINARY_DIR}/tex-miktex.web
        > ${CMAKE_CURRENT_BINARY_DIR}/tex-miktex-n.web
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/tex-miktex.web
        web-n
    VERBATIM
)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/pre-ptex-n.web
    COMMAND
        web-n
        < ${CMAKE_CURRENT_BINARY_DIR}/pre-ptex.web
        > ${CMAKE_CURRENT_BINARY_DIR}/pre-ptex-n.web
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/pre-ptex.web
        web-n
    VERBATIM
)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/ptex-final-n.web
    COMMAND
        web-n
        < ${CMAKE_CURRENT_BINARY_DIR}/ptex-final.web
        > ${CMAKE_CURRENT_BINARY_DIR}/ptex-final-n.web
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/ptex-final.web
        web-n
    VERBATIM
)

add_custom_target(ptex-dev ALL
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/tex-miktex-n.web        
        ${CMAKE_CURRENT_BINARY_DIR}/ptex-final-n.web
        ${CMAKE_CURRENT_BINARY_DIR}/pre-ptex-n.web
)

set_property(TARGET ptex-dev PROPERTY FOLDER ${MIKTEX_CURRENT_FOLDER})
