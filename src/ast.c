#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "ast.h"

void freeSyntaxTree(SyntaxNode *node) {
  if (node->rhsNodes != NULL) {
    for (size_t i = 0; i < node->rhsCount; i++) {
      if (node->rhsNodes[i].type == RHS_TYPE_SYNTAX_NODE) {
        freeSyntaxTree(node->rhsNodes[i].value.rhsNode);
      }
    }
    free(node->rhsNodes);
  }
  free(node);
}

void pushRhsNode(SyntaxNode *lhs, RHSNode rhs) {
  if (lhs->rhsNodes == NULL) {

  } else if (lhs->rhsCount == lhs->rhsCapacity) {
    lhs->rhsCapacity *= 2;

    printf("Construct AST: Expanding node rhs capacity to %zu",
           lhs->rhsCapacity);

    lhs->rhsNodes =
        reallocarray(lhs->rhsNodes, lhs->rhsCount, lhs->rhsCapacity);
  }

  lhs->rhsNodes[lhs->rhsCount].value = rhs.value;
  lhs->rhsNodes[lhs->rhsCount].type = rhs.type;
  lhs->rhsCount++;
}

// instead of allowing SyntaxNode to be both the stem, and the leaf,
// we could limit it's role to just the stem, and use separate constructs
// as leafs.
// Example:
// -> ASSIGN [Node]
// 		-> VALUE [Leaf]
// 			 -> "foo"
// -> ASSIGN [Node]
// 		-> GET [Leaf]
// 			 -> "varname"
// Consider this solution, if S. Node handling logic will become overcomplicated
//
// ---
//
// desired structure:
// [global]
//  - [file.js] // may export definitions
//    - [nested definitions]
// example:
// global
// -> FILE(RHS, RHS...)
// 		-> "main.js"
// 	  -> DEFINE(RHS)
// 	  .  -> "increment"
// 	  -> ASSIGN(RHS, RHS)
// 	  .  -> "increment"
// 	  .  -> FUNCTION(RHS...)
// 	  .     -> DEFINE_ARG(RHS) // not sure how this will work yet
// 	  .     .  -> "input"
// 	  .			-> DEFINE(RHS)
// 	  .     .	-> "output"
// 	  .     -> ASSIGN(RHS, RHS)
// 	  .     .	-> "output"
// 	  .     .	-> ADD(RHS, RHS)
// 	  .     .		 -> GET(RHS)
// 	  .     .		 .	-> "input"
// 	  .     .		 -> 1
// 	  .     -> RETURN(RHS)
// 	  .     		-> GET(RHS)
// 	  .     			 -> "output"
//		-> CALL(RHS, RHS...)
//			 -> "increment"

// recursive AST constructor
SyntaxNode *constructSyntaxTree(ssize_t *tokens, SyntaxNode *lhs) {
  if (lhs == NULL) {
    printf("Construct AST: Initializing AST root\n");
    // init root
    lhs = malloc(sizeof(SyntaxNode));
    lhs->command = CMD_RETURN;
    lhs->lhs = NULL;
    lhs->rhsCount = 0;
    lhs->rhsCapacity = 256; // large initial cap for root
    lhs->rhsNodes = malloc(sizeof(RHSNode) * lhs->rhsCapacity);
  }

  // function-body parsing is not recursive
  // nested statements ARE recursive with constructSyntaxTree
  for (size_t tokensOffset = 0; tokens[tokensOffset] != TOK_END;
       tokensOffset += 1) {
    size_t *args = NULL;
    ssize_t ruleIdx = findMatchingPatternIndex(tokens + tokensOffset);

    if (ruleIdx < 0) {
      printf("Construct AST: Invalid syntax: Rule not found\n");
    }

    CommandRuleset *ruleset = &commandRulesets[ruleIdx];
    if (ruleset->type == CMD_TYPE_RHS_PAIR) {
      // operator type RHS
      printf("Construct AST: Match found: RHS PAIR\n");
      RHSNode rhs;
      rhs.type = RHS_TYPE_SYNTAX_NODE;
      rhs.value.rhsNode = malloc(sizeof(SyntaxNode));
      rhs.value.rhsNode->lhs = lhs;
      rhs.value.rhsNode->command = ruleset->command;
      pushRhsNode(lhs, rhs);
    }

    if (ruleset->type == CMD_TYPE_CALL) {
      printf("Construct AST: Match found: CALL\n");
      // eval including member access parent class
    }

    if (ruleset->type == CMD_TYPE_CUSTOM) {
      printf("Construct AST: Match found: DEFINED\n");
      for (size_t tokIdx = 0; tokIdx < CMD_TOK_BUF_SIZE; tokIdx++) {
        // Consume ; as part of END
        if (ruleset->tokenSeries[tokIdx] == TOK_LITERAL && tokens[tokIdx] > 0) {
          break;
        }
      }
    }
  }
  return lhs;
}
