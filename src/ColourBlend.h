/*
 * ColourBlend.h
 *
 *  Created on: 10 Oct 2024
 *      Author: jondurrant
 */

#ifndef EXP_TREE_SRC_COLOURBLEND_H_
#define EXP_TREE_SRC_COLOURBLEND_H_

#include "ColourAnim.h"


#ifndef COLOUR_SEQ_MAX
#define COLOUR_SEQ_MAX 10
#endif


class ColourBlend: public  ColourAnim{
public:
	ColourBlend();
	virtual ~ColourBlend();

	virtual void addColour(uint8_t r, uint8_t g,  uint8_t b);

	virtual void step(float step=0.5);

protected:
	uint8_t xSeq[COLOUR_SEQ_MAX][3];
	uint32_t xSeqLen=0;
};

#endif /* EXP_TREE_SRC_COLOURBLEND_H_ */
