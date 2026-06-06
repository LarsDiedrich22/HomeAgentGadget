add_executable(${NAME}
        main.cpp
        DisplayInit.c
        AgentUI.cpp
        SerialProtocol.c
        WS2812.c
        )

file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/generated)
pico_generate_pio_header(${NAME} ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

target_link_libraries(${NAME}
    pico_stdlib
    Config
    LCD
    Touch
    QMI8658
    lvgl
    hardware_clocks
    hardware_rtc
    hardware_adc
    hardware_dma
    hardware_pio
    pico_multicore
    DS3231
)

pico_add_extra_outputs(${NAME})

# USB Serial für Serial Protocol aktivieren
pico_enable_stdio_usb(${NAME} 1)
pico_enable_stdio_uart(${NAME} 0)
