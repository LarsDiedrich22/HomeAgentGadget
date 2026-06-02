/*
 * TimeTile.h
 *
 *  Created on: 11 Oct 2024
 *      Author: jondurrant
 */

#ifndef SRC_TIMETILE_H_
#define SRC_TIMETILE_H_

#include "lvgl.h"
#include "hardware/rtc.h"
#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#include "DS3231.hpp"

enum TimeSelect {
	NothingSelected,
	HourSelected,
	MinSelected,
	SecSelected
};

class TimeTile {
public:
	TimeTile();
	virtual ~TimeTile();

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
	void initStyles();

	static void eventSelectHandler(lv_event_t * e);
	void selectHandler(lv_event_t * e);

	static void eventIncHandler(lv_event_t * e);
	void incHandler(lv_event_t * e);

	static void eventSaveHandler(lv_event_t * e);
	void saveHandler(lv_event_t * e);

	static void eventUndoHandler(lv_event_t * e);
	void undoHandler(lv_event_t * e);

	static void eventUpdateTime(void * obj, int32_t v);
	void updateTime();
	void updateLabelNum(lv_obj_t *label, uint8_t num);

	 lv_style_t xStyleBtn;
	 lv_style_t xStyleSelectBtn;
	 lv_obj_t * pHourLabel ;
	 lv_obj_t * pHourBtn;
	 lv_obj_t * pMinLabel ;
	 lv_obj_t * pMinBtn;
	 lv_obj_t * pImgBg ;

	 lv_obj_t * pPlusBtn;
	 lv_obj_t * pMinusBtn ;


	 lv_obj_t * pSecLabel;
	 lv_obj_t * pSecBtn;

	 lv_anim_t xAnim;

	 datetime_t xDateTime;
	 TimeSelect xSelection = NothingSelected;

	 DS3231 *pDS3231 = NULL;
};

#endif /* SRC_TIMETILE_H_ */
