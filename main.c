#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "exprparser.h"

int main() {
	char input[100];
	fgets(input, sizeof(input), stdin);

	Node* root = parser(input);
	double result = evaluateTree(root);
	printf("%lf\n", result);

	return 0;
}