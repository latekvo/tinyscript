#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>

#include "rulesets.h"

CommandRuleset commandRulesets[] = {
    {
        // =
        CMD_ASSIGN,
        CMD_TYPE_CUSTOM,
        // Using TOK_LITERAL as temporary RHS wildcard arg - this is invalid!
        {
            TOK_LITERAL,
            TOK_ASSIGN,
            TOK_LITERAL,
            TOK_END,
        },
    },
    {
        // + +=
        CMD_ADD,
        CMD_TYPE_RHS_PAIR,
        {TOK_ADD},
    },
    {
        // - -=
        CMD_SUB,
        CMD_TYPE_RHS_PAIR,
        {TOK_SUB},
    },
    {
        // * *=
        CMD_MULT,
        CMD_TYPE_RHS_PAIR,
        {TOK_MULT},
    },
    {
        // / /=
        CMD_DIV,
        CMD_TYPE_RHS_PAIR,
        {TOK_DIV},
    },
};

size_t commandRulesetsCount = sizeof(commandRulesets) / sizeof(CommandRuleset);

ssize_t findMatchingPatternIndex(ssize_t *tokens, size_t tokensCount) {
  // todo: write a variadic arg tok pattern checker, will be much cleaner
  for (size_t ruleIdx = 0; ruleIdx < commandRulesetsCount; ruleIdx++) {
    CommandRuleset *ruleset = &commandRulesets[ruleIdx];
    if (ruleset->type == CMD_TYPE_RHS_PAIR) {
      // simplest case, no recursion
      if (tokens[0] > 0 ||                        //
          tokens[1] != ruleset->tokenSeries[0] || //
          tokens[2] > 0) {
        continue;
      }
      // todo: identify pattern despite recursivity
      // todo: replace simplest case with programatic catch-all approach
      // for now: if not the simplest case, recurse at the blank start
    }

    if (ruleset->type == CMD_TYPE_CALL) {
      if (tokens[0] > 0 || tokens[1] != TOK_L_PN_BRACKET) {
        continue;
      }
    }

    if (ruleset->type == CMD_TYPE_CUSTOM) {
      bool a = 1;
      for (size_t tokIdx = 0; tokIdx < CMD_TOK_BUF_SIZE; tokIdx++) {
        if (ruleset->tokenSeries[tokIdx] == TOK_END) {
          // todo: handle terminators here, preferably have
          // 			 a virtual TOK_SEP added during cleaning
          break;
        }
        // todo: handle literals being nested RHSs here
        if (ruleset->tokenSeries[tokIdx] == TOK_LITERAL && tokens[tokIdx] > 0) {
          break;
        }
      }
    }

    // return first rule that is valid (non-violated)
    return ruleIdx;
  }
  printf("Construct AST: Error: Couldn't find any matching rules.");
  return -1;
}

