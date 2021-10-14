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

if(LINK_EVERYTHING_STATICALLY)
  use_static_crt()
endif()

default_char_type_is_unsigned()

include_directories(BEFORE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/${MIKTEX_REL_SYNCTEX_INCLUDE_DIR}
)

add_definitions(
    -DHAVE_C4P_PRE_H
    -DSYNCTEX_ENGINE_H="synctex-ptex.h"
    -DTeX
    -D__SyncTeX__
    -DpTeX
)

list(APPEND C4P_FLAGS
    --auto-exit=10
    --chars-are-unsigned
    --emit-optimize-pragmas
)

set(ptex_target_name ptex)

set(tex_miktex_change_files
    ${MLTEX_MIKTEX_CH}
    ${TEX_MIKTEX_CONSTANTS_CH}
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

set(ptex_ch_synctex
	${CMAKE_SOURCE_DIR}/${MIKTEX_REL_SYNCTEX_SOURCE_DIR}/synctex-def.ch0
	${CMAKE_SOURCE_DIR}/${MIKTEX_REL_SYNCTEX_SOURCE_DIR}/synctex-p-mem.ch0
	${CMAKE_SOURCE_DIR}/${MIKTEX_REL_SYNCTEX_SOURCE_DIR}/synctex-mem.ch0
	${CMAKE_SOURCE_DIR}/${MIKTEX_REL_SYNCTEX_SOURCE_DIR}/synctex-p-mem.ch1
	${CMAKE_SOURCE_DIR}/${MIKTEX_REL_SYNCTEX_SOURCE_DIR}/synctex-p-rec.ch0
	${CMAKE_SOURCE_DIR}/${MIKTEX_REL_SYNCTEX_SOURCE_DIR}/synctex-rec.ch0
	${CMAKE_SOURCE_DIR}/${MIKTEX_REL_SYNCTEX_SOURCE_DIR}/synctex-rec.ch1
	${CMAKE_SOURCE_DIR}/${MIKTEX_REL_SYNCTEX_SOURCE_DIR}/synctex-rec.ch2
	${CMAKE_SOURCE_DIR}/${MIKTEX_REL_SYNCTEX_SOURCE_DIR}/synctex-p-rec.ch1
)

set(tex_web_file ${CMAKE_SOURCE_DIR}/${MIKTEX_REL_TEX_DIR}/source/tex.web)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/tex-miktex.web
    COMMAND
        ${MIKTEX_PREFIX}tie
        -m ${CMAKE_CURRENT_BINARY_DIR}/tex-miktex.web
        ${tex_web_file}
        ${tex_miktex_change_files}
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
        ${CMAKE_CURRENT_SOURCE_DIR}/miktex-ptex-adapter.ch
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/miktex-ptex-adapter.ch
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
        ${projdir}/source/ptex-base.ch
        ${ptex_ch_synctex}
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/pre-ptex.web
        ${MIKTEX_PREFIX}tie
        ${projdir}/source/ptex-base.ch
        ${ptex_ch_synctex}
    VERBATIM
)

list(APPEND ${ptex_target_name}_sources
    ${projdir}/source/ptexextra.c
    ${projdir}/source/ptexextra.h
)

set_source_files_properties(
    ${projdir}/source/ptexextra.c
    PROPERTIES LANGUAGE CXX
)

list(APPEND ${ptex_target_name}_sources
    ${CMAKE_CURRENT_BINARY_DIR}/ptex_pool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ptex-miktex.cpp
)

list(APPEND ${ptex_target_name}_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/c4p_pre.h
    ${CMAKE_CURRENT_SOURCE_DIR}/miktex-first.h
)

list(APPEND ${ptex_target_name}_sources
    ${CMAKE_SOURCE_DIR}/${MIKTEX_REL_SYNCTEX_SOURCE_DIR}/synctex-common.h
    ${CMAKE_SOURCE_DIR}/${MIKTEX_REL_SYNCTEX_SOURCE_DIR}/synctex-pdftex.h
    ${CMAKE_SOURCE_DIR}/${MIKTEX_REL_SYNCTEX_SOURCE_DIR}/synctex.c
    ${CMAKE_SOURCE_DIR}/${MIKTEX_REL_SYNCTEX_SOURCE_DIR}/synctex.h
)

set_source_files_properties(
    ${CMAKE_SOURCE_DIR}/${MIKTEX_REL_SYNCTEX_SOURCE_DIR}/synctex.c
    PROPERTIES LANGUAGE CXX
)

file(READ ${MIKTEX_DYN_TEX_SCRIPT} tex_dyn_sed_contents)

if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/dyn.sed)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dyn.sed "${tex_dyn_sed_contents}")
endif()

set(ptex_web_file ${CMAKE_CURRENT_BINARY_DIR}/ptex-final.web)
set(ptex_header_file ${CMAKE_CURRENT_BINARY_DIR}/ptexd.h)

create_web_app(pTeX)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/ptex_pool.cpp
    COMMAND
        inipool
        ${CMAKE_CURRENT_BINARY_DIR}/ptex.pool
        ptex-miktex.h
        ${ptex_progclass}
        ${ptex_prog}
        > ${CMAKE_CURRENT_BINARY_DIR}/ptex_pool.cpp
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        inipool
        ${CMAKE_CURRENT_BINARY_DIR}/ptex.pool
)

if(LINK_EVERYTHING_STATICALLY)
    target_link_libraries(${MIKTEX_PREFIX}ptex
        ${kpsemu_lib_name}
        ${w2cemu_lib_name}
        ${web2c_sources_lib_name}
        ptex_kanji
    )
    if(MIKTEX_NATIVE_WINDOWS)
        target_link_libraries(${MIKTEX_PREFIX}ptex
            ${utf8wrap_lib_name}
        )
    endif()
else()
    target_link_libraries(${ptex_target_name}
        PRIVATE
            ${kpsemu_dll_name}
            ${w2cemu_dll_name}
            ${web2c_sources_dll_name}
            ptex_kanji
    )
    if(MIKTEX_NATIVE_WINDOWS)
        target_link_libraries(${ptex_target_name}
            PRIVATE
                ${utf8wrap_dll_name}
        )
    endif()
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
