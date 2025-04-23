#pragma once

#include <sys/types.h>

#include "tokens.h"

Token *extractFirstTokens(char **str, char **literal);

void tokenizeFile(ssize_t **tokens, size_t *tokensCount, char ***literals,
                  size_t *literalsCount, char *filename);
