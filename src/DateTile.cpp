/*
 * DateTile.cpp
 *
 *  Created on: 11 Oct 2024
 *      Author: jondurrant
 */

#include "DateTile.h"
#include "hardware/rtc.h"
#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#include <cstdio>

extern const lv_img_dsc_t Clock;

DateTile::DateTile() {
	// TODO Auto-generated constructor stub

}

DateTile::~DateTile() {
	// TODO Auto-generated destructor stub
}


void DateTile::init(lv_obj_t *tv, uint8_t col_id, uint8_t row_id, lv_dir_t dir){
	 lv_obj_t *tile = lv_tileview_add_tile(tv, col_id, row_id,  dir);

	 //Background
	LV_IMG_DECLARE(Clock);
	pImgBg = lv_img_create(tile);
	lv_img_set_src(pImgBg, &Clock);
	lv_obj_align(pImgBg, LV_ALIGN_CENTER, 0, 0);

	//Calendar
	 xCalendar = lv_calendar_create(tile);
	 lv_obj_set_size(xCalendar, 185, 185);
	 lv_obj_align(xCalendar, LV_ALIGN_CENTER, 0, 0);
	 lv_calendar_header_arrow_create(xCalendar);
	 lv_obj_add_event_cb(xCalendar, eventHandler, LV_EVENT_ALL, this);


	 datetime_t t;
	 rtc_get_datetime(&t);

	 lv_calendar_set_today_date(xCalendar, t.year,  t.month, t.day);
	 lv_calendar_set_showed_date(xCalendar,  t.year,  t.month);
}


void DateTile::eventHandler(lv_event_t * e){
	DateTile *dt = (DateTile *) lv_event_get_user_data(e);
	dt->handler(e);
}


void DateTile::handler(lv_event_t * e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_current_target(e);

    if(code == LV_EVENT_VALUE_CHANGED) {
        lv_calendar_date_t date;
        if(lv_calendar_get_pressed_date(obj, &date)) {
        	lv_calendar_set_today_date(xCalendar, date.year, date.month, date.day);
        	lv_calendar_set_showed_date(xCalendar, date.year, date.month);
        	//printf("Clicked date: %02d.%02d.%d\n", date.day, date.month, date.year);

        	datetime_t t;
			rtc_get_datetime(&t);
			t.year = date.year;
			t.month = date.month;
			t.day = date.day;
			rtc_set_datetime(&t);

			if (pDS3231 != NULL){
				pDS3231->set_date(date.day,   date.month,  date.year);
			}
        }
    }
}


void DateTile::setRTC(DS3231 *p){
	pDS3231 = p;
}
