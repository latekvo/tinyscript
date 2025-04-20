#include <string.h>

#include "conversions.h"

TokenConversion tokenConversions[] = {{"\n", TOK_EOL},
                                      {" ", TOK_SPACE},
                                      {";", TOK_SEMICOLON},
                                      {".", TOK_FULL_STOP},
                                      {",", TOK_COMMA},
                                      {":", TOK_COLON},
                                      {"[", TOK_L_SQ_BRACKET},
                                      {"]", TOK_R_SQ_BRACKET},
                                      {"{", TOK_L_CU_BRACKET},
                                      {"}", TOK_R_CU_BRACKET},
                                      {"(", TOK_L_PN_BRACKET},
                                      {")", TOK_R_PN_BRACKET},
                                      {"'", TOK_QUOTE_SINGLE},
                                      {"\"", TOK_QUOTE_DOUBLE},
                                      {"`", TOK_QUOTE_TICK},
                                      {"=", TOK_ASSIGN},
                                      {"return", TOK_RETURN},
                                      {"==", TOK_EQ},
                                      {"===", TOK_EEQ},
                                      {"!==", TOK_NEEQ},
                                      {">", TOK_GT},
                                      {"<", TOK_LT},
                                      {">=", TOK_GE},
                                      {"<=", TOK_LE},
                                      {"&&", TOK_AND},
                                      {"||", TOK_OR},
                                      {"+", TOK_ADD},
                                      {"-", TOK_SUB},
                                      {"*", TOK_MULT},
                                      {"/", TOK_DIV},
                                      {"**", TOK_EXP},
                                      {"%", TOK_MOD},
                                      {"true", TOK_TRUE},
                                      {"false", TOK_FALSE},
                                      {"null", TOK_NULL},
                                      {"undefined", TOK_UNDEFINED},
                                      {"!", TOK_NEGATION},
                                      {"++", TOK_INC},
                                      {"--", TOK_DEC},
                                      {"do", TOK_DO},
                                      {"for", TOK_FOR},
                                      {"while", TOK_WHILE},
                                      {"continue", TOK_CONTINUE},
                                      {"break", TOK_BREAK},
                                      {"in", TOK_IN},
                                      {"of", TOK_OF},
                                      {"if", TOK_IF},
                                      {"else", TOK_ELSE},
                                      {"case", TOK_CASE},
                                      {"switch", TOK_SWITCH},
                                      {"class", TOK_CLASS},
                                      {"new", TOK_NEW},
                                      {"delete", TOK_DELETE},
                                      {"default", TOK_DEFAULT},
                                      {"final", TOK_FINAL},
                                      {"this", TOK_THIS},
                                      {"extends", TOK_EXTENDS},
                                      {"super", TOK_SUPER},
                                      {"protected", TOK_PROTECTED},
                                      {"implements", TOK_IMPLEMENTS},
                                      {"public", TOK_PUBLIC},
                                      {"static", TOK_STATIC},
                                      {"private", TOK_PRIVATE},
                                      {"interface", TOK_INTERFACE},
                                      {"instanceof", TOK_INSTANCEOF},
                                      {"catch", TOK_CATCH},
                                      {"finally", TOK_FINALLY},
                                      {"throws", TOK_THROWS},
                                      {"try", TOK_TRY},
                                      {"throw", TOK_THROW},
                                      {"typeof", TOK_TYPEOF},
                                      {"void", TOK_VOID},
                                      {"const", TOK_CONST},
                                      {"function", TOK_FUNCTION},
                                      {"enum", TOK_ENUM},
                                      {"var", TOK_VAR},
                                      {"let", TOK_LET},
                                      {"await", TOK_AWAIT},
                                      {"yield", TOK_YIELD},
                                      {"eval", TOK_EVAL},
                                      {"debugger", TOK_DEBUGGER},
                                      {"with", TOK_WITH},
                                      {"import", TOK_IMPORT},
                                      {"package", TOK_PACKAGE},
                                      {"export", TOK_EXPORT}};

size_t tokenConversionsCount =
    sizeof(tokenConversions) / sizeof(tokenConversions[0]);

Token strToToken(char *str) {
  if (strcmp(str, "") == 0) {
    return TOK_BLANK;
  }

  for (int i = 0; i < tokenConversionsCount; i++) {
    if (strcmp(str, tokenConversions[i].match) == 0) {
      return tokenConversions[i].token;
    }
  }

  // todo: return literal as string in union with token
  //    	 alternatively - add it to a new runtime token, store globally,
  //    access later
  // note: we have to attach metadata to literals - their type, ...?
  // note: "literal" in this context could also be a LHS key, it's anything but
  // a keyword
  return TOK_LITERAL;
}
