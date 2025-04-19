#pragma once

#include <sys/types.h>

#include "tokens.h"

typedef struct {
	char* match;
	Token token;
} TokenConversion;


// todo: convert to hashmap, or use any other O(1) acc. method
extern TokenConversion tokenConversions[];
extern size_t tokenConversionsCount;
