#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "ast.h"

void freeSyntaxTree(SyntaxNode *node) {
  if (node->rhsValues != NULL && node->rhsTypes != NULL) {
    for (size_t i = 0; i < node->rhsCount; i++) {
      if (node->rhsTypes[i] == RHS_TYPE_SYNTAX_NODE) {
        freeSyntaxTree(node->rhsValues[i].rhsNode);
      }
    }
    free(node->rhsTypes);
    free(node->rhsValues);
  }
  free(node);
}
void pushNodeRhs(SyntaxNode *node, RHSType type, RHSValue value) {
  node->rhsValues[node->rhsCount] = value;
  node->rhsTypes[node->rhsCount] = type;
  node->rhsCount++;
}

// recursive AST constructor
SyntaxNode *constructSyntaxTree(ssize_t *tokens, size_t tokensCount,
                                SyntaxNode *lhs) {
  if (lhs == NULL) {
    // init root
    lhs = malloc(sizeof(SyntaxNode));
    lhs->command = CMD_RETURN;
    lhs->lhs = NULL;
    lhs->rhsTypes = NULL;
    lhs->rhsValues = NULL;
  }

  size_t *args = NULL;
  ssize_t ruleIdx = findMatchingPatternIndex(tokens, tokensCount);

  if (ruleIdx < 0) {
  }

  CommandRuleset *ruleset = &commandRulesets[ruleIdx];
  if (ruleset->type == CMD_TYPE_RHS_PAIR) {
    printf("Construct AST: Match found: RHS PAIR\n");

    // fixme: recursive mechanism omitted for now
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

  return lhs;
}
