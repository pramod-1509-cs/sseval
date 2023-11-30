#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Token
{
    char label[100];
    char opcode[100];
    char operands[100];
};
struct SymbolTable
{
    char label[100];
    int address;
};
struct SymbolTable sym[100];
int address;
void tokenise(char line[1000], struct Token *token)
{
    int result;
    result = sscanf(line, "%s %s %s", token->label, token->opcode, token->operands);
    if (result == 2)
    {
        char temp[1000], temp1[1000];
        strcpy(temp1, token->opcode);
        strcpy(temp, token->label);
        strcpy(token->opcode, temp);
        strcpy(token->operands, temp1);
        strcpy(token->label,"");
    }
    else if (result == 1)
    {
        strcpy(token->opcode, token->label);
        strcpy(token->operands,"");
        strcpy(token->label,"");
    }
}
int search_opcode(FILE *fp1,struct Token token)
{
    struct Token token2;
    char line2[1000];
    while(fgets(line2,1000,fp1)!=NULL)
    {
          tokenise(line2,&token2);
          if(strcmp(token2.opcode,token.opcode)==0)
          {
             return 1;
          }

    }
    return 0;
}
int search_label(struct Token token,int n)
{
    int i;
    for(i=0;i<n;i++)
    {
        if(strcmp(token.label,sym[i].label)==0)
        {
            return 1;
        }
    }
    return 0;
}
int main()
{
    struct Token token;
    struct Token token2;
    int program_length=0;
    int Line=1,i=0,n_lines_symtab=0,duplicate_opcode_flag=0,temp_current_address;
    FILE *input, *output,*fp1,*fp2,*fp_len;
    char line[1000],mnemonic[1000];
    int start;
    input = fopen("input.txt", "r");
    fp1 = fopen("optab.txt", "r");
    fp2 = fopen("symtab.txt", "w");
    output = fopen("intermediate.txt", "w");
    fp_len = fopen("program_length.txt","w");
    fgets(line, 1000, input);
    tokenise(line,&token);
    if(strcmp(token.opcode, "START") == 0)
        {
            fprintf(output,"%s\t%s\t%s\t%s\n",token.operands, token.label, token.opcode, token.operands);
            start=address=strtol(token.operands,NULL,16);
        }
    else
        {
            address=0;
        }

while(fgets(line, 1000, input) != NULL)
    {
       duplicate_opcode_flag=0;
       temp_current_address=address;
       tokenise(line,&token);
       rewind(fp1);
        if(search_opcode(fp1,token))
        {
           address+=3;
           program_length += 3;
        }


        else if(strcmp(token.opcode, "WORD") == 0)
        {
             address += 3;
             program_length += 3;
        }

        else if (strcmp(token.opcode, "RESW") == 0)
        {
            address += (3 * (atoi(token.operands)));
            program_length += (3 * (atoi(token.operands)));
        }

        else if (strcmp(token.opcode, "BYTE") == 0)
        {
            if(token.operands[0]=='C')
                {
                    address+=strlen(token.operands)-3;
                    program_length += strlen(token.operands)-3;
                }

            else if(token.operands[0]=='X')
                {
                    if(strlen(token.operands)%2 == 0)
                        {
                            address+=(strlen(token.operands)-3)/2;
                            program_length += (strlen(token.operands)-3)/2;
                        }
                    else
                        {
                            address+=(strlen(token.operands)-2)/2;
                            program_length += (strlen(token.operands)-2)/2;
                        }

                }

        }
        else if(strcmp(token.opcode, "RESB") == 0)
        {
            address += atoi(token.operands);
            program_length += atoi(token.operands);
        }

        else if(strcmp(token.opcode, "ORG") == 0)
        {
            address = strtol(token.operands,NULL,16);
            //program_length += 3;
            //address = atoi(token.operands);
            //temp_current_address=address;
        }
        else
        {
            printf("Invalid Opcode at line %d\n",Line+1);
            printf("%s\t%s\t%s\t\n",token.label,token.opcode,token.operands);//you can stop the program here by adding exit(0);
            duplicate_opcode_flag=1;
        }
    Line++;
    if(!duplicate_opcode_flag)
    {
        fprintf(output,"%x\t%s\t%s\t%s\n",temp_current_address,token.label, token.opcode, token.operands);
    }

    if(strcmp(token.label, "") != 0 && !duplicate_opcode_flag)
       {
        if(search_label(token,n_lines_symtab)==1)
        {
            printf("Error message: duplicate Label found \npress 1:to continue\npress: 0 to exit.\n");
            printf("Invalid Opcode at line %d\n",Line);
            int x;
            scanf("%d",&x);
            switch(x){
                case 0:exit(1);
                break;
                case 1:goto star;
                }
        }
        else
            {
            star:
            fprintf(fp2, "%s\t%x\n", token.label, temp_current_address);
            sym[i].address=address;
            strcpy(sym[i].label,token.label);
            i++;
            n_lines_symtab=i;
            }
       }
    }//end of while loop
    fclose(input);
    fclose(output);
    fclose(fp1);
    fclose(fp2);
    int length=address-start;
    printf("%x",program_length-3);
    fprintf(fp_len,"%x",program_length-3);
    fclose(fp_len);
    return 0;
}
