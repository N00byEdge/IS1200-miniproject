#include "Lib.h"

int getHighestBitSet(unsigned i) {
	int j = 31;
	while(j != -1) if((1 << j) & i) break;
	return j;
}
