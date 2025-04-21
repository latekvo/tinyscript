#pragma once

#include <sys/types.h>

#include "rulesets.h"

typedef enum {
  RHS_TYPE_SYNTAX_NODE = 0,
  RHS_TYPE_LITERAL_REF,
} RHSType;

typedef union {
  struct SyntaxNode *rhsNode;
  size_t rhsLiteralRef;
} RHSValue;

typedef struct SyntaxNode {
  Command command;
  struct SyntaxNode *lhs; // parent
  size_t rhsCount;
  RHSType *rhsTypes;
  RHSValue *rhsValues;
} SyntaxNode;

void freeSyntaxTree(SyntaxNode *node);
SyntaxNode *constructSyntaxTree(ssize_t *tokens, size_t tokensCount,
                                SyntaxNode *lhs);

void pushNodeRhs(SyntaxNode *node, RHSType type, RHSValue value);
