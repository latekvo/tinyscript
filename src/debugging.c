#include <stdio.h>
#include <string.h>

#include "ast.h"
#include "conversions.h"
#include "debugging.h"

char *tokenToText(ssize_t token) {
  // debug utility
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

void _prettyPrintAst(SyntaxNode *node, char **literals, size_t nesting) {
  _indent(nesting);
  printf("- command %u\n", node->command);
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
  _prettyPrintAst(root, literals, 0);
}

