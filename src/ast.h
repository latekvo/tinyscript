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

typedef struct {
  RHSType type;
  RHSValue value;
} RHSNode;

typedef struct SyntaxNode {
  Command command;
  struct SyntaxNode *lhs; // parent
  size_t rhsCount;
  size_t rhsCapacity;
  RHSNode *rhsNodes;
} SyntaxNode;

void freeSyntaxTree(SyntaxNode *node);
SyntaxNode *constructSyntaxTree(ssize_t *tokens, SyntaxNode *lhs);
