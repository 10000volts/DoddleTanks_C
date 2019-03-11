#include <stdlib.h>
#include <time.h>
#include "RandomEngine.h"

static unsigned int seed;

void InitializeRandomEngine()
{
	seed = time(NULL);
}

static int r;
int NextRand()
{
	seed = rand() * rand();
	srand(seed);
	return rand() % 1024;
}
