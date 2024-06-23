%{
#include "y.tab.h"
%}

%%
NOP         { return NOP; }
LD          { return LD; }
ADD         { return ADD; } 
SUB         { return SUB; }
STORE       { return STORE; }
JP          { return JP; }
JPZ         { return JPZ; }
INC         { return INC; }
DEC         { return DEC; }
HALT        { return HALT; }
A           { return REGISTER_A; }
B           { return REGISTER_B; }
C           { return REGISTER_C; }
F           { return REGISTER_F; }
AB          { return UNION_AB; }
BC          { return UNION_BC; }
[0-9]+      { yylval.ichr = atoi(yytext); return NUMBER; }
[a-z]+:     { return LABEL; }
\n          { return EOL; }
[ \t]       ; // ignore whitespace
.           { return *yytext; }
%%

int yywrap() {
    return 1;
}
