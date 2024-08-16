%{
#include <stdio.h>
#include <string.h>

int yylex();
void yyerror(const char *s);

int pc = 0;
%}

%token <ichr> NOP LD ADD SUB STORE JP JPZ INC DEC HALT
%token <ichr> REGISTER_A REGISTER_B REGISTER_C REGISTER_F
%token <ichr> UNION_AB UNION_BC
%token <ichr> LABEL 
%token <ichr> EOL
%token <ichr> NUMBER

%union {
    int ichr;
}

%type <ichr> program line nop ld_a_b ld_a_c ld_b_c add_b add_c sub_b sub_c store_a store_b jp_n jpz_n inc_a inc_b inc_c dec_a dec_b dec_c halt
%type <ichr> ld_a_value

%%
program: | program line EOL { } 
line: nop | ld_a_b | ld_a_c | ld_b_c | ld_a_value |
    add_b | add_c | sub_b | sub_c | store_a | store_b | jp_n | jpz_n |
    inc_a | inc_b | inc_c | dec_a | dec_b| dec_c | halt |
    label { }
nop: NOP { printf("%c", 0); pc += 1; } 
ld_a_b: LD REGISTER_B ',' REGISTER_A { printf("%c", 1); pc += 1; }
ld_a_c: LD REGISTER_C ',' REGISTER_A { printf("%c", 2); pc += 1; }
ld_b_c: LD REGISTER_B ',' REGISTER_C { printf("%c", 3); pc += 1; }
ld_a_value: LD REGISTER_A ',' NUMBER { printf("%c%c", 4, $4); pc += 2; } 
add_b: ADD REGISTER_B { printf("%c", 5); pc += 1; }
add_c: ADD REGISTER_C { printf("%c", 6); pc += 1; }
sub_b: SUB REGISTER_B { printf("%c", 7); pc += 1; }
sub_c: SUB REGISTER_C { printf("%c", 8); pc += 1; }
store_a: STORE REGISTER_A ',' '[' UNION_AB ']' { printf("%c", 9); pc += 1; }
store_b: STORE REGISTER_A ',' '[' UNION_BC ']' { printf("%c", 10); pc += 1; }
jp_n: JP NUMBER { printf("%c%c%c", 11, ($2 >> 8) & 0xFF, $2 & 0xFF); pc += 3; }
jpz_n: JPZ NUMBER { printf("%c%c%c", 12, ($2 >> 8) & 0xFF, $2 & 0xFF); pc += 3; }
halt: HALT { printf("%c", 13); pc += 1; }
inc_a: INC REGISTER_A { printf("%c", 14); pc += 1; }
inc_b: INC REGISTER_B { printf("%c", 15); pc += 1; }
inc_c: INC REGISTER_C { printf("%c", 16); pc += 1; }
dec_a: DEC REGISTER_A { printf("%c", 17); pc += 1; }
dec_b: DEC REGISTER_B { printf("%c", 18); pc += 1; }
dec_c: DEC REGISTER_C { printf("%c", 19); pc += 1; }
label: LABEL { printf("hey\n"); }

%%

void yyerror(const char* s)
{
    fprintf(stderr, "%s\n", s);
}

int main()
{
    yyparse();
    return 0;
}
