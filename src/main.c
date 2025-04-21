#define _GNU_SOURCE

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "ast.h"
#include "separators.h"
#include "tokens.h"

int main(int argc, char **argv) {
  if (argc == 1) {
    printf("You have to provide entrypoint filename. Terminating.\n");
    return -1;
  }

  FILE *file = fopen(argv[1], "r");
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  size_t tokensHead = 0;
  size_t tokensSize = 1024;
  ssize_t *tokens = malloc(sizeof(Token) * tokensSize);

  // note: array of malloc ptrs, free each member
  ssize_t literalsHead = 0;
  size_t literalsSize = 64;
  char **literals = malloc(sizeof(char *) * literalsSize);

  if (tokens == NULL) {
    perror("Token serialization: malloc failed. Aborting");
    exit(1);
  }

  printf("Token serialization: tokens allocating %zu\n", tokensSize);
  printf("Token serialization: literals allocating %zu\n", literalsSize);

  while ((nread = getline(&line, &len, file)) != -1) {
    // ptr copy allows slicing off extracted tokens
    char *linePtr = line;

    while (*linePtr != '\0') {
      char *literal = NULL;
      Token *newToks = extractFirstTokens(&linePtr, &literal);

      if (newToks[0] == TOK_LITERAL) {
        literals[literalsHead] = literal;
        tokens[tokensHead++] = -literalsHead; // literal's ref
        literalsHead++;
      } else if (newToks[0] != TOK_BLANK) {
        tokens[tokensHead++] = newToks[0];
      }

      if (newToks[1] != TOK_SPACE) {
        tokens[tokensHead++] = newToks[1];
      }

      if (newToks[1] == TOK_EOL) {
        free(newToks);
        break;
      }

      free(newToks);

      // 2 byte padding is required for TOK_END and potential early break
      if (tokensHead >= tokensSize - 2) {
        printf("Token serialization: contains %zu, reallocating %zu -> %zu\n",
               tokensHead, tokensSize, tokensSize * 2);
        tokensSize *= 2;
        tokens = reallocarray(tokens, tokensHead, tokensSize);

        if (tokens == NULL) {
          perror("Token serialization: reallocarray failed. Aborting");
          free(line);
          exit(1);
        }
      }
    };
  }

  free(line);

  tokens[tokensHead] = TOK_END;

  // Step: Tokens to AST

  SyntaxNode *astRoot = constructSyntaxTree(tokens, tokensHead, NULL);

  free(tokens);

  // Step: beta-reduce AST

  for (; 0;) {
    // 1. collect all literals refs
    // 2. hoist collected literals where possible
  }

  // Step: AST to C

  for (size_t i = 0; i < literalsHead; i++) {
    free(literals[i]);
  }
  free(literals);

  freeSyntaxTree(astRoot);

  fclose(file);
  return 0;
}
