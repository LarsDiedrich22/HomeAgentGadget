/*
 * AdventTrees.h
 *
 *  Created on: 10 Oct 2024
 *      Author: jondurrant
 */

#ifndef EXP_TREE_SRC_ADVENTTREES_H_
#define EXP_TREE_SRC_ADVENTTREES_H_

#include "Tree.h"
#include "LedChain.h"

class AdventTrees {
public:
	AdventTrees(uint8_t gp);
	virtual ~AdventTrees();

	/***
	 * Set Day
	 * 0 to 24 is Advent
	 * 25 to 35 is 12 days of Christmas
	 * @param d
	 */
	virtual void setDay(uint8_t d);

	virtual void animate(float step=0.5);

private:
	LedChain *pLedChain;
	Tree *pTrees[3];

};

#endif /* EXP_TREE_SRC_ADVENTTREES_H_ */
