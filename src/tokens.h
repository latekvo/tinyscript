#pragma once

typedef enum {
	// internal util, do not treat as token
	TOK_BLANK = 0,
	TOK_LITERAL,

	// separators 
	TOK_EOL, // \n
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
	TOK_QUOTE_TICK, // ``

	// LHS with RHS operators
	TOK_ASSIGN, // =
	TOK_RETURN, // return;	

	// RHS with RHS operators
	TOK_EEQ, // ===
	TOK_EQ, // ==
	TOK_NEEQ, // !==
	TOK_NEQ, // !=
	TOK_GT, // >
	TOK_LT, // <
	TOK_GE, // >=
	TOK_LE, // <=
	TOK_AND, // &&
	TOK_OR, // ||
	TOK_ADD, // +
	TOK_SUB, // -
	TOK_MULT, // *
	TOK_DIV, // /
	TOK_EXP, // **
	TOK_MOD, // %

	// RHS values
	TOK_TRUE, 	
	TOK_FALSE,
	TOK_NULL,
	TOK_UNDEFINED,

	// RHS modifiers
	TOK_NEGATION, // !
	TOK_INC, // ++
	TOK_DEC, // --


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
