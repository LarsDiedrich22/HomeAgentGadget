/*
 * Tree.h
 *
 * Manage a Christmas tree of 1 WS2812B LEDs
 *
 *  Created on: 10 Oct 2024
 *      Author: jondurrant
 */

#ifndef EXP_TREE_SRC_TREE_H_
#define EXP_TREE_SRC_TREE_H_

#include "pico/stdlib.h"
#include "LedChain.h"

#include "ColourAnim.h"
#include "ColourBlend.h"
#include "ColourFadeInOut.h"



enum FeastDay {
	normal_day,
	krampus_day,
	two_weeks_day,
	one_week_day,
	christmas_day,
	new_years_day,
	twelth_night
};

class Tree {
public:
	Tree(LedChain *chain, uint16_t startLed=0);
	virtual ~Tree();

	/***
	 * Days are normal 0 to 8
	 *
	 * @param d 0 to 8.
	 * @param feast = Feast day will display special animation
	 */
	virtual void setDay(uint8_t d, FeastDay feast = normal_day);

	/***
	 * Animate by sending values between 0.0 and 1.0
	 * @param step
	 */
	virtual void animate(float step=0.5);

	/***
	 * Clear the tree
	 */
	virtual void clear();
protected:
	virtual void pixel(ColourAnim *c,  uint16_t nm = 0);



	LedChain *pLedChain = NULL;
	uint16_t xStartLed;
	uint8_t xDay = 0;
	FeastDay xFeast = normal_day;


	ColourBlend xGlow;
	ColourBlend xSolidGrn;
	ColourBlend xTrunkA;
	ColourBlend xTrunkB;
	ColourBlend xCandleA;
	ColourBlend xCandleB;

	ColourFadeInOut xFairyA;
	ColourFadeInOut xFairyB;
	ColourBlend xBFairyA;
	ColourBlend xBFairyB;

	ColourBlend xBlood;
	ColourBlend xBlue;

	ColourBlend xSnuffA;
	ColourBlend xSnuffB;
	ColourBlend xSnuffC;


};

#endif /* EXP_TREE_SRC_TREE_H_ */
