/*
 * ColourAnim.h
 *
 *  Created on: 10 Oct 2024
 *      Author: jondurrant
 */

#ifndef EXP_TREE_SRC_COLOURANIM_H_
#define EXP_TREE_SRC_COLOURANIM_H_

#include "pico/stdlib.h"

class ColourAnim {
public:
	ColourAnim();
	virtual ~ColourAnim();

	virtual void step(float step=0.5)=0;
	virtual uint8_t red();
	virtual uint8_t green();
	virtual uint8_t blue();

protected:
	uint8_t xRed = 0;
	uint8_t xGrn = 0;
	uint8_t xBlu = 0;

};

#endif /* EXP_TREE_SRC_COLOURANIM_H_ */
