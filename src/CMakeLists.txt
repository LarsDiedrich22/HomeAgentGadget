add_executable(${NAME}
        LVGL_example.c
        main.cpp
            
        Present.c
        Wolf.c
        NorthernLights.c
        Dressing.c
        Skating.c
        Krampus.c 
        SaintNicolas.c
        ChristmasTree.c
        Presents.c
	    Snowball.c
        GingerbreadHouse.c
        Wreath.c
	    Kittens.c
	    RobotsSnowman1.c
	    Sweets.c
	    Sheppard.c
	    Chestnuts.c
        Canes.c
       	RobotsSnowman2.c
       	Stag.c
       	Shoppers.c    	   
        Dinner.c
        Solstice.c
        Pollar.c
        Exchange.c
        Sley.c
     
        
        Day1.c
        Day2.c
        Day3.c
        Day4.c
        Day5.c
        Day6.c
        Day7.c
        Day8.c
        Day9.c
        Day10.c
        Day11.c
        Day12.c
       
        Clock.c
    
        AdventTile.cpp
        DateTile.cpp
        TimeTile.cpp
        
        ledChain.cpp
        ColourAnim.cpp
        ColourFadeInOut.cpp
        Tree.cpp
        ColourBlend.cpp
        AdventTrees.cpp
        )


file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/generated)
pico_generate_pio_header(${NAME} ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

# Pull in our pico_stdlib which pulls in commonly used features
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
	hardware_rtc
	DS3231
)

# create map/bin/hex file etc.
pico_add_extra_outputs(${NAME})

# enable usb output, disable uart output
pico_enable_stdio_usb(${NAME} 0)
pico_enable_stdio_uart(${NAME} 0)
target_compile_definitions(${NAME} PRIVATE
    PICO_DEFAULT_UART_RX_PIN=17
    PICO_DEFAULT_UART_TX_PIN=16
)

