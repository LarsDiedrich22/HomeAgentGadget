/*
 * AdventTrees.cpp
 *
 *  Created on: 10 Oct 2024
 *      Author: jondurrant
 */

#include "AdventTrees.h"
#include <cstdio>

#define LEDS_LENGTH 33

AdventTrees::AdventTrees(uint8_t gp){
	pLedChain = new LedChain(LEDS_LENGTH, gp);
	if (pLedChain == NULL){
		printf("ERROR AdventTrees allocate chain failed\n");
	}
	for (int i=0; i < 3; i++){
		pTrees[i] = new Tree(pLedChain, i * 11);
		if (pTrees[i] == NULL){
			printf("ERROR AdventTrees allocate tree failed\n");
		}
	}

}

AdventTrees::~AdventTrees() {
	for (int i=0; i < 3; i++){
		delete(pTrees[i] );
	}
	delete(pLedChain);
}


void AdventTrees::setDay(uint8_t d){

	FeastDay feast = normal_day;

	switch(d){
	case 5:
		feast =krampus_day;
		break;
	case 11:
		feast = two_weeks_day;
		break;
	case 18:
		feast = one_week_day;
		break;
	case 25:
		feast = christmas_day;
		break;
	case 32:
		feast = new_years_day;
		break;
	case 36:
		feast = twelth_night;
		break;
	}

	if ((feast == one_week_day) || (feast == two_weeks_day)){
		pTrees[0]->setDay(d);
	} else {
		pTrees[0]->setDay(d, feast);
	}
	if (d < 8){
		pTrees[1]->setDay(0, feast);
	} else {
		if (feast == one_week_day){
			pTrees[1]->setDay(d - 8);
		} else {
			pTrees[1]->setDay(d - 8, feast);
		}
	}
	if (d < 16){
		pTrees[2]->setDay(0, feast);
	} else {
		pTrees[2]->setDay(d - 16, feast);
	}
}

void AdventTrees::animate(float step){
	for (int i=0; i < 3; i++){
		pTrees[i]->animate(step);
	}
	pLedChain->draw();
}


