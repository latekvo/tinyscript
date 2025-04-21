#pragma once

#include <sys/types.h>

#include "tokens.h"

extern char *syntaxUnitsSeparators[];
extern size_t syntaxUnitsSeparatorsCount;

Token *extractFirstTokens(char **str, char **literal);
