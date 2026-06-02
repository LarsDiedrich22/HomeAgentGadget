/*
 * DateTile.h
 *
 *  Created on: 11 Oct 2024
 *      Author: jondurrant
 */

#ifndef SRC_DATETILE_H_
#define SRC_DATETILE_H_


#include "lvgl.h"
#include "extra/widgets/calendar/lv_calendar.h"
#include "DS3231.hpp"

class DateTile {
public:
	DateTile();
	virtual ~DateTile();

	/***
	 *
	 * 	@param tv - Tile Viewer
	 * @param col_id
	 * @param row_id
	 * @param dir
	 */
	void init(lv_obj_t *tv, uint8_t col_id, uint8_t row_id, lv_dir_t dir);

	/***
	 * Optionally set a RTC device
	 * @param p
	 */
	void setRTC(DS3231 *p);

private:
	static void eventHandler(lv_event_t * e);
	void handler(lv_event_t * e);


	 lv_obj_t  * xCalendar;
	 lv_obj_t * pImgBg ;

	 DS3231 *pDS3231 = NULL;


};

#endif /* SRC_DATETILE_H_ */
