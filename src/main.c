#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "tokens.h"
#include "separators.h"
#include "conversions.h"

// all separators except '\n' and ' ' are kept in place
// precedence set by array ordering

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
	
	for (int i = 0; i < tokenConversionsCount; i++) {
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
	Token *tokens = malloc(sizeof(Token) * 2);
	
	size_t spanToClosestSep = strlen(*str) - 1;
	char* closestSepString = "\n";

	for (size_t i = 0; i < syntaxUnitsSeparatorsCount; i++) {
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
		// ptr copy allows slicing off extracted tokens
		char *linePtr = line;	

		while (*linePtr != '\0') {
			Token *newToks = extractFirstTokens(&linePtr);
	
			if (newToks[0] != TOK_BLANK) {
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

			if (tokensHead >= tokensSize - 2) {
				printf("Token serialization: contains %zu, reallocating %zu -> %zu\n", tokensHead, tokensSize, tokensSize * 2);		
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
	free(tokens);

	// cleanup tokens
	
	// convert tokens to flat AST

	// beta-reduce AST - temporarily omitted
	
	// convert AST to C

	fclose(file);
	return 0;
}
