#pragma once

#include <sys/types.h>

#include "ast.h"
#include "tokens.h"

char *tokenToText(ssize_t token);
void prettyPrintAst(SyntaxNode *root, char **literals);
void prettyPrintTokens(ssize_t *tokens, size_t count, short isSlice);

