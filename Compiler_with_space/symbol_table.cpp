#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <ctime>
#include "regex.h"
using namespace std;

//vector< vector<int> > accept_state_dfa;
//transition_table

int op=1;
int ident=1;
int constant=1;
int key=1;

FILE *identifier_file;
FILE *operator_file;
FILE *constant_file;
FILE *keyword_file;

vector<string> idents_strings;
vector<string> constants_strings;

int i,current_state;
bool flag;
char str[100];

int find_column(char c)
{
	int len =char_array.size();
	for(int i=0;i<input_char;i++)
	{
		if(char_array[i]==c)
			return i;
	}
	return -1;
}

int look(string s,bool b)
{
	if(b)
	{
		for(int i=0;i<constants_strings.size();i++)
		{
			if(s==constants_strings[i])
				return i+1;
		}
		return -1;
	}
	else
	{
		for(int i=0;i<idents_strings.size();i++)
		{
			if(s==idents_strings[i])
				return i+1;
		}
		return -1;
	}
}

void symbol()
{
	identifier_file = fopen ("identifier.txt","w");
	//operator_file = fopen ("operator.txt","w");
	constant_file = fopen ("constant.txt","w");
	//keyword_file = fopen ("keyword.txt","w");

	fputs("Identifier Number\tIdentifier\n",identifier_file);
	//fputs("Operator Number\tOperator\n",operator_file);
	fputs("Constant Number\tConstant\n",constant_file);
	//fputs("Keyword Number\tKeyword\n",keyword_file);

	freopen("c_in.txt","r",stdin);
	//freopen("out.txt","w",stdout);
	cout << endl;

	while(scanf("%s",str)==1)
	{
		flag=0;
		current_state=0;
		int len=strlen(str);
		
		for(i=0;i<len;i++)
		{
			int temp = find_column(str[i]);
			//cout << str[i] << " -- "<< temp;
			//cout << " -- >cur state -- >" << current_state << endl;
			if(find_column(str[i])==-1)
			{
				cout << "ERROR\n";
				break;
			}
			current_state=transition_table[current_state][temp];
			if(current_state==-1)
			{
				cout << "ERROR\n";
				break;
			}
		}
		if(i!=len)
			continue;

		vector< vector<int> >::iterator iter_accept_d;
		vector<int>::iterator iter_accept_str_d;
		int count=0;
		for(iter_accept_d=accept_state_dfa.begin();iter_accept_d!=accept_state_dfa.end();iter_accept_d++)
		{
			count++;
			for(iter_accept_str_d=(*iter_accept_d).begin();iter_accept_str_d!=(*iter_accept_d).end();iter_accept_str_d++)
			{
				if(*iter_accept_str_d==current_state)
				{
					flag=1;
					break;
				}
			}
			if(flag==1)
				break;
		}
		if(flag==0)
			cout << "Lexeme not found.\n";
		else
		{
			//cout << token_class[count-1]<<endl;
			if(!(strcmp(token_class[count-1].c_str(),"Identifiers")))
			{
				int temp = look(str,0);
				if(temp==-1)
				{
					idents_strings.push_back(str);
					fprintf(identifier_file,"%d\t%s\n",ident++,str);
					cout <<" < "<<token_class[count-1]<<" , "<< str << " , " << ident-1 << " > ";
				}
				else
				{
					cout <<" < "<<token_class[count-1]<<" , "<< str << " , " << temp << " > ";
				}
			}
			else if(!(strcmp(token_class[count-1].c_str(),"Constants")))
			{
				int temp = look(str,1);
				if(temp==-1)
				{
					constants_strings.push_back(str);
					fprintf(constant_file,"%d\t%s\n",constant++,str);
					cout <<" < "<<token_class[count-1]<<" , "<< str << " , " << constant-1 << " > ";
				}
				else
				{
					cout <<" < "<<token_class[count-1]<<" , "<< str << " , " << temp << " > ";
				}
			}
			else
				cout <<" < "<<token_class[count-1]<<" , "<< str<< " > ";
		}
		cout << endl;
	}
	cout<<endl;
	fclose(identifier_file);
	fclose(constant_file);
}
