
#include "pico/stdlib.h"
#include <stdio.h>

extern "C"{
#include "LCD_test.h"
#include "DEV_Config.h"
#include "LCD_1in28.h"
#include "CST816S.h"
#include "QMI8658.h"
#include "LVGL_example.h"
}

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"
#include "pico/multicore.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"

#include "AdventTile.h"
#include "DateTile.h"
#include "TimeTile.h"
#include "AdventTrees.h"
#include "DS3231.hpp"

#define LEDS_PIN  18 //15
#define LEDS_LENGTH 12
#define IS_RGBW false
#define DEMO_GP 28
#define DEMO_LED 27

#define RTC 1
#define RTC_SDA 16
#define RTC_SCL 17
#define RTC_I2C i2c0


/***
 * Core 1 code to run the tree animations
 */
void core1Entry(){
	uint32_t u;
	uint8_t d;

	printf("Core 1 start\n");

	AdventTrees advent(LEDS_PIN);
	advent.setDay(0);

	for (;;){

		if (multicore_fifo_pop_timeout_us(5, &u)){
			d = u & 0xFF;
			advent.setDay(d);
		}

		for (float f=0.0; f <= 1.0; f+=0.05){
			advent.animate(f);
			sleep_ms(100);
		}

	}

}



/***
 * Init the display
 * @return
 */
bool initDisplay(){
    if (DEV_Module_Init() != 0){
        return false;
    }

    // /*Init LCD*/
    LCD_1IN28_Init(HORIZONTAL);
    LCD_1IN28_Clear(WHITE);
    DEV_SET_PWM(100);

    // /*Init touch screen*/
    CST816S_init(CST816S_Point_Mode);

    // /*Init LVGL*/
    LVGL_Init();

    //Widgets_Init();
    return true;
}

#ifdef RTC
	DS3231 ds3231(RTC_I2C,  RTC_SDA,  RTC_SCL);
#endif
void initRTC(){
	datetime_t t;
#ifdef RTC
	t.year 			= 	ds3231.get_year();
	t.month 		= 	ds3231.get_mon();
	t.day 			= ds3231.get_day();
	t.hour			= ds3231.get_hou();
	t.min			= ds3231.get_min();
	t.sec			= ds3231.get_sec();
	t.dotw		= 0;
#else
	 t = {
	            .year  = 2024,
	            .month = 10,
	            .day   = 11,
	            .dotw  = 5, // 0 is Sunday, so 5 is Friday
	            .hour  = 12,
	            .min   = 00,
	            .sec   = 00
	    };
#endif

	    // Start the RTC
	    rtc_init();
	    rtc_set_datetime(&t);
}

uint8_t getDay(){
	uint8_t today = 0;
	datetime_t t ;
	if ( rtc_get_datetime(&t)){
		if (t.month == 12){
			today = t.day;
		} else if (
				(t.month == 1) &&
				(t.day <= 5)){
			today = t.day + 31;
		}
	}
	return today;
}


void initDemo(){
	gpio_init(DEMO_GP);
	gpio_pull_up (DEMO_GP);
	gpio_set_dir(DEMO_GP, GPIO_IN);

	gpio_init(DEMO_LED);
	gpio_set_dir(DEMO_LED, GPIO_OUT);
	gpio_put(DEMO_LED, false);
}

bool isDemo(){
	uint8_t c = gpio_get(DEMO_GP);
	if (c==0){
		gpio_put(DEMO_LED, true);
	} else {
		gpio_put(DEMO_LED, false);
	}
	return (c == 0);
}


/***
 * This will be run on core0
 */
void core2Entry(){
	//bool demo = false;
	uint32_t nextMove = 0;

    lv_obj_t *tv = lv_tileview_create(lv_scr_act());
    lv_obj_set_scrollbar_mode(tv,  LV_SCROLLBAR_MODE_OFF);

	AdventTile adv ;
	adv.init(tv, 0, 0, LV_DIR_ALL);
	adv.setToday(0);

	DateTile dateTile;
	dateTile.init(tv, 0, 1, LV_DIR_ALL);
#ifdef RTC
	dateTile.setRTC(&ds3231);
#endif

	TimeTile timeTile;
	timeTile.init(tv, 0, 2, LV_DIR_ALL);
#ifdef RTC
	timeTile.setRTC(&ds3231);
#endif


	 for (;;){
		 lv_task_handler();

		 uint32_t now =  to_ms_since_boot (get_absolute_time());
		 if (now > nextMove){
			 uint8_t d = adv.getCurrent();
			 d++;
			 if (d > adv.getToday()){
				 d = 0;
			 }
			 adv.setCurrent(d);

			 if (isDemo()){
				 d = adv.getToday();
				 d++;
				 if (d > 36){
					 d = 0;
				 }
				 adv.setToday(d);
				 uint32_t dd = d;
				 multicore_fifo_push_timeout_us (dd,  5);
			 } else {
				 d = getDay();
				 if ( d != adv.getToday()) {
						 adv.setToday(d);
						 adv.setCurrent(d);
						 uint32_t dd = d;
						 multicore_fifo_push_timeout_us (dd,  5);
				 }
			 }

			 nextMove = now + 10000;
		 }


		DEV_Delay_ms(5);
	 }

	 DEV_Module_Exit();


}



int main(void)
{
	stdio_init_all();
	sleep_ms(2000);
	printf("GO\n");

	initRTC();

	initDemo();
	initDisplay();
	printf("Disp Init\n");
	sleep_ms(5);

	multicore_reset_core1();
	multicore_launch_core1(core1Entry);


	core2Entry();



    return 0;
}
