# MCUboot board configuration for MR-MCXN-T1
# Usage: west build -b mr_mcxn_t1/mcxn947/cpu0 bootloader/mcuboot/boot/zephyr -- -C <path>/mcuboot.cmake

get_filename_component(_mcuboot_board_dir "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

set(DTC_OVERLAY_FILE "${_mcuboot_board_dir}/mr_mcxn_t1_mcxn947_cpu0.overlay" CACHE STRING "" FORCE)
set(EXTRA_CONF_FILE "${_mcuboot_board_dir}/mr_mcxn_t1_mcxn947_cpu0.conf" CACHE STRING "" FORCE)
