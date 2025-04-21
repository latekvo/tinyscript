#define _GNU_SOURCE

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "ast.h"
#include "conversions.h"
#include "debugging.h"
#include "separators.h"
#include "tokens.h"

// TODO: shrink to longest CommandRuleset token series

// all separators except '\n' and ' ' are kept in place
// precedence set by array ordering

Token stringLiteralsTokens[] = {
    TOK_QUOTE_SINGLE,
    TOK_QUOTE_DOUBLE,
    TOK_QUOTE_TICK,
};

Token *extractFirstTokens(char **str, char **literal) {
  // returning first two tokens - an expression and separator or newline token
  // if no separator is present returned tokens are removed from str by shifting
  // str ptr by returned tokens' length
  Token *tokens = malloc(sizeof(Token) * 2);

  size_t spanToClosestSep = strlen(*str) - 1;
  char *closestSepString = "\n";

  for (size_t i = 0; i < syntaxUnitsSeparatorsCount; i++) {
    char *separator = syntaxUnitsSeparators[i];

    // fixme: we cannot just lookup seps, we have to find the first one
    char *foundSep = strstr(*str, separator);

    if (foundSep == NULL) {
      continue;
    }

    size_t nToSep = foundSep - *str;

    if (nToSep >= spanToClosestSep) {
      // only keeping the closest separator
      continue;
    }

    spanToClosestSep = nToSep;
    closestSepString = separator;
  }

  char tokenBuf[spanToClosestSep + 1];
  strncpy(tokenBuf, *str, spanToClosestSep);
  tokenBuf[spanToClosestSep] = '\0';

  tokens[0] = strToToken(tokenBuf);
  tokens[1] = strToToken(closestSepString);

  if (tokens[0] == TOK_LITERAL) {
    // heap is slow, only allocating when needed
    *literal = malloc(spanToClosestSep + 1);
    strcpy(*literal, tokenBuf);
  }

  // remove from input
  *str += spanToClosestSep + strlen(closestSepString);

  return tokens;
}

// - Transpile to C for now, compile to opcodes in future.
// - Figure out entry points, hooking up to JSI could work.
// End goal:
// - 1. Compile JS to executable binaries.
// - 2. Convert TSX apps to binaries + minimal TS.

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

  // cleanup tokens - overwriting the existing mempool
  size_t updatedTokensHead = 0;

  size_t rHead = 0;
  for (size_t wHead = 0; wHead < tokensHead; wHead++) {
    // skip unwanted tokens
    // note: could optimize	by ANDing a bitmask
    // note: could optimize by moving logic to token extraction

    printf("Token cleanup: \"%s\", (%li)\n", tokenToText(tokens[wHead]),
           tokens[wHead]);

    updatedTokensHead = wHead + 1;
  }

  // convert tokens to commands
  for (int i = 0; i < updatedTokensHead; i++) {
    // could also simplify code in this step, by unfolding all statements,
    // deduplicating, and individually optimizing them, then folding them back
    // in the AST init step
  }

  // TODO: verify if not potential OOR UB
  tokens[updatedTokensHead] = TOK_END;

  // convert commands to flat AST

  // Single-pass is theoritically possible, but much more complex.
  // Basically, we have to store incomplete commands and defer them.
  // But it's not so bad if we're live-building the AST, as the empty
  // fields are immidiately filled in with the very next tokens.

  // fixme: multiline statements not supported yet
  // TODO: implement single-passing nested statements
  // 			 complex RHS can be left as TOK_BLANK, we can then track
  // 			 all TOK_BLANK, and substitute them sequentially
  // 			 since we're performing a BFS
  SyntaxNode *astRoot = constructSyntaxTree(tokens, updatedTokensHead, NULL);

  free(tokens);

  // beta-reduce AST - temporarily omitted

  // convert AST to C

  for (size_t i = 0; i < literalsHead; i++) {
    free(literals[i]);
  }
  free(literals);

  freeSyntaxTree(astRoot);

  fclose(file);
  return 0;
}
