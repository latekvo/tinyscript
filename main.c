#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct {
	char* match;
	Token token;
} TokenConversion;

// all separators except '\n' and ' ' are kept in place
// precedence set by array ordering
char *syntaxUnitsSeparators[] = {
	// "\n", redundant to existing logic
	" ", 
	";", 
	"===",
	"!==",
	"**=",
	"==",
	"!=",
	">=",
	"<=",
	"&&",
	"||",
	"++",
	"--",
	"**",
	"+=",
	"-=",
	"*=",
	"/=",
	"%=",
	".", 
	",", 
	":", 
	"[", 
	"]", 
	"{", 
	"}", 
	"(", 
	")", 
	"=", 
	">", 
	"<", 
	"+", 
	"-", 
	"*", 
	"/", 
	"%", 
	"!", 
	// explicit string are to be detected, templates are to be evaluated separately
	// "'", 
	// "\"",
	// "`", 
};

// todo: convert to hashmap, or use any other O(1) acc. method
TokenConversion tokenConversions[] = {
	{"\n", TOK_EOL}, 
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
	{"export", TOK_EXPORT}
};

Token stringLiteralsTokens[] = {
	TOK_QUOTE_SINGLE,
	TOK_QUOTE_DOUBLE,
	TOK_QUOTE_TICK
};

typedef enum {
	ASSIGN = 0,
	CALL, 
	GET,
	SET,
	ADD,
	SUB,
	MULT,
	DIV,
} Command;

// Rulesets for given command
// - token matches
// - resulting Command
typedef struct {
	
} CommandRuleset;

// each AST node has a precedence index, a parent, 
// - LHS (parent), RHS list,
typedef struct {
	Command command;
	struct SyntaxNode *lhs; // single parent
	struct SyntaxNode *rhs; // list of children
} SyntaxNode;

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

Token strToToken(char *str) {
	if (strcmp(str, "") == 0) {
		return TOK_BLANK;
	}
	
	static size_t convArraySize = sizeof(tokenConversions)/sizeof(tokenConversions[0]);
	
	for (int i = 0; i < convArraySize; i++) {
		if (strcmp(str, tokenConversions[i].match) == 0) {
			return tokenConversions[i].token;
		}
	}

	// todo: return literal as string in union with token
	//    	 alternatively - add it to a new runtime token, store globally, access later
	// note: we have to attach metadata to literals - their type, ...?
	// note: "literal" in this context could also be a LHS key, it's anything but a keyword 
	return TOK_LITERAL;
}

Token* extractFirstTokens(char **str) {
	// returning first two tokens - an expression and separator or newline token if no separator is present
	// returned tokens are removed from str by shifting str ptr by returned tokens' length
	Token *tokens = (Token *) malloc(sizeof(Token) * 2);
	
	static size_t matchArrSize = sizeof(syntaxUnitsSeparators)/sizeof(syntaxUnitsSeparators[0]);

	size_t spanToClosestSep = strlen(*str) - 1;
	char* closestSepString = "\n";

	for (size_t i = 0; i < matchArrSize; i++) {
		char *separator = syntaxUnitsSeparators[i];

		// fixme: we cannot just lookup seps, we have to find the first one
		char* foundSep = strstr(*str, separator);
		
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
	
	printf("Str '%s' sep '%s'\n", tokenBuf, closestSepString);

	// remove from input
	*str += spanToClosestSep + strlen(closestSepString);

	return tokens;
}


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
	Token *tokens = malloc(sizeof(Token) * tokensSize);

	if (tokens == NULL) {
    perror("Token serialization: malloc failed. Aborting");
    exit(1);
 	}

	printf("Token serialization: allocating %zu\n", tokensSize);		

	while ((nread = getline(&line, &len, file)) != -1) {
		printf("Token serialization: line: %s", line);

		// ptr copy allows slicing off extracted tokens

		while (*line != '\0') {
			Token *newToks = extractFirstTokens(&line);
	
			if (newToks[0] != TOK_BLANK) {
				tokens[tokensHead++] = newToks[0];
			}

			if (newToks[1] != TOK_SPACE) {
				tokens[tokensHead++] = newToks[1];
			}

			free(newToks);

			if (newToks[1] == TOK_EOL) {
				break;
			}

			if (tokensHead >= tokensSize - 2) {
				printf("Token serialization: contains %zu, reallocating %zu -> %zu\n", tokensHead, tokensSize, tokensSize * 2);		
				tokensSize *= 2;
				tokens = reallocarray(tokens, tokensHead, tokensSize);
	
				if (tokens == NULL) {
					perror("Token serialization: reallocarray failed. Aborting");
					exit(1);
				}
			}
		};
	}

	free(tokens);
	
	fclose(file);
	return 0;
}
