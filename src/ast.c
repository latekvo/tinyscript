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
  for (size_t tokensOffset = 0; tokens[tokensOffset] != TOK_END;) {
    size_t *args = NULL;

    // FIXME: large amount of logic is duplicated here and within this function
    ssize_t ruleIdx = findMatchingPatternIndex(tokens + tokensOffset);

    if (ruleIdx < 0) {
      printf("Fatal: Invalid syntax: Rule not found\n");
      exit(0);
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

      size_t leftLen = getRhsGroupLength(tokens, tokensOffset);

      // fixme: removal of all spaces removes the need for this loop
      // 				^ all spaces can be safely removed AFAIK
      size_t lenToRight = leftLen;
      for (;; lenToRight++) {
        ssize_t token = tokens[tokensOffset + lenToRight];
        if (token != TOK_SPACE && token != ruleset->tokenSeries[0]) {
          break;
        }
      }

      size_t rightLen =
          getRhsGroupLength(tokens, tokensOffset + lenToRight + leftLen);

      size_t leftStartIdx = tokensOffset;
      size_t rightStartIdx = tokensOffset;

      RHSNode left;
      RHSNode right;

      if (leftLen == 1) {
        // has to be literal or variable
        // todo: use CMD_GET if variable
        left.type = RHS_TYPE_LITERAL_REF;
        left.value.rhsLiteralRef = tokens[tokensOffset];
      } else {
        // +1, as we're skipping past parenthesis wrapping
        ssize_t *shiftedTokens = tokens + leftStartIdx + 1;
        left.type = RHS_TYPE_SYNTAX_NODE;
        left.value.rhsNode =
            constructSyntaxTree(shiftedTokens, rhs.value.rhsNode);
      }

      if (rightLen == 1) {
        // has to be literal or variable
        // todo: use CMD_GET if variable
        right.type = RHS_TYPE_LITERAL_REF;
        right.value.rhsLiteralRef = tokens[tokensOffset];
      } else {
        ssize_t *shiftedTokens = tokens + rightStartIdx + 1;
        right.type = RHS_TYPE_SYNTAX_NODE;
        right.value.rhsNode =
            constructSyntaxTree(shiftedTokens, rhs.value.rhsNode);
      }

      pushRhsNode(rhs.value.rhsNode, left);
      pushRhsNode(rhs.value.rhsNode, right);
      pushRhsNode(lhs, rhs);
      tokensOffset += leftLen + lenToRight + rightLen;
    } else if (ruleset->type == CMD_TYPE_CALL) {
      printf("Construct AST: Match found: CALL\n");
      // eval including member access parent class
      printf("Construct AST: Unimplemented - function calling. Skipping.\n");
      tokensOffset += 1;
    } else if (ruleset->type == CMD_TYPE_CUSTOM) {
      printf("Construct AST: Match found: CUSTOM\n");
      for (size_t tokIdx = 0; tokIdx < CMD_TOK_BUF_SIZE; tokIdx++) {
        // Consume ; as part of END
        if (ruleset->tokenSeries[tokIdx] == TOK_LITERAL && tokens[tokIdx] > 0) {
          break;
        }
      }
      printf("Construct AST: Unimplemented - custom. Skipping.\n");
      tokensOffset += 1;
    } else {
      printf("Construct AST: Warning: No valid pattern found. Skipping.\n");
      tokensOffset += 1;
    }
  }
  return lhs;
}
