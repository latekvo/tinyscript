#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "conversions.h"
#include "parser.h"
#include "separators.h"

void extractFirstTokens(Token results[2], char **str, char **literal) {
  // returning first two tokens - an expression and separator or newline token
  // if no separator is present returned tokens are removed from str by shifting
  // str ptr by returned tokens' length

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

  results[0] = strToToken(tokenBuf);
  results[1] = strToToken(closestSepString);

  if (results[0] == TOK_LITERAL) {
    // heap is slow, only allocating when needed
    *literal = malloc(spanToClosestSep + 1);
    strcpy(*literal, tokenBuf);
  }

  // remove from input
  *str += spanToClosestSep + strlen(closestSepString);
}

void tokenizeFile(ssize_t **tokens, size_t *tokensCount, char ***literals,
                  size_t *literalsCount, char *filename) {
  FILE *file = fopen(filename, "r");
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  size_t tokensSize = 1024;
  *tokens = malloc(sizeof(Token) * tokensSize);

  size_t literalsSize = 64;
  *literals = malloc(sizeof(char *) * literalsSize);

  if (tokens == NULL) {
    perror("Token serialization: malloc failed. Aborting");
    exit(1);
  }

  printf("Token serialization: tokens allocating %zu\n", tokensSize);
  printf("Token serialization: literals allocating %zu\n", literalsSize);

  Token tokenBuf[2];

  while ((nread = getline(&line, &len, file)) != -1) {
    // ptr copy allows slicing off extracted tokens
    char *linePtr = line;

    while (*linePtr != '\0') {
      char *literal = NULL;
      extractFirstTokens(tokenBuf, &linePtr, &literal);

      if (tokenBuf[0] == TOK_LITERAL) {
        // fixme: dynamically expand literals memory
        (*literals)[*literalsCount] = literal;

        (*tokens)[*tokensCount] = -(*literalsCount); // literal's ref
        (*literalsCount)++;
        (*tokensCount)++;
      } else if (tokenBuf[0] != TOK_BLANK) {
        (*tokens)[*tokensCount] = tokenBuf[0];
        (*tokensCount)++;
      }

      // todo: temporarily ignoring \n
      // 			 we won't be able to do that in the future
      if (tokenBuf[1] != TOK_SPACE && tokenBuf[1] != TOK_EOL) {
        (*tokens)[*tokensCount] = tokenBuf[1];
        (*tokensCount)++;
      }

      // 2 byte padding is required for TOK_END and potential early break
      if ((*tokensCount) >= tokensSize - 2) {
        printf("Token serialization: contains %zu, reallocating %zu -> %zu\n",
               *tokensCount, tokensSize, tokensSize * 2);
        tokensSize *= 2;
        *tokens = reallocarray(*tokens, *tokensCount, tokensSize);

        if (tokens == NULL) {
          perror("Token serialization: reallocarray failed. Aborting");
          free(line);
          exit(1);
        }
      }
    };
  }

  free(line);
  fclose(file);

  // note: necessary padding is present
  (*tokens)[(*tokensCount)++] = TOK_END;
}

