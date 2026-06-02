/*
 * AdventTile.h
 *
 *  Created on: 11 Oct 2024
 *      Author: jondurrant
 */

#ifndef SRC_ADVENTTILE_H_
#define SRC_ADVENTTILE_H_

#include "lvgl.h"

class AdventTile {
public:
	AdventTile();
	virtual ~AdventTile();

	/***
	 *
	 * 	@param tv - Tile Viewer
	 * @param col_id
	 * @param row_id
	 * @param dir
	 */
	void init(lv_obj_t *tv, uint8_t col_id, uint8_t row_id, lv_dir_t dir);

	void setToday(uint8_t day);
	void setCurrent(uint8_t day);
	uint8_t getToday();
	uint8_t getCurrent();

private:
	void initStyles();
	void initImgs();

	void updateDayLabel( lv_obj_t * label, uint8_t day);

	void updateImg();
	static void eventHandlerPlus(lv_event_t * e);
	void handlerPlus(lv_event_t * e);
	static void eventHandlerMinus(lv_event_t * e);
	void handlerMinus(lv_event_t * e);
	static void eventHandlerToday(lv_event_t * e);
	void handlerToday(lv_event_t * e);

	 lv_style_t xStyleBtn;
	 lv_obj_t * pCurrentLabel ;
	 lv_obj_t * pTodayLabel ;
	 lv_obj_t * pImgBg ;

	 uint8_t xCurrentDay = 0;
	 uint8_t xToday = 0;
};

#endif /* SRC_ADVENTTILE_H_ */
