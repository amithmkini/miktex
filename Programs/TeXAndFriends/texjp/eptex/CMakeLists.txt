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

set(MIKTEX_CURRENT_FOLDER "${MIKTEX_IDE_TEX_AND_FRIENDS_FOLDER}/e-pTeX")

set(tex_web_file ${CMAKE_SOURCE_DIR}/${MIKTEX_REL_TEX_DIR}/source/tex.web)

set(miktex_tex_change_files
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

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/miktex-etex.web
    COMMAND
        ${MIKTEX_PREFIX}tie
        -m ${CMAKE_CURRENT_BINARY_DIR}/miktex-etex.web
        ${tex_web_file}
        ${ETEX_CH}
        ${ETEX_MIKTEX_ADAPTER_CH}
        ${miktex_tex_change_files}
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    MAIN_DEPENDENCY
        ${tex_web_file}
    DEPENDS
        ${ETEX_CH}
        ${ETEX_MIKTEX_ADAPTER_CH}
        ${MIKTEX_PREFIX}tie
        ${miktex_tex_change_files}
    VERBATIM
)

# Last but not least: developer's convenience

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/miktex-etex-n.web
    COMMAND
        web-n
        < ${CMAKE_CURRENT_BINARY_DIR}/miktex-etex.web
        > ${CMAKE_CURRENT_BINARY_DIR}/miktex-etex-n.web
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/miktex-etex.web
        web-n
    VERBATIM
)

add_custom_target(eptex-dev ALL
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/miktex-etex-n.web
)

set_property(TARGET eptex-dev PROPERTY FOLDER ${MIKTEX_CURRENT_FOLDER})
