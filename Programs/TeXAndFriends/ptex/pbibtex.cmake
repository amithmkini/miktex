## pbibtex.cmake:
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

set(pbibtex_target_name pbibtex)

set(bibtex_miktex_change_file ${CMAKE_SOURCE_DIR}/${MIKTEX_REL_BIBTEX_DIR}/bibtex-miktex.ch)
set(bibtex_web_file ${CMAKE_SOURCE_DIR}/${MIKTEX_REL_BIBTEX_DIR}/source/bibtex.web)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/bibtex-miktex.web
    COMMAND
        ${MIKTEX_PREFIX}tie
        -m ${CMAKE_CURRENT_BINARY_DIR}/bibtex-miktex.web
        ${bibtex_web_file}
        ${bibtex_miktex_change_file}
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    MAIN_DEPENDENCY
        ${bibtex_web_file}
    DEPENDS
        ${MIKTEX_PREFIX}tie
        ${bibtex_miktex_change_file}
    VERBATIM
)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/pre-pbibtex.web
    COMMAND
        ${MIKTEX_PREFIX}tie
        -m ${CMAKE_CURRENT_BINARY_DIR}/pre-pbibtex.web
        ${CMAKE_CURRENT_BINARY_DIR}/bibtex-miktex.web
        ${CMAKE_CURRENT_SOURCE_DIR}/pbibtex-miktex-adapter.ch
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/pbibtex-miktex-adapter.ch
        ${MIKTEX_PREFIX}tie
        ${CMAKE_CURRENT_BINARY_DIR}/bibtex-miktex.web
    VERBATIM
)

add_custom_command(
    OUTPUT
         ${CMAKE_CURRENT_BINARY_DIR}/pbibtex-final.web
    COMMAND
        ${MIKTEX_PREFIX}tie
        -m  ${CMAKE_CURRENT_BINARY_DIR}/pbibtex-final.web
        ${CMAKE_CURRENT_BINARY_DIR}/pre-pbibtex.web
        ${CMAKE_CURRENT_SOURCE_DIR}/source/pbibtex.ch
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/pre-pbibtex.web
        ${CMAKE_CURRENT_SOURCE_DIR}/pbibtex-miktex.ch
        ${MIKTEX_PREFIX}tie
    VERBATIM
)

set(pbibtex_web_file ${CMAKE_CURRENT_BINARY_DIR}/pbibtex-final.web)

create_web_app(pBibTeX)

if(LINK_EVERYTHING_STATICALLY)
    target_link_libraries(${MIKTEX_PREFIX}pbibtex
        ptex_kanji
    )
else()
    target_link_libraries(${pbibtex_target_name}
        PRIVATE
            ptex_kanji
    )
endif()

# Last but not least: developer's convenience

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/bibtex-miktex-n.web
    COMMAND
        web-n
        < ${CMAKE_CURRENT_BINARY_DIR}/bibtex-miktex.web
        > ${CMAKE_CURRENT_BINARY_DIR}/bibtex-miktex-n.web
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/bibtex-miktex.web
        web-n
    VERBATIM
)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/pre-pbibtex-n.web
    COMMAND
        web-n
        < ${CMAKE_CURRENT_BINARY_DIR}/pre-pbibtex.web
        > ${CMAKE_CURRENT_BINARY_DIR}/pre-pbibtex-n.web
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/pre-pbibtex.web
        web-n
    VERBATIM
)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/pbibtex-final-n.web
    COMMAND
        web-n
        < ${CMAKE_CURRENT_BINARY_DIR}/pbibtex-final.web
        > ${CMAKE_CURRENT_BINARY_DIR}/pbibtex-final-n.web
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/pbibtex-final.web
        web-n
    VERBATIM
)

add_custom_target(pbibtex-dev ALL
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/bibtex-miktex-n.web        
        ${CMAKE_CURRENT_BINARY_DIR}/pbibtex-final-n.web
        ${CMAKE_CURRENT_BINARY_DIR}/pre-pbibtex-n.web
)

set_property(TARGET pbibtex-dev PROPERTY FOLDER ${MIKTEX_CURRENT_FOLDER})
