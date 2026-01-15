# SPDX-License-Identifier: Apache-2.0

if (CONFIG_BOARD_MR_NAVQ95B_MIMX9596_M7_FLASH)
  board_runner_args(pyocd "--target=mimx95_cm7_mx25um" "--frequency=20M")
  math(EXPR VTOR "${CONFIG_FLASH_BASE_ADDRESS} + ${CONFIG_ROM_START_OFFSET}" OUTPUT_FORMAT HEXADECIMAL)
  board_runner_args(pyocd "-O-O vtor=${VTOR}")
  board_runner_args(jlink "--device=cortex-m7")
elseif (CONFIG_SOC_MIMX9596_M7)
  board_runner_args(pyocd "--target=mimx95_cm7")
  board_runner_args(jlink "--device=cortex-m7")
endif()

if (CONFIG_SOF AND CONFIG_BOARD_MR_NAVQ95B_MIMX9596_M7_DDR)
  board_set_rimage_target(imx95)
endif()

include(${ZEPHYR_BASE}/boards/common/pyocd.board.cmake)
include(${ZEPHYR_BASE}/boards/common/jlink.board.cmake)