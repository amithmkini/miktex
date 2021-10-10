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

set(ptftopl_target_name ptftopl)

set(tftopl_miktex_change_file ${CMAKE_SOURCE_DIR}/${MIKTEX_REL_TEXWARE_DIR}/tftopl-miktex.ch)
set(tftopl_web_file ${CMAKE_SOURCE_DIR}/${MIKTEX_REL_TEXWARE_DIR}/source/tftopl.web)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/tftopl-miktex.web
    COMMAND
        ${MIKTEX_PREFIX}tie
        -m ${CMAKE_CURRENT_BINARY_DIR}/tftopl-miktex.web
        ${tftopl_web_file}
        ${tftopl_miktex_change_file}
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    MAIN_DEPENDENCY
        ${tftopl_web_file}
    DEPENDS
        ${MIKTEX_PREFIX}tie
        ${tftopl_miktex_change_file}
    VERBATIM
)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/pre-ptftopl.web
    COMMAND
        ${MIKTEX_PREFIX}tie
        -m ${CMAKE_CURRENT_BINARY_DIR}/pre-ptftopl.web
        ${CMAKE_CURRENT_BINARY_DIR}/tftopl-miktex.web
        ${CMAKE_CURRENT_SOURCE_DIR}/ptftopl-miktex-adapter.ch
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/ptftopl-miktex-adapter.ch
        ${MIKTEX_PREFIX}tie
        ${CMAKE_CURRENT_BINARY_DIR}/tftopl-miktex.web
    VERBATIM
)

add_custom_command(
    OUTPUT
         ${CMAKE_CURRENT_BINARY_DIR}/ptftopl-final.web
    COMMAND
        ${MIKTEX_PREFIX}tie
        -m  ${CMAKE_CURRENT_BINARY_DIR}/ptftopl-final.web
        ${CMAKE_CURRENT_BINARY_DIR}/pre-ptftopl.web
        ${projdir}/source/ptftopl.ch
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/pre-ptftopl.web
        ${MIKTEX_PREFIX}tie
        ${projdir}/source/ptftopl.ch
    VERBATIM
)

set(ptftopl_web_file ${CMAKE_CURRENT_BINARY_DIR}/ptftopl-final.web)

create_web_app(pTFtoPL)

if(LINK_EVERYTHING_STATICALLY)
    target_link_libraries(${MIKTEX_PREFIX}ptftopl
        ptex_kanji
    )
else()
    target_link_libraries(${ptftopl_target_name}
        PRIVATE
            ptex_kanji
    )
endif()

# Last but not least: developer's convenience

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/tftopl-miktex-n.web
    COMMAND
        web-n
        < ${CMAKE_CURRENT_BINARY_DIR}/tftopl-miktex.web
        > ${CMAKE_CURRENT_BINARY_DIR}/tftopl-miktex-n.web
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/tftopl-miktex.web
        web-n
    VERBATIM
)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/pre-ptftopl-n.web
    COMMAND
        web-n
        < ${CMAKE_CURRENT_BINARY_DIR}/pre-ptftopl.web
        > ${CMAKE_CURRENT_BINARY_DIR}/pre-ptftopl-n.web
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/pre-ptftopl.web
        web-n
    VERBATIM
)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/ptftopl-final-n.web
    COMMAND
        web-n
        < ${CMAKE_CURRENT_BINARY_DIR}/ptftopl-final.web
        > ${CMAKE_CURRENT_BINARY_DIR}/ptftopl-final-n.web
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/ptftopl-final.web
        web-n
    VERBATIM
)

add_custom_target(ptftopl-dev ALL
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/tftopl-miktex-n.web        
        ${CMAKE_CURRENT_BINARY_DIR}/ptftopl-final-n.web
        ${CMAKE_CURRENT_BINARY_DIR}/pre-ptftopl-n.web
)

set_property(TARGET ptftopl-dev PROPERTY FOLDER ${MIKTEX_CURRENT_FOLDER})
