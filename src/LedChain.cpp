/*
 * LedChain.cpp
 *
 *  Created on: 9 Oct 2024
 *      Author: jondurrant
 */

#include "LedChain.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"
#include <cstdlib>
#include <cstdio>

LedChain::LedChain(uint16_t numLeds, uint8_t gp) {
	xPixels = (uint32_t*)malloc(numLeds * sizeof(uint32_t));
	if (xPixels == NULL){
		printf("ERROR LedChain malloc failed\n");
	}
	xNumLeds = numLeds;

	 PIO pio = pio0;
	 int sm = 0;
	 uint offset = pio_add_program(pio, &ws2812_program);

	 ws2812_program_init(pio, sm, offset, gp, 800000, false);
}

LedChain::~LedChain() {
	free(xPixels);
}

void LedChain::all(uint8_t r, uint8_t g, uint8_t b){
	if (xPixels != NULL){
		uint32_t c = r << 8 | g << 16 | b;
		for (uint i=0; i < xNumLeds; i++){
			xPixels[i] = c;
		}
	}
}

void LedChain::pixel(uint8_t r, uint8_t g, uint8_t b, uint16_t num ){
	if (xPixels != NULL){
		uint32_t c = r << 8 | g << 16 | b;
		xPixels[num] = c;
	}
}

void LedChain::pixel(ColourAnim *c, uint16_t num ){
	pixel(c->red(),  c->green(), c->blue(),  num );
}


void LedChain::off(uint16_t num){
	if (xPixels != NULL){
		uint32_t c = 0;
		xPixels[num] = c;
	}
}

void LedChain::allOff(){
	if (xPixels != NULL){
		uint32_t c = 0;
		for (uint i=0; i < xNumLeds; i++){
			xPixels[i] = c;
		}
	}
}

void LedChain::draw(){
	if (xPixels != NULL){
		for (uint i=0; i < xNumLeds; i++){
			pio_sm_put_blocking(pio0, 0, xPixels[i]  << 8u);
		}
	}
}

