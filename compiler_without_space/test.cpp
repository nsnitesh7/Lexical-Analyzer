#include <cstdlib>
#include <cstdio>
#include <iostream>
using namespace std;

int main()
{
	freopen("file.txt","r",stdin);
	char c=getchar();
	while(c!=EOF)
	{
		cout << c;
		c=getchar();
	}
	return 0;
}
