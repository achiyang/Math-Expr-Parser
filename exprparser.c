#include "exprparser.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

static void skipWhitespace(const char** expr) {
	if (expr == NULL || *expr == NULL) {
		return;
	}

	while (**expr != '\0' && isspace(**expr)) {
		(*expr)++;
	}
}

static Node* createNode() {
	Node* node = (Node*)malloc(sizeof(Node));
	if (node == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	return node;
}

static Node* createNumberNode(int number) {
	Node* node = createNode();
	node->type = 'n';
	node->data.number = number;
	return node;
}

static Node* createExpressionNode(char op, Node* left, Node* right) {
	Node* node = createNode();
	node->type = 'e';
	node->data.expression.op = op;
	node->data.expression.left = left;
	node->data.expression.right = right;
	return node;
}

static Node* expressionParser(const char** expr);
static Node* termParser(const char** expr);
static Node* factorParser(const char** expr);
static Node* numberParser(const char** expr);

Node* expressionParser(const char** expr) {
	skipWhitespace(expr);

	Node* left = termParser(expr);
	skipWhitespace(expr);

	while (**expr == '+' || **expr == '-') {
		char op = **expr;
		(*expr)++;

		Node* right = termParser(expr);

		Node* node = createExpressionNode(op, left, right);

		left = node;
		skipWhitespace(expr);
	}

	if (**expr != '\0' && **expr != ')') {
		fprintf(stderr, "Unexpected character at the end of expression: '%c'", **expr);
		exit(EXIT_FAILURE);
	}

	return left;
}

Node* termParser(const char** expr) {
	skipWhitespace(expr);

	Node* left = factorParser(expr);
	skipWhitespace(expr);

	while (**expr == '*' || **expr == '/') {
		char op = **expr;
		(*expr)++;
		Node* right = factorParser(expr);

		Node* node = createExpressionNode(op, left, right);

		left = node;
		skipWhitespace(expr);
	}

	return left;
}

Node* factorParser(const char** expr) {
	skipWhitespace(expr);

	if (isdigit(**expr)) {
		return numberParser(expr);
	}
	else if (**expr == '-') {
		(*expr)++;
		Node* temp = factorParser(expr);
		return createExpressionNode('*', temp, createNumberNode(-1));
	}
	else if (**expr == '(') {
		(*expr)++;
		Node* result = expressionParser(expr);
		skipWhitespace(expr);

		if (**expr == ')') {
			(*expr)++;
			return result;
		}
		else {
			fprintf(stderr, "Missing closing parenthesis\n");
			exit(EXIT_FAILURE);
		}
	}
	else {
		fprintf(stderr, "Unexpected character in factor: '%c'\n", **expr);
		exit(EXIT_FAILURE);
	}

	return NULL;
}

Node* numberParser(const char** expr) {
	int num = 0;

	while (isdigit(**expr)) {
		num = num * 10 + (**expr - '0');
		(*expr)++;
	}
	return createNumberNode(num);
}

Node* parser(const char* expression) {
	if (expression == NULL) {
		fprintf(stderr, "Invalid input expression (null pointer)\n");
		exit(EXIT_FAILURE);
	}

	Node* result = expressionParser(&expression);

	skipWhitespace(&expression);
	if (*expression != '\0') {
		fprintf(stderr, "Unexpected character at the end of expression: '%c'\n", *expression);
		exit(EXIT_FAILURE);
	}
	return result;
}

double evaluateTree(Node* root) {
	if (root == NULL) {
		fprintf(stderr, "Invalid expression tree (null pointer)\n");
		exit(EXIT_FAILURE);
	}

	if (root->type == 'n') {
		return (double)root->data.number;
	}
	switch (root->data.expression.op) {
	case '+':
		return evaluateTree(root->data.expression.left) + evaluateTree(root->data.expression.right);
		break;
	case '-':
		return evaluateTree(root->data.expression.left) - evaluateTree(root->data.expression.right);
		break;
	case '*':
		return evaluateTree(root->data.expression.left) * evaluateTree(root->data.expression.right);
		break;
	case '/':
		if (root->data.expression.right != 0) {
			return evaluateTree(root->data.expression.left) / evaluateTree(root->data.expression.right);
		}
		else {
			fprintf(stderr, "Divide zero error\n");
			exit(EXIT_FAILURE);
		}
		break;
	default:
		fprintf(stderr, "Unexpected operator: '%c'\n", root->data.expression.op);
		exit(EXIT_FAILURE);
	}
}

static void freeTreeCore(Node* node) {
	if (node->type == 'n') {
		free(node);
		return;
	}
	freeTreeCore(node->data.expression.left);
	freeTreeCore(node->data.expression.right);
	free(node);
}

void freeTree(Node** root) {
	if (root == NULL || *root == NULL) {
		fprintf(stderr, "Invalid expression tree (null pointer)\n");
		exit(EXIT_FAILURE);
	}
	freeTreeCore(*root);
	*root = NULL;
}