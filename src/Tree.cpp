/*
 * Tree.cpp
 *
 *  Created on: 10 Oct 2024
 *      Author: jondurrant
 */

#include "Tree.h"

#define TREE_LEN 11

Tree::Tree(LedChain *chain, uint16_t startLed){
	pLedChain = chain;
	xStartLed = startLed;

	//Top of tree glow
	xGlow.addColour(0x30, 0x30, 0x30);
	xGlow.addColour(0x10, 0x10, 0x10);

	//Top tree point
	xSolidGrn.addColour(0x00, 0xD0, 0x00);
	xSolidGrn.step(1.0);

	//trunk
	xTrunkA.addColour(0x00, 0xFF, 0x00);
	xTrunkA.addColour(0x00, 0x00, 0xFF);
	xTrunkB.addColour(0x00, 0x00, 0xFF);
	xTrunkB.addColour(0x00, 0xFF, 0x00);

	//Candle
	xCandleA.addColour(0xC2, 0xF5, 0x42);
	xCandleA.addColour(0xF5, 0xA4, 0x42);
	xCandleA.addColour(0xD5, 0xC4, 0xC4);
	xCandleB.addColour(0xF5, 0xA4, 0x42);
	xCandleB.addColour(0xC2, 0xF5, 0x42);

	//Fairy
	xFairyA.addColour(0xF0, 0x00, 0x00);
	xFairyA.addColour(0xFF, 0xC8, 0x00);
	xFairyA.addColour(0x23, 0xF5, 0x00);
	xFairyA.addColour(0x00, 0x3F, 0xF5);
	xFairyA.addColour(0xCC, 0x00, 0xF5);
	xFairyB.addColour(0x23, 0xF5, 0x00);
	xFairyB.addColour(0x00, 0x3F, 0xF5);
	xFairyB.addColour(0xCC, 0x00, 0xF5);
	xFairyB.addColour(0xF0, 0x00, 0x00);
	xFairyB.addColour(0xFF, 0xC8, 0x00);
	xBFairyA.addColour(0xFF, 0xC8, 0x00);
	xBFairyA.addColour(0x23, 0xF5, 0x00);
	xBFairyA.addColour(0x00, 0x3F, 0xF5);
	xBFairyA.addColour(0xCC, 0x00, 0xF5);
	xBFairyA.addColour(0xF0, 0x00, 0x00);
	xBFairyB.addColour(0x00, 0x3F, 0xF5);
	xBFairyB.addColour(0xCC, 0x00, 0xF5);
	xBFairyB.addColour(0xF0, 0x00, 0x00);
	xBFairyB.addColour(0xFF, 0xC8, 0x00);
	xBFairyB.addColour(0x23, 0xF5, 0x00);

	//Krampus
	xBlood.addColour(0x50, 0, 0);
	xBlood.addColour(0x30, 0, 0);

	//Blue Candle
	xBlue.addColour(0x10, 0x10, 0x30);
	xBlue.addColour(0x00, 0x00, 0x10);

	//Snuffing Candles
	xSnuffA.addColour(0xC2, 0xF5, 0x42);
	xSnuffA.addColour(0x52, 0x65, 0x22);
	xSnuffA.addColour(0x22, 0x35, 0x12);
	xSnuffB.addColour(0xC2, 0xF5, 0x42);
	xSnuffB.addColour(0x22, 0x35, 0x12);
	xSnuffB.addColour(0x00, 0x00, 0x00);
	xSnuffC.addColour(0xC2, 0xF5, 0x42);
	xSnuffC.addColour(0x00, 0x00, 0x00);
}

Tree::~Tree() {
	// TODO Auto-generated destructor stub
}


void Tree::setDay(uint8_t d, FeastDay feast){
	xDay = d;
	xFeast = feast;
}

void Tree::animate(float step){
	clear();

	//Tree Tip
	if (xFeast == normal_day){
		if (xDay < 8){
			xGlow.step(step);
			pixel(&xGlow, TREE_LEN -1);
		} else {
			xCandleA.step(step);
			xCandleB.step(step);
			for (int i=8; i < TREE_LEN; i++){
				if (i%2){
					pixel(&xCandleA, i);
				} else {
					pixel(&xCandleB, i);
				}
			}
		}
	} else {
		switch(xFeast){
		case krampus_day:
			xBlood.step(step);
			for (int i=9; i < TREE_LEN; i++){
				pixel(&xBlood, i);
			}
			break;
		case two_weeks_day:
		case one_week_day:
			xBlue.step(step);
			pixel(&xBlue, TREE_LEN -1);
			break;
		case twelth_night:
			xSnuffA.step(step);
			pixel(&xSnuffA, TREE_LEN -3);
			xSnuffB.step(step);
			pixel(&xSnuffB, TREE_LEN -2);
			xSnuffC.step(step);
			pixel(&xSnuffC, TREE_LEN -1);
		}
	}

	if (xDay == 0){
		return;
	}

	uint8_t d = xDay;
	if (xDay > 8){
		xDay = 8;
	}


	pixel(&xSolidGrn, xDay -1);
	xTrunkA.step(step);
	xTrunkB.step(step);
	for (int i= 0; i  < xDay -1; i++){
		if (i%2){
			pixel(&xTrunkA, i);
		} else {
			pixel(&xTrunkB, i);
		}
	}


	if (xDay >= 8) {
		xBFairyA.step(step);
		xBFairyB.step(step);
		if ((xFeast == christmas_day) || (xFeast == new_years_day)){
			xFairyA.step(step);
			xFairyB.step(step);
			for (int i= 0; i  < TREE_LEN; i++){
				switch(i%4){
				case 0:
					pixel(&xFairyA, i);
					break;
				case 1:
					pixel(&xBFairyA, i);
					break;
				case 2:
					pixel(&xFairyB, i);
					break;
				case 3:
					pixel(&xBFairyA, i);
					break;
				}
			}
		} else {
			for (int i= 0; i  < TREE_LEN; i+=3){
				if (i%2){
						pixel(&xBFairyA, i);
					} else {
						pixel(&xBFairyB, i);
					}
			}
		}
	}

}



void Tree::pixel(ColourAnim *c,  uint16_t nm ){
	pLedChain->pixel(c,  nm  + xStartLed);
}

void Tree::clear(){
	for (int i=0;  i < TREE_LEN; i++){
		pLedChain->off(i+ xStartLed);
	}
}
