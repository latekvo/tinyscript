#define _GNU_SOURCE

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "ast.h"
#include "debugging.h"
#include "parser.h"

// split "literals" into typedLiterals:
// - variables (non-value)
// - strings
// - numbers
// - objects
// utilize token pattern matching

int main(int argc, char **argv) {
  if (argc == 1) {
    printf("You have to provide entrypoint filename. Terminating.\n");
    return -1;
  }

  size_t tokensCount = 0;
  ssize_t *tokens = NULL;

  size_t literalsCount = 0;
  char **literals = NULL;

  // Step: Plaintext to Tokens

  tokenizeFile(&tokens, &tokensCount, &literals, &literalsCount, argv[1]);

  // Step: Patch Tokens

  for (size_t i = 0; i < tokensCount; i++) {
    // Steps which offload edge-case logic away from AST constructor
    // - in math ops, precedence has to be set using parenthesis
    // - statements have to be clearly isolated and terminated (semicolon)
    // - nested statements have to be wrapped by parentheses
  }

  // Step: Tokens to AST

  SyntaxNode *astRoot = constructSyntaxTree(tokens, tokensCount, NULL);

  prettyPrintAst(astRoot, literals);

  free(tokens);

  // Step: Beta-reduce AST - skipping for now

  for (; 0;) {
    // 1. collect all literals refs
    // 2. hoist collected literals where possible
  }

  // Step: AST to C

  for (; 0;) {
  }

  for (size_t i = 0; i < literalsCount; i++) {
    free(literals[i]);
  }
  free(literals);

  freeSyntaxTree(astRoot);

  return 0;
}
