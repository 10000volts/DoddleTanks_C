#include <stdlib.h>
#include <time.h>
#include "RandomEngine.h"

static unsigned int seed;

void InitializeRandomEngine()
{
	seed = time(NULL);
}

double NextRand()
{
	seed = rand();
	srand(seed);
	return rand() / RAND_MAX;
}
