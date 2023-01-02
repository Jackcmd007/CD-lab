#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#define TEMP_SYM '~'

char s_list[20];
int s_count = 0;

char sym_indices[20], symtab[10][50], input[10][50];

char str[50];
int I = 0, N = 0;

void replaceAt(char *expr, int j, char *value, bool replace) // replace character of a string at given index with another string
{
	char temp[50] = "\0", end[50] = "\0";
	strncpy(temp, expr, j);
	strncpy(end, expr + j + 1, strlen(expr) - j);

	if (replace == true)
	{
		strcpy(end, value);
		int n = strlen(end);
		end[n] = ';';
		end[n + 1] = '\0';
	}
	else
		strcat(temp, value);

	strcat(temp, end);
	strcpy(expr, temp);
}

void propagateAt(int expr_index, int j, int valueIndex) { replaceAt(symtab[expr_index], j, symtab[valueIndex], false); }

void propagateAll(int expr_index)
{
	char sym;
	for (int i = 0; i < s_count; i++)
	{
		sym = s_list[i];
		// if sym found in expression
		for (int j = 0; j < strlen(symtab[expr_index]); j++)
		{
			if (symtab[expr_index][j] == sym)
			{
				//  at jth index copy sym's corresponding value
				propagateAt(expr_index, j, sym); // valueIndex is sym
			}
		}
	}
}

char getSymbolName(int t)
{
	while (t >= 0)
	{
		if (isalpha(input[I][t]))
			return input[I][t];
		t--;
	}
	printf("\nSyntax error: No symbol name before '='");
	exit(-1);
}

void getValue(int t)
{
	t += 1;
	strcpy(str, "");
	while (input[I][t] != ';')
	{
		strncat(str, &input[I][t], 1);
		t++;
	}
}

int RHS() // returns index of first ' ' (space) if not an expression, otherwise returns index of character after '='
{
	int j = 0;
	bool flag = false;
	int firstSpace = -1;
	while (input[I][j] != '\0')
	{
		if ((input[I][j] == ' ') && (firstSpace == -1))
			firstSpace = j;
		if (input[I][j] == '=')
		{
			flag = true;
			break;
		}
		j++;
	}
	if (flag == true)
		return j;
	return -1 * abs(firstSpace); // -ve values distinguish non-expressions from expressions
}

void transformCurrentInput()
{
	if (sym_indices[I] != '\0')
	{
		int r = abs(RHS()) + 1;
		replaceAt(input[I], r, symtab[sym_indices[I]], true);
	}
	else
	{
		for (int i = 0; i < I; i++) // for each previous symbol
		{
			if (sym_indices[i] == '\0')
				break;
			int r = abs(RHS()) + 1;
			// if symbol is in this rhs
			for (int j = r; j < strlen(input[I]); j++)
			{
				if (sym_indices[i] == input[I][j])
				{
					// replace each symbol in rhs with corresponding value in symtab
					replaceAt(input[I], j, symtab[sym_indices[i]], false);
				}
			}
		}
	}
}

char extractSymbols()
{
	int j = RHS();

	if (j > -1) // if and only if current line is an expression
	{
		char sym = getSymbolName(j);
		if (!isalpha(sym))
		{
			printf("\nInvalid Identifier : %c", sym);
			exit(-1);
		}

		s_list[s_count++] = TEMP_SYM;

		getValue(j);
		strcpy(symtab[TEMP_SYM], str);	   // temporarily add to symtab as TEMP_SYM
		propagateAll(s_list[s_count - 1]); // apply propagateAll

		s_list[s_count - 1] = sym;
		strcpy(symtab[sym], symtab[TEMP_SYM]); // rename symbol

		return sym;
	}

	return '\0';
}

void main()
{
	FILE *fp;
	char line[50];
	size_t len = 0;
	ssize_t read;

	fp = fopen("input.c", "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);

	printf("\nInput:\n");

	// Read lines from file and store in array 'input'
	//  while ((read = getline(&line, &len, fp)) != -1)
	while (fgets(line, 1000, fp))
	{
		strcpy(input[N], line);
		printf("%s", input[N++]);
	}

	printf("\n\nN : %d", N);

	// For each line
	for (I = 0; I < N; I++)
	{
		char c = extractSymbols(); // Extract symbols in each line to symbol table and apply propagation
		if (c != '\0')			   // For later use : '\0' means this line contains no symbol definition, otherwise yes
		{
			sym_indices[I] = c;
		}
		else
			sym_indices[I] = '\0';
		transformCurrentInput(); // modify input
	}

	printf("\n\nRead %d symbols:", s_count);
	for (int i = 0; i < s_count; i++)
		printf("\n%c : %s", s_list[i], symtab[s_list[i]]);

	printf("\n\nInputs after propagation:");
	for (int i = 0; i < N; i++)
		printf("\n%s", input[i]);

	fclose(fp);
}
