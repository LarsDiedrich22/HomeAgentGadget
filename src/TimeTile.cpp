/*
 * TimeTile.cpp
 *
 *  Created on: 11 Oct 2024
 *      Author: jondurrant
 */

#include "TimeTile.h"

extern "C"{
#include "LVGL_example.h"
}

#include "hardware/rtc.h"
#include "pico/stdlib.h"
#include "pico/util/datetime.h"

extern const lv_img_dsc_t Clock;

TimeTile::TimeTile() {
	// TODO Auto-generated constructor stub

}

TimeTile::~TimeTile() {
	// TODO Auto-generated destructor stub
}

void TimeTile::init(lv_obj_t *tv, uint8_t col_id, uint8_t row_id, lv_dir_t dir){
	rtc_get_datetime(&xDateTime);

	int left = DISP_HOR_RES/2-75;
	int centre = DISP_HOR_RES/2-25;
	int right = DISP_HOR_RES/2-25+50;
	int top = 35;
	int middle = top + 60;
	int bottom = middle + 60;

	initStyles();
	lv_obj_t *tile= lv_tileview_add_tile(tv, col_id, row_id,  dir);

	//Background
	LV_IMG_DECLARE(Clock);
	pImgBg = lv_img_create(tile);
	lv_img_set_src(pImgBg, &Clock);
	lv_obj_align(pImgBg, LV_ALIGN_CENTER, 0, 0);

	//HourButton
	 pHourBtn = lv_btn_create(tile);
	 lv_obj_remove_style_all(pHourBtn);
	 lv_obj_set_pos(pHourBtn, left, top);
	 lv_obj_set_size(pHourBtn, 50, 50);
	 lv_obj_add_style(pHourBtn, &xStyleBtn, 0);
	 /*Add a label to the button*/
	 pHourLabel = lv_label_create(pHourBtn);
	 lv_label_set_text(pHourLabel, "01");
	 lv_obj_center(pHourLabel);
	 lv_obj_add_event_cb(pHourBtn, eventSelectHandler, LV_EVENT_ALL,  this);


	//MinButton
	 pMinBtn = lv_btn_create(tile);
	 lv_obj_remove_style_all(pMinBtn);
	 lv_obj_set_pos(pMinBtn, centre,  top);
	 lv_obj_set_size(pMinBtn, 50, 50);
	 lv_obj_add_style(pMinBtn, &xStyleBtn, 0);
	 /*Add a label to the button*/
	 pMinLabel = lv_label_create(pMinBtn);
	 lv_label_set_text(pMinLabel, "02");
	 lv_obj_center(pMinLabel);
	 lv_obj_add_event_cb(pMinBtn, eventSelectHandler, LV_EVENT_ALL,  this);


	//SecButton
	 pSecBtn = lv_btn_create(tile);
	 lv_obj_remove_style_all(pSecBtn);
	 lv_obj_set_pos(pSecBtn, right,  top);
	 lv_obj_set_size(pSecBtn, 50, 50);
	 lv_obj_add_style(pSecBtn, &xStyleBtn, 0);
	 /*Add a label to the button*/
	 pSecLabel = lv_label_create(pSecBtn);
	 lv_label_set_text(pSecLabel, "03");
	 lv_obj_center(pSecLabel);


	//Plus Button
	 pPlusBtn = lv_btn_create(tile);
	 lv_obj_remove_style_all(pPlusBtn);
	 lv_obj_set_pos(pPlusBtn, left, middle);
	 lv_obj_set_size(pPlusBtn, 50, 50);
	 lv_obj_add_style(pPlusBtn, &xStyleBtn, 0);
	 /*Add a label to the button*/
	 lv_obj_t * plusLabel = lv_label_create(pPlusBtn);
	 lv_label_set_text(plusLabel, "+");
	 lv_obj_center(plusLabel);
	 lv_obj_add_event_cb(pPlusBtn, eventIncHandler, LV_EVENT_ALL,  this);


	 //Min Button
	 pMinusBtn = lv_btn_create(tile);
	 lv_obj_remove_style_all(pMinusBtn);
	 lv_obj_set_pos(pMinusBtn, right, middle);
	 lv_obj_set_size(pMinusBtn, 50, 50);
	 lv_obj_add_style(pMinusBtn, &xStyleBtn, 0);
	 /*Add a label to the button*/
	 lv_obj_t * minLabel = lv_label_create(pMinusBtn);
	 lv_label_set_text(minLabel, "-");
	 lv_obj_center(minLabel);
	 lv_obj_add_event_cb(pMinusBtn, eventIncHandler, LV_EVENT_ALL,  this);


	 //Save Button
	 lv_obj_t * saveBtn = lv_btn_create(tile);
	 lv_obj_remove_style_all(saveBtn);
	 lv_obj_set_pos(saveBtn, left, bottom);
	 lv_obj_set_size(saveBtn, 50, 50);
	 lv_obj_add_style(saveBtn, &xStyleBtn, 0);
	 /*Add a label to the button*/
	 lv_obj_t * saveLabel = lv_label_create(saveBtn);
	 lv_label_set_text(saveLabel, "=");
	 lv_obj_center(saveLabel);
	 lv_obj_add_event_cb(saveBtn, eventSaveHandler, LV_EVENT_ALL,  this);

	 //Undo Button
	 lv_obj_t * undoBtn = lv_btn_create(tile);
	 lv_obj_remove_style_all(undoBtn);
	 lv_obj_set_pos(undoBtn, right, bottom);
	 lv_obj_set_size(undoBtn, 50, 50);
	 lv_obj_add_style(undoBtn, &xStyleBtn, 0);
	 /*Add a label to the button*/
	 lv_obj_t * undoLabel = lv_label_create(undoBtn);
	 lv_label_set_text(undoLabel, "X");
	 lv_obj_center(undoLabel);
	 lv_obj_add_event_cb(undoBtn, eventUndoHandler, LV_EVENT_ALL,  this);

	 //Time Animation
	lv_anim_init(&xAnim);
	lv_anim_set_var(&xAnim, this);
	lv_anim_set_exec_cb(&xAnim, eventUpdateTime);
	lv_anim_set_time(&xAnim, 1000);
	lv_anim_set_repeat_count(&xAnim, LV_ANIM_REPEAT_INFINITE);    /*Just for the demo*/
	lv_anim_set_repeat_delay(&xAnim, 0);
	lv_anim_set_values(&xAnim,0 , 1);
	lv_anim_start(&xAnim);


}

void TimeTile::initStyles(){
	    lv_style_init(&xStyleBtn);
	    lv_style_set_bg_color(&xStyleBtn, lv_palette_main(LV_PALETTE_LIGHT_GREEN));
	    lv_style_set_border_color(&xStyleBtn, lv_color_make(0x30, 0x30, 0x50));
	    lv_style_set_border_width(&xStyleBtn, 2);
	    lv_style_set_radius(&xStyleBtn, 10);
	    lv_style_set_shadow_width(&xStyleBtn, 10);
	    lv_style_set_shadow_ofs_y(&xStyleBtn, 5);
	    lv_style_set_shadow_opa(&xStyleBtn, LV_OPA_50);
	    lv_style_set_text_color(&xStyleBtn, lv_color_make(0xFF, 0xFF, 0xFF));
	    lv_style_set_text_font(&xStyleBtn, &lv_font_montserrat_32);
	    lv_style_set_width(&xStyleBtn, 100);
	    lv_style_set_height(&xStyleBtn, LV_SIZE_CONTENT);

	    lv_style_init(&xStyleSelectBtn);
	    lv_style_set_border_color(&xStyleSelectBtn, lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 3));
	    lv_style_set_text_color(&xStyleSelectBtn, lv_color_make(0xCF, 0x1F, 0x1F));
}



void TimeTile::eventSelectHandler(lv_event_t * e){
	TimeTile *adv = (TimeTile *) lv_event_get_user_data(e);
	adv->selectHandler(e);
}

void TimeTile::selectHandler(lv_event_t * e){
	lv_obj_t * btn = lv_event_get_target( e);
	lv_event_code_t code = lv_event_get_code(e);
	 if (code == LV_EVENT_PRESSED){
		lv_obj_remove_style(pHourBtn, &xStyleSelectBtn, 0);
		lv_obj_remove_style(pMinBtn, &xStyleSelectBtn, 0);

		if (btn ==pHourBtn){
			lv_obj_add_style(pHourBtn, &xStyleSelectBtn, 0);
			xSelection = HourSelected;
			xDateTime.sec = 0;
		}
		if (btn ==pMinBtn){
			lv_obj_add_style(pMinBtn, &xStyleSelectBtn, 0);
			xSelection = MinSelected;
			xDateTime.sec = 0;
		}
		updateTime();
	}

}


void TimeTile::eventIncHandler(lv_event_t * e){
	TimeTile *adv = (TimeTile *) lv_event_get_user_data(e);
	adv->incHandler(e);
}

void TimeTile::incHandler(lv_event_t * e){
	lv_obj_t * btn = lv_event_get_target( e);
	lv_event_code_t code = lv_event_get_code(e);
	 if (code == LV_EVENT_PRESSED){

		 //PLUS
		 if (btn ==pPlusBtn){
			 //Hour
			 if (xSelection == HourSelected){
				 xDateTime.hour++;
				 if (xDateTime.hour > 23){
					 xDateTime.hour = 0;
				 }
			}
			 if (xSelection == MinSelected){
					 xDateTime.min++;
					 if (xDateTime.min > 59){
						 xDateTime.min = 0;
					 }
				}
		 }

		 //Minus
		 if (btn ==pMinusBtn){
			 //Hour
			 if (xSelection == HourSelected){
				 if (xDateTime.hour == 0){
					 xDateTime.hour = 23;
				 } else {
					 xDateTime.hour--;
				 }
			}
			 if (xSelection == MinSelected){
				 if (xDateTime.min == 0){
					 xDateTime.min = 59;
				 } else {
					 xDateTime.min--;
				 }
			 }
		 }

		 updateTime();

	 }


}



void TimeTile::eventUpdateTime(void * obj, int32_t v){
	TimeTile * self = (TimeTile *) obj;
	//printf("Anim Trigger\n");
	self->updateTime();
}

void TimeTile::updateTime(){
	if (xSelection == NothingSelected){
		 rtc_get_datetime(&xDateTime);
	}
	updateLabelNum(pHourLabel, xDateTime.hour);
	updateLabelNum(pMinLabel, xDateTime.min);
	updateLabelNum(pSecLabel, xDateTime.sec);

}

void TimeTile::updateLabelNum(lv_obj_t *label, uint8_t num){
	char buf[5];

	sprintf(buf, "%02d", num);
	 lv_label_set_text(label, buf);
}

void TimeTile::eventSaveHandler(lv_event_t * e){
	TimeTile *adv = (TimeTile *) lv_event_get_user_data(e);
	adv->saveHandler(e);
}
void TimeTile::saveHandler(lv_event_t * e){
	lv_obj_t * btn = lv_event_get_target( e);
	lv_event_code_t code = lv_event_get_code(e);
	 if (code == LV_EVENT_PRESSED){
		lv_obj_remove_style(pHourBtn, &xStyleSelectBtn, 0);
		lv_obj_remove_style(pMinBtn, &xStyleSelectBtn, 0);
		rtc_set_datetime(&xDateTime);
		if (pDS3231 != NULL){
			pDS3231->set_time(
					xDateTime.hour,
					xDateTime.min,
					xDateTime.sec,
					false,
					false);
		}
		xSelection = NothingSelected;
	}
}

void TimeTile::eventUndoHandler(lv_event_t * e){
	TimeTile *adv = (TimeTile *) lv_event_get_user_data(e);
	adv->undoHandler(e);
}
void TimeTile::undoHandler(lv_event_t * e){
	lv_obj_t * btn = lv_event_get_target( e);
	lv_event_code_t code = lv_event_get_code(e);
	 if (code == LV_EVENT_PRESSED){
		lv_obj_remove_style(pHourBtn, &xStyleSelectBtn, 0);
		lv_obj_remove_style(pMinBtn, &xStyleSelectBtn, 0);
		xSelection = NothingSelected;
		updateTime();
	}
}

void TimeTile::setRTC(DS3231 *p){
	pDS3231 = p;
}
