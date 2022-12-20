lex.l
/*lex program to count number of words*/
%{
#include<stdio.h>
#include<string.h>
int i = 0;
%}
/* Rules Section*/
%%
([a-zA-Z0-9])* {i++;} /* Rule for counting
number of words*/
"\n" {printf("%d\n", i); i = 0;}
%%
int yywrap(void){}
int main()
{
// The function that starts the analysis
yylex();
return 0;
}
OUTPUT
sngce@sngce-OptiPlex-3000:~$ cd Desktop
sngce@sngce-OptiPlex-3000:~/Desktop$ lex lex.l
sngce@sngce-OptiPlex-3000:~/Desktop$ gcc lex.yy.c
sngce@sngce-OptiPlex-3000:~/Desktop$ ./a.out
How are you where are u from
7
