#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "debugging.h"
#include "rulesets.h"

// fixme: both the definition of CommandRuleset,
// and of the commandRulesets contains multiple redundancies,
// these should rewritten be simplified
CommandRuleset commandRulesets[] = {
    {
        // = w/ const
        // todo: change to cmd. define
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
        // = w/ let
        // todo: change to cmd. define
        CMD_ASSIGN,
        CMD_TYPE_CUSTOM,
        {
            TOK_LET,
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
            TOK_LHS_GROUP,
            TOK_ASSIGN,
            TOK_RHS_GROUP,
            TOK_END,
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
    {
        // func(args...)
        CMD_CALL,
        CMD_TYPE_CUSTOM,
        {
            TOK_RHS_GROUP,
            TOK_L_PN_BRACKET,
            TOK_RHS_GROUP,
            TOK_R_PN_BRACKET,
            TOK_END,
        },
    },
};

size_t commandRulesetsCount = sizeof(commandRulesets) / sizeof(CommandRuleset);

// TODO: is it better to define assignment commands as:
// - LHS = RHS;
// or
// - = RHS;
// and define
// - let LHS;
// as a separete CMD_DEFINE
// i feel like the latter would be more robust
// it would simplify handling classes and nested accesses as well
// new node types will have to be added:
// - node command (already present)
// - node literal (already present)
// - node variable, attributes: isLHS, (i assume isRHS would always be true)

// fixme: mova away from malloc'ing within functions everywhere

/**
 *
 * Returns: length of the RHS group.
 */
size_t getRhsGroupLength(ssize_t *tokens, size_t startOffset) {
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
  // TODO: this function is currently ran 2 times over the exact same tokens
  // 			 both in AST construction and ruleset detection

  // replace depth with a opening_pn_queue and closing_pn_queue,
  // to support infinite nesting (currently supporting 1 pair of '()')

  size_t nestingDepth = 0;

  // fixme: treat entire "" block as one token - string literal

  // 1. RHS group could be a non-nested literal, starts with anything
  // 2. RHS group could be a nested literal, starts with (, ends with )
  for (size_t i = startOffset; tokens[i] != TOK_END; i++) {
    if (tokens[i] == TOK_L_PN_BRACKET) {
      nestingDepth++;
    }
    if (tokens[i] == TOK_R_PN_BRACKET) {
      if (nestingDepth == 1) {
        // opening bracked accounted for in 'i', +1 for closing bracket
        return i + 1;
      }
      nestingDepth--;
    }

    if (nestingDepth > 0) {
      // ignore literals when inside () block
      continue;
    }

    if (tokens[i] <= 0) {
      return 1;
    }
  }
  return 3;
}

// TODO: don't malloc, return pre-allocated full pattern ptr
void getFullTokenPattern(long *fullPattern, size_t rulesetId) {
  CommandRuleset *baseRuleset = &commandRulesets[rulesetId];
  if (baseRuleset->type == CMD_TYPE_CUSTOM) {
    for (size_t i = 0;; i++) {
      if (i == CMD_TOK_BUF_SIZE) {
        printf("Fatal: Missing TOK_END in ruleset %zu. Terminating.\n",
               rulesetId);
        exit(1);
      }
      fullPattern[i] = baseRuleset->tokenSeries[i];
      if (baseRuleset->tokenSeries[i] == TOK_END) {
        break;
      }
    }
  } else if (baseRuleset->type == CMD_TYPE_RHS_PAIR) {
    // TODO: are parentheses always skipped? are they always present?
    // TODO: remove LHS in the next version
    fullPattern[0] = TOK_RHS_GROUP;
    fullPattern[1] = baseRuleset->tokenSeries[0];
    fullPattern[2] = TOK_RHS_GROUP;
  } else if (baseRuleset->type == CMD_TYPE_INLINE_SETTER) {
    // TODO: are parentheses always skipped? are they always present?
    // TODO: remove LHS in the next version
    fullPattern[0] = TOK_LHS_GROUP;
    fullPattern[1] = baseRuleset->tokenSeries[0];
    fullPattern[2] = TOK_ASSIGN;
    fullPattern[3] = TOK_RHS_GROUP;
  }
};

ssize_t findMatchingPatternIndex(ssize_t *tokens) {
  // TODO: harden by clearing before every overwrite
  long *pattern = malloc(sizeof(long) * CMD_TOK_BUF_SIZE);
  for (size_t ruleIdx = 0; ruleIdx < commandRulesetsCount; ruleIdx++) {
    CommandRuleset *ruleset = &commandRulesets[ruleIdx];
    getFullTokenPattern(pattern, ruleIdx);

    size_t tokenCursor = 0; // possibly ahead of "i"
    for (size_t i = 0;; i++, tokenCursor++) {
      if (i >= CMD_TOK_BUF_SIZE) {
        // err should not be possible, caught in getFull[...] if present
        printf("Fatal: Missing TOK_END in ruleset %zu. Terminating.\n",
               ruleIdx);
        exit(1);
      }

      printf("Dbg: RULE (%zu), PATTERN (%s), FOUND (%s)\n", ruleIdx,
             tokenToText(pattern[i]), tokenToText(tokens[tokenCursor]));

      if (pattern[i] == TOK_END) {
        // success - found match
        free(pattern);
        return ruleIdx;
      }

      if (pattern[i] == TOK_LHS_GROUP) {
        // TODO: lookup if present in variable store
        // fail - not a LHS value
        if (tokens[tokenCursor] > 0) {
          break;
        }
      } else if (pattern[i] == TOK_RHS_GROUP) {
        // TODO: lookup if present in value or variable store
        if (tokens[tokenCursor] == TOK_L_PN_BRACKET) {
          // this function is a simple matcher,
          // simply skip over "()" block
          size_t rhsLen = getRhsGroupLength(tokens, tokenCursor);
          tokenCursor += rhsLen + 1; // moving cursor +1 over the end
        } else if (tokens[tokenCursor] > 0) {
          // fail - not a RHS value
          break;
        }
      } else if (pattern[i] != tokens[tokenCursor]) {
        // fail - pattern mismatch
        break;
      }
    }
  }
  free(pattern);
  return -1;
}

