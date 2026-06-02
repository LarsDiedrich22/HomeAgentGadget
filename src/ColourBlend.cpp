/*
 * ColourBlend.cpp
 *
 *  Created on: 10 Oct 2024
 *      Author: jondurrant
 */

#include "ColourBlend.h"
#include <cstdio>

ColourBlend::ColourBlend() {
	// TODO Auto-generated constructor stub

}

ColourBlend::~ColourBlend() {
	// TODO Auto-generated destructor stub
}


void ColourBlend::addColour(uint8_t r, uint8_t g,  uint8_t b){
	if (xSeqLen < (COLOUR_SEQ_MAX )){
		xSeq[xSeqLen][0] = r;
		xSeq[xSeqLen][1] = g;
		xSeq[xSeqLen][2] = b;
		xSeqLen++;
	}
}

void ColourBlend::step(float step){
	if (xSeqLen > 1){
		float perColour =  1.0 / (float) (xSeqLen ); //1.0 / (float) (xSeqLen - 1);
		int colourInx = step / perColour;
		float stepNorm = (step - ((float) colourInx * perColour)) * (float)xSeqLen;

		int colourNext = (colourInx + 1) % xSeqLen;

		//Smooth the final part of transformation
		if (stepNorm > 0.95){
			stepNorm = 1.0;
		}

		float rDif = ((float )xSeq[colourNext][0] - (float)xSeq[colourInx][0]) * stepNorm;
		float gDif = ((float )xSeq[colourNext][1] - (float)xSeq[colourInx][1]) * stepNorm;
		float bDif = ((float )xSeq[colourNext][2] - (float)xSeq[colourInx][2]) * stepNorm;

		xRed =  xSeq[colourInx][0] + rDif;
		xGrn = xSeq[colourInx][1] + gDif;
		xBlu =  xSeq[colourInx][2] + bDif;



		/*
		printf("Step(%.2f) perCol %.2f colourInx %d,  norm %.2f,  rDif %.2f RGB %X %X %X\n",
				step,
				perColour,
				colourInx,
				stepNorm,
				rDif,
				xRed,
				xGrn,
				xBlu
				);
				*/

	} else {
		xRed =  xSeq[0][0] ;
		xGrn = xSeq[0][1] ;
		xBlu =  xSeq[0][2] ;
	}
}
