#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <ctime>
#define KEYWORD 10
#define OPERATOR 20
#define IDENTIFIER 30
#define DIGIT 40
using namespace std;

char c;

int s=1;
int ident=1;
int digit=1;
int key=1;

//**//
int M=0,N=0;
char str[100];

FILE *identifier_file;
FILE *symbol_file;
FILE *digit_file;
FILE *keyword_file;

char keyword[50][10];
char operator1[50][5];
int yylex();

void operator_input()
{
	int i=0;
	freopen ("operator.txt","r",stdin);
	char str[1000];
	scanf("%s",str);
	char * pch;
	pch = strtok (str,"|");
	N++;
	while (pch != NULL)
	{
		strcpy(operator1[i++],pch);
		pch = strtok (NULL, "|");
		N++;
	}
	N--;
	for(int k=0;k<N;k++)
		cout << operator1[k]<<"\t";
	cout << endl;
	fclose(stdin);
}

void keyword_input()
{
	int i=0;
	freopen ("keyword_input.txt","r",stdin);
	char str[1000];
	scanf("%s",str);
	char * pch;
	pch = strtok (str,"|");
	M++;
	while (pch != NULL)
	{
		strcpy(keyword[i++],pch);
		pch = strtok (NULL, "|");
		M++;
	}
	M--;
	for(int k=0;k<M;k++)
		cout << keyword[k]<<"\t";
	cout << endl;
	fclose(stdin);
}

int find_column(char c)
{
	int len =strlen(char_array);
	for(int i=0;i<len;i++)
	{
		if(char_array[i]==c)
			return i;
	}
	return -1;
}

void symbol_table()
{
	operator_input();
	keyword_input();
	
/*	for(int k=0;k<strlen(char_array);k++)
		cout << char_array[k]<<"\t";
	cout << endl;
*/
	int token=1;
	
	identifier_file = fopen ("identifier.txt","w");
	symbol_file = fopen ("symbol.txt","w");
	digit_file = fopen ("digit.txt","w");
	keyword_file = fopen ("keyword.txt","w");
	
	fputs("Identifier Number\tIdentifier\n",identifier_file);
	fputs("Symbol Number\tSymbol\n",symbol_file);
	fputs("Digit Number\tDigit\n",digit_file);
	fputs("Keyword Number\tKeyword\n",keyword_file);
	
	freopen ("file.txt","r",stdin);
	while(token!=0)                                 //while something legit is returned
	{
		token = yylex();
		//cout<< token<<endl;
		if(c=='^')
		{
			fclose(stdin);
			fclose (identifier_file);
			fclose (symbol_file);
			fclose (digit_file);
			fclose (keyword_file);
			exit(1);
		}
	}
}

int flag_count=0;

int yylex()
{
	int current_state=0,prev_state,i=0,j=0,comment=0;

	if(flag_count==0)                                                               //reading the first character from the user
	{
		flag_count = 1;
		//cout << "take input\n";
		c=getchar();
	}
	
	while(1)
	{
		str[i++]=c;
		if(c=='^')                                                             //if the end of input is reached
		{
			break;
		}
		prev_state = current_state;
		//sleep(1);
		if(c==' '|| c=='\n')
		{
			flag_count=0;
			break;
		}
		//cout << "find_column(c) : "<<find_column(c)<<endl;
		if(find_column(c)==-1)
			current_state=-1;
		else
			current_state=transition_table[current_state][find_column(c)];
		if(current_state==-1)
			break;
		//cout << "take input\n";
		c=getchar();
		cout <<"c : "<<c<<endl;
	}
	str[i-1]='\0';
	cout <<"str : "<< str<<endl;
	for(int i=0;i<M;i++)
	{
		if(strcmp(str,keyword[i])==0)
		{
			fprintf(keyword_file,"%d\t%s\n",key++,str);
			return 1;
		}
	}
	
	for(int j=0;j<N;j++)
	{
		if(strcmp(str,operator1[j])==0)
		{
			fprintf(symbol_file,"%d\t%s\n",s++,str);
			return 1;
		}
	}
	
	if((str[0]>='a' && str[0]<='z')||(str[0]>='A' && str[0]<='Z'))
	{
		fprintf(identifier_file,"%d\t%s\n",ident++,str);
		return 1;
	}
	else if(str[0]>='0' && str[0]<='9')
	{
		fprintf(digit_file,"%d\t%s\n",digit++,str);
		return 1;
	}
	else
	{
		cout << "ERROR\n";
	}	
}
