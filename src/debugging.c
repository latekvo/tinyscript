#include <string.h>

#include "conversions.h"
#include "debugging.h"
#include "tokens.h"

char *tokenToText(Token token) {
  // debug utility
  for (int i = 0; i < tokenConversionsCount; i++) {
    if (tokenConversions[i].token == token) {
      if (strcmp(tokenConversions[i].match, "\n") == 0) {
        return "TOK_NEWLINE";
      }
      return tokenConversions[i].match;
    }
  }
  return "TOK_LITERAL or UNKNOWN";
}
