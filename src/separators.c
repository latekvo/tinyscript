#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "conversions.h"
#include "separators.h"

char *syntaxUnitsSeparators[] = {
    // "\n", redundant to existing logic
    " ",  ";",  "===", "!==", "**=", "==", "!=", ">=", "<=", "&&", "||", "++",
    "--", "**", "+=",  "-=",  "*=",  "/=", "%=", ",",  ":",  "[",  "]",  "{",
    "}",  "(",  ")",   "=",   ">",   "<",  "+",  "-",  "*",  "/",  "%",  "!",
    // disabled dot until classes are implemented
    // ".",
    // explicit string are to be detected, templates are to be evaluated
    // separately
    // "'",
    // "\"",
    // "`",
};

size_t syntaxUnitsSeparatorsCount =
    sizeof(syntaxUnitsSeparators) / sizeof(syntaxUnitsSeparators[0]);

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
