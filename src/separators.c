#include <stdlib.h>
#include <sys/types.h>

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

