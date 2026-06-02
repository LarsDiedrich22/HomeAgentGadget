# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.12)

# Change your executable name to something creative!
set(NAME Advent) # <-- Name your project/executable here!
set(PICO_BOARD pico CACHE STRING "Board type")

include(pico_sdk_import.cmake)

# Gooey boilerplate
project(${NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_CXX_ENABLE_EXCEPTIONS 1)
set(PICO_CXX_ENABLE_RTTI 1)

# Initialize the SDK
pico_sdk_init()

set(LIB_DIR "${CMAKE_CURRENT_LIST_DIR}/lib")
#set(LIB_DIR "${CMAKE_CURRENT_LIST_DIR}/lib")

message(LIB_DIR="${LIB_DIR}")

include(${LIB_DIR}/Config/CMakeLists.txt)
include(${LIB_DIR}/LCD_LVGL/CMakeLists.txt)
include(${LIB_DIR}/QMI8658/CMakeLists.txt)
include(${LIB_DIR}/Touch/CMakeLists.txt)

set(DS3231_LIB_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/RP2040DS3231RTC" CACHE PATH "Path to the SimpleDS3231 library")
include(${LIB_DIR}/RP2040DS3231RTC/ds3231_lib_import.cmake)

set(LV_CONF_DIR "${CMAKE_CURRENT_LIST_DIR}/port/lvgl/")
set(LVGL_DIR "${LIB_DIR}/lvgl/")
include(lvgl.cmake)

add_subdirectory(src)

#Set up files for the release packages
install(CODE "execute_process(COMMAND $ENV{HOME}/bin/picoDeploy.sh ${CMAKE_CURRENT_BINARY_DIR}/src/${NAME}.elf)")

# Set up files for the release packages
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/src/${NAME}.uf2
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_GENERATOR "ZIP" "TGZ")
include(CPack)
