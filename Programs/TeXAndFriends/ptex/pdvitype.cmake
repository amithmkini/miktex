## pdvitype.cmake:
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

set(pdvitype_target_name pdvitype)

set(dvitype_miktex_change_file ${CMAKE_SOURCE_DIR}/${MIKTEX_REL_TEXWARE_DIR}/dvitype-miktex.ch)
set(dvitype_web_file ${CMAKE_SOURCE_DIR}/${MIKTEX_REL_TEXWARE_DIR}/source/dvitype.web)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/dvitype-miktex.web
    COMMAND
        ${MIKTEX_PREFIX}tie
        -m ${CMAKE_CURRENT_BINARY_DIR}/dvitype-miktex.web
        ${dvitype_web_file}
        ${dvitype_miktex_change_file}
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    MAIN_DEPENDENCY
        ${dvitype_web_file}
    DEPENDS
        ${MIKTEX_PREFIX}tie
        ${dvitype_miktex_change_file}
    VERBATIM
)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/pre-pdvitype.web
    COMMAND
        ${MIKTEX_PREFIX}tie
        -m ${CMAKE_CURRENT_BINARY_DIR}/pre-pdvitype.web
        ${CMAKE_CURRENT_BINARY_DIR}/dvitype-miktex.web
        ${CMAKE_CURRENT_SOURCE_DIR}/pdvitype-miktex-adapter.ch
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/pdvitype-miktex-adapter.ch
        ${MIKTEX_PREFIX}tie
        ${CMAKE_CURRENT_BINARY_DIR}/dvitype-miktex.web
    VERBATIM
)

add_custom_command(
    OUTPUT
         ${CMAKE_CURRENT_BINARY_DIR}/pdvitype-final.web
    COMMAND
        ${MIKTEX_PREFIX}tie
        -m  ${CMAKE_CURRENT_BINARY_DIR}/pdvitype-final.web
        ${CMAKE_CURRENT_BINARY_DIR}/pre-pdvitype.web
        ${CMAKE_CURRENT_SOURCE_DIR}/source/pdvitype.ch
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/pre-pdvitype.web
        ${CMAKE_CURRENT_SOURCE_DIR}/pdvitype-miktex.ch
        ${MIKTEX_PREFIX}tie
    VERBATIM
)

set(pdvitype_web_file ${CMAKE_CURRENT_BINARY_DIR}/pdvitype-final.web)

create_web_app(pDVItype)

if(LINK_EVERYTHING_STATICALLY)
    target_link_libraries(${MIKTEX_PREFIX}pdvitype
        ptex_kanji
    )
else()
    target_link_libraries(${pdvitype_target_name}
        PRIVATE
            ptex_kanji
    )
endif()

# Last but not least: developer's convenience

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/dvitype-miktex-n.web
    COMMAND
        web-n
        < ${CMAKE_CURRENT_BINARY_DIR}/dvitype-miktex.web
        > ${CMAKE_CURRENT_BINARY_DIR}/dvitype-miktex-n.web
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/dvitype-miktex.web
        web-n
    VERBATIM
)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/pre-pdvitype-n.web
    COMMAND
        web-n
        < ${CMAKE_CURRENT_BINARY_DIR}/pre-pdvitype.web
        > ${CMAKE_CURRENT_BINARY_DIR}/pre-pdvitype-n.web
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/pre-pdvitype.web
        web-n
    VERBATIM
)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/pdvitype-final-n.web
    COMMAND
        web-n
        < ${CMAKE_CURRENT_BINARY_DIR}/pdvitype-final.web
        > ${CMAKE_CURRENT_BINARY_DIR}/pdvitype-final-n.web
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/pdvitype-final.web
        web-n
    VERBATIM
)
