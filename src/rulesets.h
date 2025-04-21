#pragma once

#include "tokens.h"
#include <sys/types.h>

#define CMD_TOK_BUF_SIZE 8

// TODO: explicitly define Command's RHS-ability
// note: non-RHS-able commands may still be 'RHS' of functions' bodies
typedef enum {
  CMD_ASSIGN = 0,
  CMD_CALL,
  CMD_RETURN,
  CMD_GET,
  CMD_SET,
  CMD_ADD,
  CMD_SUB,
  CMD_MULT,
  CMD_DIV,
} Command;

// note: LHS <TOK>= RHS type of operations are derived from RHS_PAIR defs
typedef enum {
  CMD_TYPE_CUSTOM = 0, // default, tok. series: {<TOK_0>, <TOK_1>, ..., <TOK_N>}
  CMD_TYPE_RHS_PAIR,   // groups R0 <TOK> R1 type ops, tok. series: {<TOK>}
  CMD_TYPE_CALL,       // special interpreting, tok. series: empty
} CommandType;

// Rulesets for given command
// - token matches
// - resulting Command
typedef struct {
  Command command;
  CommandType type; // changes tokenSeries to a custom schema
  Token tokenSeries[CMD_TOK_BUF_SIZE];
} CommandRuleset;

extern CommandRuleset commandRulesets[];
ssize_t findMatchingPatternIndex(ssize_t *tokens, size_t tokensCount);
