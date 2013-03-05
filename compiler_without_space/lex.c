
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"y.tab.h"
#define KEYWORDS 29                                              //number of keywords

int d[50][50],count=0,type;
char str[100],c;
char k[KEYWORDS][10]= {"typedef","short","sizeof" ,"int","float","double","bool","extern","char",
		       "signed","unsigned","for","while","do","return","struct","const","void","switch",
		       "break","case","continue","goto","long","static","union","volatile","default","enum"};         //list of keywords

int yylex(yylType* yylP);

int main()
{
    int i,j,token;
    yylType* yylP;

    yylP = (yylType*)malloc(sizeof(yylType));                     //allocating space to the pointer

    /**********************************************************************************Creating the FSM************************************************************************************/
     for(i=0 ;i<49  ;i++)
       d[i][0]=0;
     
     for(i=0 ;i<49  ;i++)
       d[i][1]=0;
     
     for(i=0 ;i<49  ;i++)
       d[i][2]=0;
     
     for(i=0 ;i<49  ;i++)
       d[i][3]=0;
     
     for(i=0 ;i<49  ;i++)
       d[i][4]=-1;
     
     for(i=0 ;i<49  ;i++)
       d[i][5]=0;
     
     for(i=0 ;i<49  ;i++)
       d[i][6]=0;
     
     for(i=0 ;i<49  ;i++)
       d[i][7]=0;
     
     for(j=8;j<18;j++)
       {
	 for(i=0;i<49;i++)
	   {
	     d[i][j]=0;
	   }
       }
     
     for(i=0;i<49;i++)
       {
	 for(j=18;j<36;j++)
	   {
	     if(j==18 || j==24 || j==27 || j==31 )
	       {
		 d[i][j]=-1;
	       }
	     else
	       d[i][j]=0;
	   }
       }
     
     
     d[0][0] = 25;
     d[0][5] = 6;
     d[33][6]=30;
     d[0][6] = 29;
     d[0][8]=34;
     d[0][9]=35;
     d[0][10]=15;
     d[0][11]=7;
     d[0][12]=40;
     d[0][13]=10;
     d[0][14]=41;
     d[0][15]=17;
     d[0][16]=3;
     d[0][17]=4;
     d[7][11]=8;
     d[10][13]=11;
     d[3][14]=5;
     d[4][14]=5;
     d[4][16]=4;
     d[5][16]=5;
     d[4][17]=4;
     d[5][17]=5;
     d[1][16]=1;
     d[2][16]=1;
     d[1][17]=1;
     d[2][17]=1;
     d[0][19]=33;
     d[0][20]=19;
     d[0][21]=13;
     d[0][22]=22;
     d[0][23]=42;
     d[0][25]=1;
     d[0][26]=36;
     d[0][28]=37;
     d[0][29]=31;
     d[0][30]=2;
     d[0][32]=38;
     d[0][33]=27;
     d[0][34]=39;
     d[0][35]=32;
     d[1][25]=1;
     d[1][30]=1;
     d[2][25]=1;
     d[7][21]=9;
     d[10][21]=12;
     d[10][22]=43;
     d[13][21]=14;
     d[15][21]=16;
     d[17][21]=18;
     d[19][20]=20;
     d[19][21]=21;
     d[22][21]=24;
     d[22][22]=23;
     d[25][21]=26;
     d[27][33]=28;
     d[0][0]= 0;
     d[0][1]= 25;
     d[29][6]= 30;
     d[33][6]= 0;
     d[0][2] = 45;
     d[0][3] = 44;
     d[0][7] = 46;
	
     
     for(int l=0;l<49;l++)
     {
     	for(int m=0;m<36;m++)
     		printf("%d ",d[l][m]);
     	printf("\n");
     }

     /*******************************************************************************FSM Created*******************************************************************************************/
             
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
	if(yylP->integer==430) printf("\n");
       }
          
     return 0;
}

int yylex(yylType* yylP)
{
  int current_state=0,prev_state,i=0,j=0,comment=0;
  type = 0;
  
  if(count==0)                                                               //reading the first character from the user
    {
      count = 1;
      scanf("%c",&c);
    }
//  printf("%c---> \n",c);
  while(1)
    {
      if(c == EOF)                                                             //if the end of input is reached
	{
	    return 0;
	}
      prev_state = current_state;
      
      if((c >= 32) && (c < 127))                                                   //if c is a printable character
        {
	  if((c>=32) && (c<49)) 
	    {
	      current_state = d[current_state][c-32];                          //reading the next state from the FSM
	    }
	  
	  if((c>=49 ) && (c<=57)) 
	    {
	      current_state = d[current_state][17];                        //reading the next state from the FSM
	    }
	  
	  if((c>57) && (c<65)) 
	    {
	      current_state = d[current_state][c-40];                     //reading the next state from the FSM
	    }
	  
	  if((c>=65) && (c<=90))
	    {
	      current_state = d[current_state][25];                     //reading the next state from the FSM
	    }
	  
	  if((c>90) && (c<97))
	    {
	      current_state = d[current_state][c-65];                         //reading the next state from the FSM
	    }
	  
	  if((c>=97) && (c<123))
	    {
	      current_state = d[current_state][25];                            //reading the next state from the FSM
	    }
	  
	  if((c>=123) && (c<127))
	    {
	      current_state = d[current_state][c-91];                      //reading the next state from the FSM
	    }
	  
	  if((current_state>0) && (current_state<6))                    
	    {
	      str[i++] = c;
	    }
        }
      
      else                                                                  //start from the beginning
	{
	  current_state = 0;                      
	  //count = 0;
        }
      
      if((prev_state == 0) && (current_state == -1))                        //if the lexeme does not match with any pattern
        {
	  type = 1;
	  str[i++] = c;
	  str[i] = '\0';
	  yylP->string = str;
	  count = 0;
	  return ERR;                                                       //return -1
        }
      
      if((prev_state == 17) && (c == '*'))                                      //if comments start in the input
	while(1)
	  {
	    scanf("%c",&c);
	    
	    if((comment == 1) && (c == '/'))
	      {
		type = 4;
		count = 0;
		return COMMENT;
	      }
	    
	    if(c == '*')
	      {
		comment = 1;
	      }
	    else
	      {
		comment = 0;
	      }
	  }
      
      if((current_state == 0) || (current_state == -1))                            //if a lexeme is identified
        {
	  switch(prev_state)                                                         
            {
            case 1:
	      
            case 2:
	      str[i]='\0';
	      if(strcmp(str,"true") == 0 || strcmp(str,"false") == 0)                 //if boolean constants are identified
		{
		  type = 1;
		  yylP->string = str;
		  return BOOL_OPTR;
		}
	      
	      for(j=0; j<29; j++)
		{
		  if(strcmp(str,k[j]) == 0)
                    {
		      type = 2;
		      yylP->integer = 540 + j*10;
		      return KEYOWRDS;                                          //the spelling of KEYWORDS is given wrong in the y.tab.h file
                    }
		}
	      
	      type = 1;
	      yylP->string = str;                                                
	      return IDENTIFIER;                                                  //if identifiers are identified
	      
            case 3:
	      
            case 4:
	      type = 2;
	      str[i] ='\0';
	      yylP->integer = atoi(str);
	      return INT_CONST;                                                //if integers are identified
	      
            case 5:
	      type = 3;
	      str[i]='\0';
	      yylP->real = atof(str);
	      return FLT_CONST;                                              //if floating numbers are identified

            case 6:
                yylP->integer = PERCENT;
                return ARITH_OPTR;                                            //if arithmetic operators are identified

            case 7:
                yylP->integer=PLUS;
                return ARITH_OPTR;                                             //if arithmetic operators are identified

            case 8:
                yylP->integer=INCREMENT;
                return ARITH_OPTR;                                             //if arithmetic operators are identified

            case 9:
                yylP->integer=PLUS_EQUAL;
                return ASSGN_OPTR;                                            //if assignment operators are identified

            case 10:
                yylP->integer=MINS;
                return ARITH_OPTR;                                            //if arithmetic operators are identified

            case 11:
                yylP->integer=DECREMENT;
                return ARITH_OPTR;                                            //if arithmetic operators are identified

            case 12:
                yylP->integer=MINUS_EQUAL;
                return ASSGN_OPTR;                                          //if assignment operators are identified

            case 13:
                yylP->integer=EQUAL;
                return ASSGN_OPTR;                                            //if assignment operators are identified

            case 14:
                yylP->integer=EQUAL_EQUAL;
                return RELN_OPTR;                                           //if relational operators are identified

            case 15:
                yylP->integer=MULT;
                return ARITH_OPTR;                                          //if arithmetic operators are identified

            case 16:
                yylP->integer=MULT_EQUAL;
                return ASSGN_OPTR;                                            //if assignment operators are identified

            case 17:
                yylP->integer=DIVD;
                return ARITH_OPTR;                                          //if arithmetic operators are identified

            case 18:
                yylP->integer=DIV_EQUAL;
                return ASSGN_OPTR;                                          //if assignment operators are identified

            case 19:
                yylP->integer=LESS;
                return RELN_OPTR;                                         //if relational operators are identified

            case 20:
                yylP->integer=SHIFT_LEFT;
                return SHIFT_OPTR;                                        //if shift operators are identified

            case 21:
                yylP->integer=LESS_EQUAL;
                return RELN_OPTR;                                          //if relational operators are identified

            case 22:
                yylP->integer=GREATER;
                return RELN_OPTR;                                          //if relational operators are identified

            case 23:
                yylP->integer=SHIFT_RGT;
                return SHIFT_OPTR;                                         //if shift operators are identified

            case 24:
                yylP->integer=GRT_EQUAL;
                return RELN_OPTR;                                         //if relational operators are identified

            case 25:
                yylP->integer=BOOL_NOT;
                return BOOL_OPTR;                                            //if boolean operators are identified

            case 26:
                yylP->integer=NOT_EQUAL;
                return RELN_OPTR;                                           //if relational operators are identified

            case 27:
                yylP->integer=BIT_OR;
                return BIT_OPTR;                                            //if bit operators are identified

            case 28:
                yylP->integer=BOOL_OR;
                return BOOL_OPTR;                                         //if boolean operators are identified

            case 29:
                yylP->integer=BIT_AND;
                return BIT_OPTR;                                         //if bit operators are identified

            case 30:
                yylP->integer=BOOL_AND;
                return BOOL_OPTR;                                           //if bit boolean are identified

            case 31:
                yylP->integer=BIT_XOR;
                return BIT_OPTR;                                          //if bit operators are identified

            case 32:
                yylP->integer=BIT_NOT;
                return BIT_OPTR;                                           //if bit operators are identified

            case 33:
                yylP->integer=SEMICOLON;
                return SPEC_SYM;                                         //if special symbols are identified
 
            case 34:
                yylP->integer=LFT_PARAN;
                return SPEC_SYM;                                             //if special symbols are identified

            case 35:
                yylP->integer=RGT_PARAN;
                return SPEC_SYM;                                            //if special symbols are identified

            case 36:
                yylP->integer=L_SQR_BKT;
                return SPEC_SYM;                                              //if special symbols are identified
  
	    case 37:
                yylP->integer=R_SQR_BKT;
                return SPEC_SYM;                                                //if special symbols are identified

            case 38:
                yylP->integer=LFT_BRACE;
                return SPEC_SYM;                                            //if special symbols are identified

            case 39:
                yylP->integer=RGT_BRACE;
                return SPEC_SYM;                                            //if special symbols are identified
   
	    case 40:
                yylP->integer=COMMA;
                return SPEC_SYM;                                                 //if special symbols are identified
 
           case 41:
                yylP->integer=DOTONLY;
                return SPEC_SYM;                                           //if special symbols are identified
   
	    case 42:
                yylP->integer=QUES_MARK;
                return SPEC_SYM;                                          //if special symbols are identified

            case 43:
                yylP->integer=MINS_GRTR;
                return SPEC_SYM;                                            //if special symbols are identified
	    case 44:
		yylP->integer = INCLUDE;
		return SPEC_SYM;
	    case 45:
		yylP->integer = DOUBLE_QUOTE;
		return SPEC_SYM;
	    case 46:
		yylP->integer = SINGLE_QUOTE;
		return SPEC_SYM;	
                break;
            }
        }
        scanf("%c",&c);                                                            //continue reading the input
    }

}
