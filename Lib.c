#include "Lib.h"

int getHighestBitSet(unsigned i) {
	int j = 31;
	while(j != -1) if((1 << j) & i) break;
	return j;
}

volatile void fastsleep(unsigned t) {
	for(int i = 0; i < t; ++ i)
		for(int j = 0; j < 10000; ++ j)
			j = j;
}
