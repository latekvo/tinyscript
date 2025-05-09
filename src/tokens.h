#pragma once

typedef enum {
  // internal tokens:
  TOK_LITERAL = 0, // TOK_LITERAL must be first
  TOK_LHS_GROUP,   // ruleset matching: LHS
  TOK_RHS_GROUP,   // ruleset matching: RHS
  TOK_END,         // ruleset custom series definition & iter terminator
  TOK_BLANK,

  // userspace tokens:

  // separators
  TOK_EOL,   // \n
  TOK_SPACE, // discard on read
  TOK_SEMICOLON,
  TOK_FULL_STOP,
  TOK_COMMA,
  TOK_COLON,
  TOK_L_SQ_BRACKET, // []
  TOK_R_SQ_BRACKET,
  TOK_L_CU_BRACKET, // {}
  TOK_R_CU_BRACKET,
  TOK_L_PN_BRACKET, // ()
  TOK_R_PN_BRACKET,
  TOK_QUOTE_SINGLE, // ''
  TOK_QUOTE_DOUBLE, // ""
  TOK_QUOTE_TICK,   // ``

  // LHS with RHS operators
  // note: assignment-modification operators are derived at runtime
  TOK_ASSIGN, // =

  // RHS with RHS operators
  TOK_EEQ,  // ===
  TOK_EQ,   // ==
  TOK_NEEQ, // !==
  TOK_NEQ,  // !=
  TOK_GT,   // >
  TOK_LT,   // <
  TOK_GE,   // >=
  TOK_LE,   // <=
  TOK_AND,  // &&
  TOK_OR,   // ||
  TOK_ADD,  // +
  TOK_SUB,  // -
  TOK_MULT, // *
  TOK_DIV,  // /

  // RHS with RHS operators - unplanned
  TOK_EXP,           // **
  TOK_MOD,           // %
  TOK_B_OR,          // |
  TOK_B_AND,         // &
  TOK_B_XOR,         // ^
  TOK_B_NOT,         // ~
  TOK_B_LSHIFT,      // <<
  TOK_B_RSHIFT,      // >>
  TOK_B_RSHIFT_FILL, // >>>

  // RHS values
  TOK_TRUE,
  TOK_FALSE,
  TOK_NULL,
  TOK_UNDEFINED,

  // RHS modifiers
  TOK_NEGATION, // !
  TOK_INC,      // ++
  TOK_DEC,      // --

  // RHS operations
  TOK_RETURN,

  // loops
  TOK_DO,
  TOK_FOR,
  TOK_WHILE,
  TOK_CONTINUE,
  TOK_BREAK,
  TOK_IN,
  TOK_OF,

  // conditionals
  TOK_IF,
  TOK_ELSE,
  TOK_CASE,
  TOK_SWITCH,

  // classes - unplanned
  TOK_CLASS,
  TOK_NEW,
  TOK_DELETE,
  TOK_DEFAULT,
  TOK_FINAL,
  TOK_THIS,
  TOK_EXTENDS,
  TOK_SUPER,
  TOK_PROTECTED,
  TOK_IMPLEMENTS,
  TOK_PUBLIC,
  TOK_STATIC,
  TOK_PRIVATE,
  TOK_INTERFACE,
  TOK_INSTANCEOF,

  // error handling - unplanned
  TOK_CATCH,
  TOK_FINALLY,
  TOK_THROWS,
  TOK_TRY,
  TOK_THROW,

  // types - unplanned
  TOK_TYPEOF,
  TOK_VOID,

  // definition keywords
  TOK_CONST,
  TOK_FUNCTION,
  TOK_ENUM,
  TOK_VAR,
  TOK_LET,

  // async - unplanned
  TOK_AWAIT,
  TOK_YIELD,

  // utilities - unplanned
  TOK_EVAL,
  TOK_DEBUGGER,
  TOK_WITH,

  // linking - unplanned
  TOK_IMPORT,
  TOK_PACKAGE,
  TOK_EXPORT,
} Token;

