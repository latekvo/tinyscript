#pragma once

#include <sys/types.h>

#include "ast.h"
#include "tokens.h"

char *tokenToText(ssize_t token);
void prettyPrintAst(SyntaxNode *root, char **literals);
