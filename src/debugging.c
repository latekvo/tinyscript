#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "conversions.h"
#include "debugging.h"

typedef struct {
  Command command;
  char *name;
} DbgCommandName;

DbgCommandName commandNameTable[] = {
    {CMD_ASSIGN, "ASSIGN"},
    {CMD_CALL, "CALL"},
    {CMD_RETURN, "RETURN"},
    {CMD_DEFINE, "DEFINE"},
    {CMD_CODE_BLOCK, "CODE_BLOCK"},
    {CMD_GET, "GET"},
    {CMD_SET, "SET"},
    {CMD_ADD, "ADD"},
    {CMD_SUB, "SUB"},
    {CMD_MULT, "MULT"},
    {CMD_DIV, "DIV"},
};

char *tokenToText(ssize_t token) {
  for (int i = 0; i < tokenConversionsCount; i++) {
    if (tokenConversions[i].token == token) {
      if (strcmp(tokenConversions[i].match, "\n") == 0) {
        return "TOK_NEWLINE";
      }
      return tokenConversions[i].match;
    }
  }

  if (token <= 0) {
    return "LITERAL";
  }

  return "UNKNOWN";
}

void _indent(size_t count) {
  for (size_t i = 0; i < count; i++) {
    printf(" ");
  }
}

void prettyPrintTokens(ssize_t *tokens, size_t count, short isSlice) {
  printf("--- Pretty printing tokens ---\n");
  if (!isSlice && tokens[count - 1] != TOK_END) {
    printf("FATAL: Pretty: Main token list is not terminated.\n");
    exit(1);
  }
  for (ssize_t i = 0; i < count - 1; i++) {
    ssize_t token = tokens[i];

    if (token == TOK_END) {
      printf("FATAL: Pretty: Found multiple instances of TOK_END.\n");
      exit(1);
    }

    if (token <= 0) {
      printf("LITERAL<%zi, %s, %s>", tokens[i], "type_unk", "name_unk");
    } else {
      char *txt = tokenToText(token);
      printf("%s", txt);
    }

    if (token == TOK_SEMICOLON) {
      printf("\n");
    } else {
      printf(" ");
    }
  }
}

char *cmdToText(Command command) {
  for (int i = 0; i < tokenConversionsCount; i++) {
    if (commandNameTable[i].command == command) {
      return commandNameTable[i].name;
    }
  }

  return "ERR_UNKNOWN";
}

void _prettyPrintAst(SyntaxNode *node, char **literals, size_t nesting) {
  _indent(nesting);
  printf("- command %s\n", cmdToText(node->command));
  for (size_t i = 0; i < node->rhsCount; i++) {
    if (node->rhsNodes[i].type == RHS_TYPE_SYNTAX_NODE) {
      _prettyPrintAst(node->rhsNodes[i].value.rhsNode, literals, nesting + 1);
    } else {
      _indent(nesting + 1);
      printf("- %s\n", literals[node->rhsNodes[i].value.rhsLiteralRef]);
    }
  }
}

void prettyPrintAst(SyntaxNode *root, char **literals) {
  printf("--- Pretty printing AST ---\n");
  _prettyPrintAst(root, literals, 0);
}

