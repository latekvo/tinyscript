#define _GNU_SOURCE

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "ast.h"
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

  // Step: Tokens to AST

  SyntaxNode *astRoot = constructSyntaxTree(tokens, tokensCount, NULL);

  free(tokens);

  // Step: Beta-reduce AST

  for (; 0;) {
    // 1. collect all literals refs
    // 2. hoist collected literals where possible
  }

  // Step: AST to C

  for (size_t i = 0; i < literalsCount; i++) {
    free(literals[i]);
  }
  free(literals);

  freeSyntaxTree(astRoot);

  return 0;
}
