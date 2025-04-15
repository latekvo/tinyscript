#include <stdio.h>

enum Tokens {
	// separators 
	EOL = 0, // \n
	SPACE, // discard on read
	SEMICOLON,
	FULL_STOP,
	COMMA,
	COLON,
	L_SQ_BRACKET, // []
	R_SQ_BRACKET,
	L_CU_BRACKET, // {}
	R_CU_BRACKET,
	L_PN_BRACKET, // ()
	R_PN_BRACKET,
	QUOTE_SINGLE, // ''
	QUOTE_DOUBLE, // ""
	QUOTE_TICK, // ``

	// LHS with RHS operators
	TOK_ASSIGN, // =
	TOK_RETURN, // return;	

	// RHS with RHS operators
	TOK_EQ, // ==
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
};

struct TokenConverion {
	char* match;
	enum Tokens token;
};

enum Command {
	ASSIGN = 0,
	CALL, 
	GET,
	SET,
	ADD,
	SUB,
	MULT,
	DIV,
};

// Rulesets for given command
// - token matches
// - resulting Command
struct CommandRuleset {
	
};

// each AST node has a precedence index, a parent, 
// - LHS (parent), RHS list,
struct SyntaxNode {
	enum Command command;
	struct SyntaxNode *lhs; // single parent
	struct SyntaxNode *rhs; // list of children
};

// Game plan:
// - Convert raw text to easily interpretable transitory form.
// 	 I plan to remove this step once it's determined unnecessary/slow.
// - Convert the transitory form into semi-flat AST.
// - Beta-reduce AST.
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
	
	while ((nread = getline(&line, &len, file)) != -1) {
		printf("%s", line);
	}

	
	fclose(file);
	return 0;
}
