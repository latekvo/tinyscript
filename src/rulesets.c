#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>

#include "rulesets.h"

// fixme: both the definition of CommandRuleset,
// and of the commandRulesets contains multiple redundancies,
// these should rewritten be simplified
CommandRuleset commandRulesets[] = {
    {
        // =
        CMD_ASSIGN,
        CMD_TYPE_CUSTOM,
        {
            TOK_CONST,
            TOK_LHS_GROUP,
            TOK_ASSIGN,
            TOK_RHS_GROUP,
            TOK_END,
        },
    },
    {
        // =
        CMD_ASSIGN,
        CMD_TYPE_CUSTOM,
        {
            TOK_LET,
            TOK_LITERAL,
            TOK_ASSIGN,
            TOK_LITERAL,
            TOK_END,
        },
    },
    {
        // =
        CMD_ASSIGN,
        CMD_TYPE_CUSTOM,
        {
            TOK_LHS_GROUP,
            TOK_ASSIGN,
            TOK_RHS_GROUP,
        },
    },
    {
        // +
        CMD_ADD,
        CMD_TYPE_RHS_PAIR,
        {TOK_ADD},
    },
    {
        // +=
        CMD_ADD,
        CMD_TYPE_INLINE_SETTER,
        {TOK_ADD},
    },
    {
        // -
        CMD_SUB,
        CMD_TYPE_RHS_PAIR,
        {TOK_SUB},
    },
    {
        // -=
        CMD_SUB,
        CMD_TYPE_INLINE_SETTER,
        {TOK_SUB},
    },
    {
        // *
        CMD_MULT,
        CMD_TYPE_RHS_PAIR,
        {TOK_MULT},
    },
    {
        // *=
        CMD_MULT,
        CMD_TYPE_INLINE_SETTER,
        {TOK_MULT},
    },
    {
        // /
        CMD_DIV,
        CMD_TYPE_RHS_PAIR,
        {TOK_DIV},
    },
    {
        // /=
        CMD_DIV,
        CMD_TYPE_INLINE_SETTER,
        {TOK_DIV},
    },
};

size_t commandRulesetsCount = sizeof(commandRulesets) / sizeof(CommandRuleset);

// fixme: mova away from malloc'ing within functions everywhere
size_t getRhsGroupLength(char *tokens, size_t startOffset) {
  // TODO: we can extract ALL nested RHSs within a root RHS
  // 			 in one-pass:
  // 			 on '(':
  // 			 - append '('.pos to o_pos_list
  // 			 - append -1 to c_pos_list
  // 			 on ')':
  // 			 - write ')'.pos to last non-'-1' in c_pos_list
  // TODO: we should integrate the () creation with () extraction,
  // 			 having both as separate steps is mentally simpler,
  // 			 but leads to code and logic duplication and redundancy

  for (size_t i = startOffset; tokens[i] != TOK_END; i++) {
    if (tokens[i] == 0) {
    }
  }
  return 3;
}

ssize_t findMatchingPatternIndex(ssize_t *tokens) {
  // todo: write a variadic arg tok pattern checker, will be much cleaner
  for (size_t ruleIdx = 0; ruleIdx < commandRulesetsCount; ruleIdx++) {
    CommandRuleset *ruleset = &commandRulesets[ruleIdx];
    if (ruleset->type == CMD_TYPE_RHS_PAIR) {
      ssize_t separator = ruleset->tokenSeries[0];
      // fixme: match '(anything)' as valid RHS
    }

    if (ruleset->type == CMD_TYPE_CALL) {
      if (tokens[0] > 0 || tokens[1] != TOK_L_PN_BRACKET) {
        continue;
      }
    }

    if (ruleset->type == CMD_TYPE_CUSTOM) {
      bool isInvalid = 1;
      for (size_t tokIdx = 0; tokIdx < CMD_TOK_BUF_SIZE; tokIdx++) {
        if (ruleset->tokenSeries[tokIdx] == TOK_END) {
          // todo: handle terminators here, preferably have
          // 			 a virtual TOK_SEP added during cleaning
          isInvalid = 0;
          break;
        }
        if (ruleset->tokenSeries[tokIdx] == TOK_LHS_GROUP &&
            tokens[tokIdx] > 0) {
          // fixme: check for variables of literals
          // fixme+: include selected variables into the check
          break;
        }
        if (ruleset->tokenSeries[tokIdx] == TOK_RHS_GROUP &&
            tokens[tokIdx] > 0) {
          // fixme: check for () instead of literals
          break;
        }
      }
      if (isInvalid) {
        continue;
      }
    }

    // return first rule that is valid (non-violated)
    return ruleIdx;
  }
  printf("Construct AST: Error: Couldn't find any matching rules.");
  return -1;
}

