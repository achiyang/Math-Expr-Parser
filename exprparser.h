#ifndef EXPRPARSER_H
#define EXPRPARSER_H

typedef struct Node {
	char type; // 'n' for number, 'e' for expression
	union {
		int number;
		struct {
			char op;
			struct Node* left;
			struct Node* right;
		} expression;
	} data;
} Node;

Node* parser(const char* expression);

double evaluateTree(Node* root);

void freeTree(Node** root);

#endif // EXPRPARSER_H