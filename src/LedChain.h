/*
 * LedChain.h
 *
 *  Created on: 9 Oct 2024
 *      Author: jondurrant
 */

#ifndef EXP_TREE_SRC_LEDCHAIN_H_
#define EXP_TREE_SRC_LEDCHAIN_H_

#include "pico/stdlib.h"
#include "ColourAnim.h"

class LedChain {
public:
	LedChain(uint16_t numLeds, uint8_t gp);
	virtual ~LedChain();

	void all(uint8_t r, uint8_t g, uint8_t b);
	void pixel(uint8_t r, uint8_t g, uint8_t b, uint16_t num = 0);
	void pixel(ColourAnim *c, uint16_t num = 0);
	void off(uint16_t num=0);
	void allOff();

	void draw();

private:

	uint32_t *xPixels = NULL;
	uint16_t xNumLeds;

};

#endif /* EXP_TREE_SRC_LEDCHAIN_H_ */
