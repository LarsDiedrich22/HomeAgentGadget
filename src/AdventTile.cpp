/*
 * AdventTile.cpp
 *
 *  Created on: 11 Oct 2024
 *      Author: jondurrant
 */

#include "AdventTile.h"

extern "C"{
#include "LVGL_example.h"
}

extern const lv_img_dsc_t Stag;
extern const lv_img_dsc_t NorthernLights;
extern const lv_img_dsc_t Present;
extern const lv_img_dsc_t SaintNicolas;
extern const lv_img_dsc_t ChristmasTree;
extern const lv_img_dsc_t Dressing;
extern const lv_img_dsc_t GingerbreadHouse;
extern const lv_img_dsc_t Krampus;
extern const lv_img_dsc_t Skating;
extern const lv_img_dsc_t Presents;
extern const lv_img_dsc_t Solstice;

extern const lv_img_dsc_t Dinner;
extern const lv_img_dsc_t Exchange;
extern const lv_img_dsc_t Pollar;
extern const lv_img_dsc_t Sley;
extern const lv_img_dsc_t Snowball;

extern const lv_img_dsc_t Canes;
extern const lv_img_dsc_t Chestnuts;
extern const lv_img_dsc_t Kittens;
extern const lv_img_dsc_t RobotsSnowman1;
extern const lv_img_dsc_t RobotsSnowman2;
extern const lv_img_dsc_t Sheppard;
extern const lv_img_dsc_t Sweets;
extern const lv_img_dsc_t Wreath;

extern const lv_img_dsc_t Wolf;
extern const lv_img_dsc_t Shoppers;

extern const lv_img_dsc_t Day1;
extern const lv_img_dsc_t Day2;
extern const lv_img_dsc_t Day3;
extern const lv_img_dsc_t Day4;
extern const lv_img_dsc_t Day5;
extern const lv_img_dsc_t Day6;
extern const lv_img_dsc_t Day7;
extern const lv_img_dsc_t Day8;
extern const lv_img_dsc_t Day9;
extern const lv_img_dsc_t Day10;
extern const lv_img_dsc_t Day11;
extern const lv_img_dsc_t Day12;

AdventTile::AdventTile() {
	// TODO Auto-generated constructor stub

}

AdventTile::~AdventTile() {
	// TODO Auto-generated destructor stub
}



void AdventTile::init(lv_obj_t *tv, uint8_t col_id, uint8_t row_id, lv_dir_t dir){
	initStyles();
	initImgs();

	 lv_obj_t *tileBtn = lv_tileview_add_tile(tv, col_id, row_id,  dir);

	 pImgBg = lv_img_create(tileBtn);
	 lv_img_set_src(pImgBg, &Present);
	 lv_obj_align(pImgBg, LV_ALIGN_CENTER, 0, 0);

	 //Current Day Button
	 lv_obj_t * currentBtn = lv_btn_create(tileBtn);
	 lv_obj_remove_style_all(currentBtn);
	 lv_obj_set_pos(currentBtn, DISP_HOR_RES/2-25,  0);
	 lv_obj_set_size(currentBtn, 50, 50);
	 lv_obj_add_style(currentBtn, &xStyleBtn, 0);
	 /*Add a label to the button*/
	 pCurrentLabel = lv_label_create(currentBtn);
	 lv_label_set_text(pCurrentLabel, "01");
	 lv_obj_center(pCurrentLabel);

	 //Today Button
	 lv_obj_t * todayBtn = lv_btn_create(tileBtn);
	 lv_obj_remove_style_all(todayBtn);
	 lv_obj_set_pos(todayBtn, DISP_HOR_RES/2-25, DISP_VER_RES-50);
	 lv_obj_set_size(todayBtn, 50, 50);
	 lv_obj_add_style(todayBtn, &xStyleBtn, 0);
	 /*Add a label to the button*/
	 pTodayLabel = lv_label_create(todayBtn);
	 lv_label_set_text(pTodayLabel, "01");
	 lv_obj_center(pTodayLabel);
	 lv_obj_add_event_cb(todayBtn, eventHandlerToday, LV_EVENT_ALL,  this);

	 //Plus Buton
	 lv_obj_t * plusBtn = lv_btn_create(tileBtn);
	 lv_obj_remove_style_all(plusBtn);
	 lv_obj_set_pos(plusBtn, 0,DISP_VER_RES/2-25);
	 lv_obj_set_size(plusBtn, 50, 50);
	 lv_obj_add_style(plusBtn, &xStyleBtn, 0);
	 /*Add a label to the button*/
	 lv_obj_t * plusLabel = lv_label_create(plusBtn);
	 lv_label_set_text(plusLabel, "+");
	 lv_obj_center(plusLabel);
	 lv_obj_add_event_cb(plusBtn, eventHandlerPlus, LV_EVENT_ALL,  this);

	 // - Button
	 lv_obj_t * minBtn = lv_btn_create(tileBtn);
	 lv_obj_remove_style_all(minBtn);
	 lv_obj_set_pos(minBtn, DISP_HOR_RES-50, DISP_VER_RES/2-25);
	 lv_obj_set_size(minBtn, 50, 50);
	 lv_obj_add_style(minBtn, &xStyleBtn, 0);
	 /*Add a label to the button*/
	 lv_obj_t * minLabel = lv_label_create(minBtn);
	 lv_label_set_text(minLabel, "-");
	 lv_obj_center(minLabel);
	 lv_obj_add_event_cb(minBtn, eventHandlerMinus, LV_EVENT_ALL,  this);
}

void AdventTile::initStyles(){
	    lv_style_init(&xStyleBtn);
	    lv_style_set_bg_color(&xStyleBtn, lv_palette_main(LV_PALETTE_LIGHT_GREEN));
	    lv_style_set_border_color(&xStyleBtn, lv_palette_darken(LV_PALETTE_LIGHT_GREEN, 3));
	    lv_style_set_border_width(&xStyleBtn, 2);
	    lv_style_set_radius(&xStyleBtn, 10);
	    lv_style_set_shadow_width(&xStyleBtn, 10);
	    lv_style_set_shadow_ofs_y(&xStyleBtn, 5);
	    lv_style_set_shadow_opa(&xStyleBtn, LV_OPA_50);
	    lv_style_set_text_color(&xStyleBtn, lv_color_make(0xFF, 0xFF, 0xFF));
	    lv_style_set_text_font(&xStyleBtn, &lv_font_montserrat_32);
	    lv_style_set_width(&xStyleBtn, 100);
	    lv_style_set_height(&xStyleBtn, LV_SIZE_CONTENT);
}


void AdventTile::eventHandlerPlus(lv_event_t * e){
	AdventTile *adv = (AdventTile *) lv_event_get_user_data(e);
	adv->handlerPlus(e);
}

void AdventTile::handlerPlus(lv_event_t * e){
   lv_event_code_t code = lv_event_get_code(e);
   if (code == LV_EVENT_PRESSED){
		xCurrentDay++;
		if (xCurrentDay > 36){
			xCurrentDay = 36;
		}
		char newS[5];

		if (xCurrentDay > 24){
			sprintf(newS, "C%d", xCurrentDay-24);
		} else {
			sprintf(newS, "%d", xCurrentDay);
		}
		 lv_label_set_text(pCurrentLabel,  newS);

		 updateImg();
	}
}

void AdventTile::eventHandlerMinus(lv_event_t * e){
	AdventTile *adv = (AdventTile *) lv_event_get_user_data(e);
	adv->handlerMinus(e);
}

void AdventTile::handlerMinus(lv_event_t * e){
   lv_event_code_t code = lv_event_get_code(e);
   if (code == LV_EVENT_PRESSED){
	   if (xCurrentDay > 0){
		   xCurrentDay--;
		}
		char newS[5];

		if (xCurrentDay > 24){
			sprintf(newS, "C%d", xCurrentDay-24);
		} else {
			sprintf(newS, "%d", xCurrentDay);
		}
		 lv_label_set_text(pCurrentLabel,  newS);

		 updateImg();
	}
}

void AdventTile::eventHandlerToday(lv_event_t * e){
	AdventTile *adv = (AdventTile *) lv_event_get_user_data(e);
	adv->handlerToday(e);
}

void AdventTile::handlerToday(lv_event_t * e){
   lv_event_code_t code = lv_event_get_code(e);
   if (code == LV_EVENT_PRESSED){
		xCurrentDay= xToday;
		updateDayLabel(pCurrentLabel, xCurrentDay);
		 updateImg();
	}
}

void AdventTile::updateDayLabel( lv_obj_t * label, uint8_t day){
	char newS[5];

	if (day > 24){
		sprintf(newS, "C%d", day-24);
	} else {
		sprintf(newS, "%d", day);
	}
	 lv_label_set_text(label,  newS);
}


void AdventTile::updateImg(){

	if( xCurrentDay > xToday) {
		 lv_img_set_src(pImgBg, &Present);
		 lv_obj_align(pImgBg, LV_ALIGN_CENTER, 0, 0);
		 return;
	}

	switch(xCurrentDay){
	case 0:
		 lv_img_set_src(pImgBg, &Present);
		 break;
	case 1:
		 lv_img_set_src(pImgBg, &Wolf);
		 break;
	case 2:
		 lv_img_set_src(pImgBg, &NorthernLights);
		 break;
	case 3:
		 lv_img_set_src(pImgBg, &Dressing);
		 break;
	case 4:
		 lv_img_set_src(pImgBg, &Skating);
		 break;
	case 5:
		 lv_img_set_src(pImgBg, &Krampus);
		 break;
	case 6:
		 lv_img_set_src(pImgBg, &SaintNicolas);
		 break;
	case 7:
		 lv_img_set_src(pImgBg, &ChristmasTree);
		 break;
	case 8:
		 lv_img_set_src(pImgBg, &Presents);
		 break;
	case 9:
			 lv_img_set_src(pImgBg, &Snowball);
			 break;
	case 10:
			 lv_img_set_src(pImgBg, &GingerbreadHouse);
			 break;
	case 11:
			 lv_img_set_src(pImgBg, &Wreath);
			 break;
	case 12:
			 lv_img_set_src(pImgBg, &Kittens);
			 break;
	case 13:
			 lv_img_set_src(pImgBg, &RobotsSnowman1);
			 break;
	case 14:
			 lv_img_set_src(pImgBg, &Sweets);
			 break;
	case 15:
			 lv_img_set_src(pImgBg, &Sheppard);
			 break;
	case 16:
			 lv_img_set_src(pImgBg, &Chestnuts);
			 break;
	case 17:
			 lv_img_set_src(pImgBg, &Canes);
			 break;
	case 18:
			 lv_img_set_src(pImgBg, &Shoppers);
			 break;
	case 19:
			 lv_img_set_src(pImgBg, &Stag);
			 break;
	case 20:
			 lv_img_set_src(pImgBg, &Dinner);
			 break;
	case 21:
		 lv_img_set_src(pImgBg, &Solstice);
		 break;
	case 22:
			 lv_img_set_src(pImgBg, &Pollar);
			 break;
	case 23:
			 lv_img_set_src(pImgBg, &Exchange);
			 break;
	case 24:
			 lv_img_set_src(pImgBg, &Sley);
			 break;
	case 25:
		 lv_img_set_src(pImgBg, &Day1);
		 break;
	case 26:
		 lv_img_set_src(pImgBg, &Day2);
		 break;
	case 27:
		 lv_img_set_src(pImgBg, &Day3);
		 break;
	case 28:
		 lv_img_set_src(pImgBg, &Day4);
		 break;
	case 29:
		 lv_img_set_src(pImgBg, &Day5);
		 break;
	case 30:
		 lv_img_set_src(pImgBg, &Day6);
		 break;
	case 31:
		 lv_img_set_src(pImgBg, &Day7);
		 break;
	case 32:
		 lv_img_set_src(pImgBg, &Day8);
		 break;
	case 33:
		 lv_img_set_src(pImgBg, &Day9);
		 break;
	case 34:
		 lv_img_set_src(pImgBg, &Day10);
		 break;
	case 35:
		 lv_img_set_src(pImgBg, &Day11);
		 break;
	case 36:
		 lv_img_set_src(pImgBg, &Day12);
		 break;
	default:
		 lv_img_set_src(pImgBg, &Present);
	}

	 lv_obj_align(pImgBg, LV_ALIGN_CENTER, 0, 0);
}

void AdventTile::initImgs(){
	 LV_IMG_DECLARE(Stag);
	 LV_IMG_DECLARE(NorthLights);
	 LV_IMG_DECLARE(Present);
	 LV_IMG_DECLARE(SaintNicolas);
	 LV_IMG_DECLARE(ChristmasTree);
	 LV_IMG_DECLARE(Dressing);
	 LV_IMG_DECLARE(GingerbreadHouse);
	 LV_IMG_DECLARE(Krampus);
	 LV_IMG_DECLARE(Skating);
	 LV_IMG_DECLARE(Presents);
	 LV_IMG_DECLARE(Solstice);

	 LV_IMG_DECLARE(Dinner);
	 LV_IMG_DECLARE(Exchange);
	 LV_IMG_DECLARE(Pollar);
	 LV_IMG_DECLARE(Sley);
	 LV_IMG_DECLARE(Snowball);

	 LV_IMG_DECLARE(Canes);
	 LV_IMG_DECLARE(Chestnuts);
	 LV_IMG_DECLARE(	Kittens);
	 LV_IMG_DECLARE(RobotsSnowman1);
	 LV_IMG_DECLARE(	RobotsSnowman2);
	 LV_IMG_DECLARE(Sheppard);
	 LV_IMG_DECLARE(Sweets);
	 LV_IMG_DECLARE(Wreath);

	 LV_IMG_DECLARE(Day1);
	 LV_IMG_DECLARE(Day2);
	 LV_IMG_DECLARE(Day3);
	 LV_IMG_DECLARE(Day4);
	 LV_IMG_DECLARE(Day5);
	 LV_IMG_DECLARE(Day6);
	 LV_IMG_DECLARE(Day7);
	 LV_IMG_DECLARE(Day8);
	 LV_IMG_DECLARE(Day9);
	 LV_IMG_DECLARE(Day10);
	 LV_IMG_DECLARE(Day11);
	 LV_IMG_DECLARE(Day12);
}

void AdventTile::setToday(uint8_t day){
	xToday = day;
	updateDayLabel(pTodayLabel, day);
	updateImg();
}
void AdventTile::setCurrent(uint8_t day){
	xCurrentDay = day;
	updateDayLabel(pCurrentLabel, day);
	updateImg();
}

uint8_t AdventTile::getToday(){
	return xToday;
}

uint8_t AdventTile::getCurrent(){
	return xCurrentDay;
}

