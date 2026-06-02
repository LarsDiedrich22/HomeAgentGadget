/*
 * ColourFadeInOut.cpp
 *
 *  Created on: 10 Oct 2024
 *      Author: jondurrant
 */

#include "ColourFadeInOut.h"
#include <cstdio>

ColourFadeInOut::ColourFadeInOut() {
	// TODO Auto-generated constructor stub

}

ColourFadeInOut::~ColourFadeInOut() {
	// TODO Auto-generated destructor stub
}

void ColourFadeInOut::addColour(uint8_t r, uint8_t g,  uint8_t b){
	if (xSeqLen < (COLOUR_SEQ_MAX )){
		xSeq[xSeqLen][0] = r;
		xSeq[xSeqLen][1] = g;
		xSeq[xSeqLen][2] = b;
		xSeqLen++;
	}
}

void ColourFadeInOut::step(float step){
	if (xSeqLen > 0){
		float perColour = 1.0 / (float) xSeqLen;
		int colourInx = step / perColour;
		float stepNorm = (step - ((float) colourInx * perColour)) * (float)xSeqLen;


		float dim;
		if (stepNorm <= 0.5){
			dim = stepNorm * 2;
		} else {
			dim = (0.5 - ( stepNorm - 0.5)) * 2.0;
		}

		xRed = dim * xSeq[colourInx][0];
		xGrn = dim * xSeq[colourInx][1];
		xBlu = dim * xSeq[colourInx][2];


		/*
		printf("Step(%.2f) colourInx %d,  norm %.2f,  dim %.2f RGB %X %X %X\n",
				step,
				colourInx,
				stepNorm,
				dim,
				xRed,
				xGrn,
				xBlu
				);
				*/
	}
}
