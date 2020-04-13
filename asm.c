#include "stdio.h"
#include <stdlib.h>
#include <string.h>

FILE *inputFile;
FILE *outFile;
char *line = NULL;
size_t len = 0;
size_t read;

char *uniq_spc(char *str);
char decodeASM(char *ints);
unsigned char strToHex(char *str, int max);
unsigned long hash(char *str);
unsigned char getRegister(char *reg);
char *removeChar(char *s);

#define LD 5862485
#define OUT 193465917
#define ADD 193450094
#define SUB 193470255
#define JP 5862431

#define A 177638
#define B 177639
#define X 177661


#define C 177640

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Please secify an input and output file\n");
    }

    if ((inputFile = fopen(argv[1], "r")) == NULL)
    {
        printf("Error! opening file %s \n", argv[1]);
        exit(1);
    }

    if ((outFile = fopen(argv[2], "w")) == NULL)
    {
        printf("Error! opening file  %s \n", argv[2]);

        exit(1);
    }

    while ((read = getline(&line, &len, inputFile)) != -1)
    {
        printf("%s\n", uniq_spc(line));
        fprintf(outFile,"%c",decodeASM(uniq_spc(line)));
    }
    printf("Success!\n");
    fclose(inputFile);
    fclose(outFile);
}

char decodeASM(char *ints)
{
    unsigned char decodeValue = 0;
    char *ptr = strtok(ints, " ");
    switch (hash(ptr))
    {
    case LD:
        decodeValue |= 8;
        ptr = strtok(NULL, " ");
        decodeValue |= getRegister(ptr);
        ptr = strtok(NULL, " ");
        decodeValue |= strToHex(ptr, 7);
        break;
    case OUT:
        decodeValue |= 32;
        break;
    case ADD:
        ptr = strtok(NULL, " ");
        decodeValue |= getRegister(ptr);
        break;
    case SUB:
        decodeValue |= 4;
        ptr = strtok(NULL, " ");
        decodeValue |= getRegister(ptr);
        break;
    case JP:
        ptr = strtok(NULL, " ");
        if(strcmp(ptr,"C") == 0){
            decodeValue |= 112;
            ptr = strtok(NULL, " ");
        }else
        {
            decodeValue |= 176;
        }
        decodeValue |= strToHex(ptr, 15);
        
        break;
    default:
        printf("Syntax Error! %s is not a recognise symbol\n", ptr);
        exit(1);
    }
    return decodeValue;
}

unsigned char getRegister(char *reg){
    switch (hash(reg))
        {
        case A:
            return 0;
            break;
        case B:
            return 16;
            break;
        case X:
            return 48;
            break;
        default:
            printf("Syntax Error! %s is not a valid register\n", reg);
            exit(1);
            break;
        }
    return 0;
}

unsigned char strToHex(char *str, int max)
{
    unsigned char val;
    sscanf(str, "%2hhx", &val);
    if(val > max){
        printf("Syntax Error! %d too large for that param, max=%d \n", val, max);
            exit(1);
    }
    return val;
}

unsigned long hash(char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

char *uniq_spc(char *str)
{
    char *from, *to;
    int spc = 0;
    to = from = str;
    while (1)
    {
        if (spc && *from == ' ' && to[-1] == ' ')
            ++from;
        else
        {
            spc = (*from == ' ') ? 1 : 0;
            *to++ = *from++;
            if (!to[-1])
                break;
        }
    }
    return removeChar(str);
}

char *removeChar(char *s){ 
  
    int j, n = strlen(s); 
    for (int i=j=0; i<n; i++) 
       if (s[i] != 10 && s[i] != 13) 
          s[j++] = s[i]; 
      
    s[j] = '\0';
    return s;
} 
