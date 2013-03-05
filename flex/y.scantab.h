#ifndef _Y_TAB_H
#define _Y_TAB_H

#define ERR (-1)

/* Tokens: identifier, integer constant,floating point constant, boolean constant, comment, keywords, arithmetic operator, assignment operator, relational
 *         operator, boolean operator, shift operator, bit operator, special symbols
 */
#define IDENTIFIER  300
#define INT_CONST   310
#define FLT_CONST   320
#define BOOL_CONST  330
#define COMMENT     340
#define KEYOWRDS    350
#define ARITH_OPTR  360
#define ASSGN_OPTR  370
#define RELN_OPTR   380
#define BOOL_OPTR   390
#define SHIFT_OPTR  400
#define BIT_OPTR    410
#define SPEC_SYM    420

/* Tokens: ; ( ) [ ] -> { } , . ? # " '
 */
#define SEMICOLON   430
#define LFT_PARAN   440
#define RGT_PARAN   450
#define L_SQR_BKT   460
#define R_SQR_BKT   470
#define MINS_GRTR   480
#define LFT_BRACE   490
#define RGT_BRACE   500
#define COMMA       510
#define DOTONLY     520
#define QUES_MARK   530
#define INCLUDE	    840
#define DOUBLE_QUOTE 850
#define SINGLE_QUOTE 860

/* Tokens for keywords: 
 */
#define TYPEDEF     540
#define SHORT       550
#define SIZEOF      560
#define INT         570
#define FLOAT       580
#define DOUBLE      590
#define BOOL        600
#define EXTERN      610
#define CHAR        620
#define SIGNED      630
#define UNSIGNED    640
#define FOR         650
#define WHILE       660
#define DO          670
#define RETURN      680
#define STRUCT      690
#define CONST       700
#define VOID        710
#define SWITCH      720
#define BREAK       730
#define CASE        740
#define CONTINUE    750
#define GOTO        760
#define LONG        770
#define STATIC      780
#define UNION       790
#define VOLATILE    800
#define DEFAULT     810
#define ENUM        820

/* Operator Attribute values: + - * / % ++ -- = += -= *= /= < > <= >= == != && || ! << >> | & ^ ~
 */
#define PLUS        40 
#define MINS        41 
#define MULT        42 
#define DIVD        43
#define PERCENT     44
#define INCREMENT   45
#define DECREMENT   46
#define EQUAL       47
#define PLUS_EQUAL  48
#define MINUS_EQUAL 49
#define MULT_EQUAL  50
#define DIV_EQUAL   51
#define LESS        52
#define GREATER     53
#define LESS_EQUAL  54
#define GRT_EQUAL   55
#define EQUAL_EQUAL 56
#define NOT_EQUAL   57
#define BOOL_AND    58
#define BOOL_OR     59
#define BOOL_NOT    60
#define SHIFT_LEFT  61
#define SHIFT_RGT   62
#define BIT_OR      63
#define BIT_AND     64
#define BIT_XOR     65
#define BIT_NOT     66


#endif
