/************  PL0.c  *************/

// pl0 compiler source code

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pl0.h"

//////////////////////////////////////////////////////////////////////
// print error message.
/* TO BE MODIFIED */
void error(int n)	//declare n as int，or there would be warning
{
	int i;

	printf("      ");
	for (i = 1; i <= cc - 1; i++)
		printf(" ");
	printf("^\n");
	printf("Error %3d: %s\n", n, err_msg[n]);
	err++;
} // error

//////////////////////////////////////////////////////////////////////
void getch(void)
{
	if (cc == ll)
	{
		/* TO BE MODIFIED */
		printf("\n");    //
		if (feof(infile))
		{
			printf("\nPROGRAM INCOMPLETE\n");
			system("pause");
			exit(1);
		}
		ll = cc = 0;
		/* TO BE MODIFIED */
		printf("%5d  ", ++cx);    //change index of current instruction
		while (!feof(infile) && (ch = getc(infile))!='\n')
		{
			printf("%c", ch);
			/* TO BE MODIFIED */
			line[++ll] = ch;
		} // while
	
		printf("\n");
		line[++ll] = ' ';
	}
	ch = line[++cc];
} // getch

//////////////////////////////////////////////////////////////////////
// gets a symbol from input stream.
void getsym(void)
{
	int i, k;
	char a[MAXIDLEN + 1];

	while (ch == ' ' || ch=='\t')
		getch();

	if (isalpha(ch))
	{ // symbol is a reserved word or an identifier.
		k = 0;
		do
		{
			if (k < MAXIDLEN)
				a[k++] = ch;
			getch();
		}
		while (isalpha(ch) || isdigit(ch));
		a[k] = 0;
		strcpy(id, a);
		word[0] = id;
		i = NRW;
		while (strcmp(id, word[i--]));
		if (++i)
			sym = wsym[i]; // symbol is a reserved word
		else
			sym = SYM_IDENTIFIER;   // symbol is an identifier
		
	}
	else if (isdigit(ch))
	{ // symbol is a number.
		k = num = 0;
		sym = SYM_NUMBER;
		do
		{
			num = num * 10 + ch - '0';
			k++;
			getch();
		}
		while (isdigit(ch));
		if (k > MAXNUMLEN)
			error(25);     // The number is too great.
	}
	else if (ch == ':')
	{
		getch();
		if (ch == '=')
		{
			sym = SYM_BECOMES; // :=
			/* TO BE MODIFIED */
			count[0] ++;
			getch();	
		}
		else
		{
			sym = SYM_NULL;       // illegal?
		}
	}
	else if (ch == '>')
	{
		getch();
		if (ch == '=')
		{
			sym = SYM_GEQ;     // >=
			getch();
		}
		else
		{
			sym = SYM_GTR;     // >
			/* TO BE MODIFIED */
			count[1] ++;
		}
	}
	else if (ch == '<')
	{
		/* TO BE MODIFIED */
		getch();
		if (ch == '='){
			sym = SYM_LEQ;		// <=
			getch();
		}
		else if (ch == '>'){
			sym = SYM_NEQ;		// <>
			/* TO BE MODIFIED */
			count[3] ++;
			getch();
		}
		else{
			sym = SYM_LES;		// <
			/* TO BE MODIFIED */
			count[2] ++;
		}
	}
	
	/* TO BE MODIFIED */
	/* Skip  Notes*/
	else if (ch == '('){
		getch();
		if (ch == '*'){		// (*
			getch();
			while (ch != '*'){
				getch();
			}
			getch();		// )
			sym = SYM_NULL; //take notes as null
			getch();
		}
		else{
			sym = SYM_LPAREN;
		}
	}
	
	else
	{ // other tokens
		i = NSYM;
		csym[0] = ch;
		while (csym[i--] != ch);
		if (++i)
		{
			sym = ssym[i];
			if (sym != SYM_PERIOD)
			{
				getch();
			}
		}
		else
		{
			printf("Fatal Error: Unknown character.\n");
			system("pause");
			exit(1);
		}
	}
} // getsym

//////////////////////////////////////////////////////////////////////
int main (void)
{
	char s[80];
	printf("Please input source file name: "); // get file name to be compiled
	scanf("%s", s);

	// //test code
	// char s[80] = "test1.txt";

	if ((infile = fopen(s, "r")) == NULL)
	{
		printf("File %s can't be opened.\n", s);
		system("pause");
		exit(1);
	}
	
	err = cc = cx = ll = 0; // initialize global variables
	ch = ' ';
	kk = MAXIDLEN;

	while(ch != '.')
	{   
		getsym();  
		
		/* TO BE MODIFIED */
	    /* Please output types of words in every line */  
		printf("%d ", sym);	
    }

	if (sym != SYM_PERIOD)
		error(9); // '.' expected.

    /* TO BE MODIFIED */
    /* Please output the number of “:=”，“>”，“<”，“<>”  to file "test.out"*/
	char outpath[80] = "";
	char counts[100] = "";
	char tmp[20];
	char *ptr;
	//strtok() to split char，in order to create the output file (<input file name>.out)
	ptr = strtok(s, ".");
	strcat(outpath, ptr);
	strcat(outpath, ".out");
	FILE* outfile = fopen(outpath, "w");
	for (int i=0; i<4; i++){
		strcat(counts, count_sym[i]);
		strcat(counts, " count is ");
		itoa(count[i], tmp, 10);	//int -> char
		strcat(counts, tmp);
		strcat(counts, "\n");
	}
	fwrite(counts, sizeof(char), sizeof(counts), outfile);
	fclose(outfile);

	//add system("pause") to prevent the shell window from closing automatically
	system("pause");	
	return 0;
} // main    END OF PL0.c
