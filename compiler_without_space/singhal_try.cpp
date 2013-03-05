#include <iostream>
#include <cstdlib>
#include "AG_RegEx.h"
#include "AG_RegEx.cpp"
using namespace std;

char c;

//**//
int M,N;



bool count;

char keyword[5][10]={"int","float","double","bool","extern"};



typedef union {
       char *string;
       int integer;
       float real;
} yylType;

yylType* yylP;

int find_column(char c)
{
	int len =strlen(char_array);
	for(int i=0;i<len;i++)
	{
		if(char_array[i]==c)
			return i;
	}
}

void symbol_table()
{
	count=0;
	while((token = yylex(yylP)))                                 //while something legit is returned
	{
		switch(type)                                                   //type represents the type of the token class
		{
			case 0 :
				printf("<%d,%d>\t",token,yylP->integer);
				break;

			case 1 :
				printf("<%d,\"%s\">\t",token,yylP->string);
				break;

			case 2 :
				printf("<%d,%d>\t",token,yylP->integer);		
				break;

			case 3 :
				printf("<%d,%f>\t",token,yylP->real);
				break;

			case 4 :
				printf("<%d,COMMENTS>\t",token);
				break;
		}
		if(yylP->integer==430)
			printf("\n");
	}
}

int yylex()
{
	int current_state=0,prev_state,i=0,j=0,comment=0;
	type = 0;

	if(count==0)                                                               //reading the first character from the user
	{
		count = 1;
		scanf("%c",&c);
	}
	
	while(1)
	{
		str[i++]=c;
		if(c == EOF)                                                             //if the end of input is reached
		{
			return 0;
		}
		
		prev_state = current_state;
		
		current_state=transition_table[current_state][find	_column(c)];
		if(current_state==-1)
			break;
		scanf("%c",&c);
		
	}
	str[i-1]='\0';
	
	for(int i=0;i<M;i++)
	{
		if(strcmp(str,keyword[i])==0)
		{
			return KEYWORD;
		}
	}
	
	for(int j=0;j<N;j++)
	{
		if(strcmp(str,operator[j])==0)
		{
			return OPERATOR;
		}
	}
	
	if((str[0]>='a' && str[0]<='z')||(str[0]>='A' && str[0]<='Z'))
	{
		return IDENTIFIER;
	}
	else if(atoi(str[0])>=0 && atoi(str[0])<=9)
	{
		return DIGIT;
	}
	else
	{
		cout << "ERROR\n";
	}
	
	
}
