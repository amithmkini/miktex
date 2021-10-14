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

set(ppltotf_target_name ppltotf)

set(pltotf_miktex_change_file ${CMAKE_SOURCE_DIR}/${MIKTEX_REL_TEXWARE_DIR}/pltotf-miktex.ch)
set(pltotf_web_file ${CMAKE_SOURCE_DIR}/${MIKTEX_REL_TEXWARE_DIR}/source/pltotf.web)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/pltotf-miktex.web
    COMMAND
        ${MIKTEX_PREFIX}tie
        -m ${CMAKE_CURRENT_BINARY_DIR}/pltotf-miktex.web
        ${pltotf_web_file}
        ${pltotf_miktex_change_file}
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    MAIN_DEPENDENCY
        ${pltotf_web_file}
    DEPENDS
        ${MIKTEX_PREFIX}tie
        ${pltotf_miktex_change_file}
    VERBATIM
)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/pre-ppltotf.web
    COMMAND
        ${MIKTEX_PREFIX}tie
        -m ${CMAKE_CURRENT_BINARY_DIR}/pre-ppltotf.web
        ${CMAKE_CURRENT_BINARY_DIR}/pltotf-miktex.web
        ${CMAKE_CURRENT_SOURCE_DIR}/ppltotf-miktex-adapter.ch
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/ppltotf-miktex-adapter.ch
        ${MIKTEX_PREFIX}tie
        ${CMAKE_CURRENT_BINARY_DIR}/pltotf-miktex.web
    VERBATIM
)

add_custom_command(
    OUTPUT
         ${CMAKE_CURRENT_BINARY_DIR}/ppltotf-final.web
    COMMAND
        ${MIKTEX_PREFIX}tie
        -m  ${CMAKE_CURRENT_BINARY_DIR}/ppltotf-final.web
        ${CMAKE_CURRENT_BINARY_DIR}/pre-ppltotf.web
        ${projdir}/source/ppltotf.ch
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/pre-ppltotf.web
        ${MIKTEX_PREFIX}tie
        ${projdir}/source/ppltotf.ch
    VERBATIM
)

set(ppltotf_web_file ${CMAKE_CURRENT_BINARY_DIR}/ppltotf-final.web)

create_web_app(pPLtoTF)

if(LINK_EVERYTHING_STATICALLY)
    target_link_libraries(${MIKTEX_PREFIX}ppltotf
        ptex_kanji
    )
else()
    target_link_libraries(${ppltotf_target_name}
        PRIVATE
            ptex_kanji
    )
endif()

# Last but not least: developer's convenience

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/pltotf-miktex-n.web
    COMMAND
        web-n
        < ${CMAKE_CURRENT_BINARY_DIR}/pltotf-miktex.web
        > ${CMAKE_CURRENT_BINARY_DIR}/pltotf-miktex-n.web
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/pltotf-miktex.web
        web-n
    VERBATIM
)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/pre-ppltotf-n.web
    COMMAND
        web-n
        < ${CMAKE_CURRENT_BINARY_DIR}/pre-ppltotf.web
        > ${CMAKE_CURRENT_BINARY_DIR}/pre-ppltotf-n.web
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/pre-ppltotf.web
        web-n
    VERBATIM
)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/ppltotf-final-n.web
    COMMAND
        web-n
        < ${CMAKE_CURRENT_BINARY_DIR}/ppltotf-final.web
        > ${CMAKE_CURRENT_BINARY_DIR}/ppltotf-final-n.web
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/ppltotf-final.web
        web-n
    VERBATIM
)

add_custom_target(ppltotf-dev ALL
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/pltotf-miktex-n.web        
        ${CMAKE_CURRENT_BINARY_DIR}/ppltotf-final-n.web
        ${CMAKE_CURRENT_BINARY_DIR}/pre-ppltotf-n.web
)

set_property(TARGET ppltotf-dev PROPERTY FOLDER ${MIKTEX_CURRENT_FOLDER})
