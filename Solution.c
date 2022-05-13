#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
char* readline();
char wordslength[10]={0}; //global array to store each word length
char numberofoccuipieddashes[10]={0}; //array to store occuipied dashes for each word
char totalnumberofwords=0; //global variable to store number of words
char solutionfoundedflag=0; //indicate if a proper solution founded or not yet
// Complete the crosswordPuzzle function below.

// Please store the size of the string array to be returned in result_count pointer. For example,
// char a[2][6] = {"hello", "world"};
//
// *result_count = 2;
//
// return a;
//
void clearcolumn(char** crossword,char* startrowindex,char wordindex, char* columnindex)
{
    char index=wordslength[wordindex]+(*startrowindex)-1;
    char index2=numberofoccuipieddashes[wordindex];
     for(;index2>0;index2--)
    {
        crossword[index][*columnindex]='-';
        index--;
    }
    if(crossword[(*startrowindex+wordslength[wordindex]+1)%10][*columnindex]=='-')
    {
        *columnindex=*columnindex-1;
        *startrowindex=(*startrowindex+1)%10;
    }
    else
    {
        *columnindex=*columnindex;
        *startrowindex=0;
    }
}
void clearrow(char* crosswordrow,char* startcolumnindex,char wordindex,char *rowindex)
{
    char index=wordslength[wordindex]+(*startcolumnindex)-1;
    char index2=numberofoccuipieddashes[wordindex];
    for(;index2>0;index2--)
    {
        crosswordrow[index]='-';
        index--;
    }
    if(crosswordrow[*startcolumnindex+wordslength[wordindex]]=='-')
    {
        *rowindex=*rowindex-1;
        *startcolumnindex=(*startcolumnindex+1)%10;
    }
    else
    {
        *rowindex=*rowindex;
        *startcolumnindex=0;
    }
}
char try_to_solve_vr(char** crossword,char* words,char wordindex,char columnindex,char* startrowindex,char wordoffset)
{
    char index=0,numberofavailabledashes=0,start=*startrowindex;
    for(index=0;index<10;index++)
    {
        numberofavailabledashes=0;
        if((crossword[index][columnindex]=='-')||(crossword[index][columnindex]==*(words+wordoffset)))
        {
            start=index;
            while((index<10)&&((crossword[index][columnindex]=='-')||(crossword[index][columnindex]==*(words+wordoffset+index-start))))
            {
                numberofavailabledashes++;
                index++;
            }
            if(numberofavailabledashes>=wordslength[wordindex])
            {
                numberofoccuipieddashes[wordindex]=0;
                break;
            }
        }
    }
    if(numberofavailabledashes>=wordslength[wordindex])
    {
        *startrowindex=start;
        for(index=0;index<wordslength[wordindex];index++)
        {
            if((crossword[start+index][columnindex]=='-')||(crossword[start+index][columnindex]==*(words+wordoffset+index)))
            {
                if(crossword[start+index][columnindex]=='-')
                {
                    numberofoccuipieddashes[wordindex]++;
                }
                crossword[start+index][columnindex]=*(words+wordoffset+index);
            }
        }
        return 1;
    }
    else
    {
        return 0;
    }
    return 0;
}
char try_to_solve_hr(char* crosswordrow,char* words,char wordindex,char rowindex,
char *startcolumnindex)
{
    char index=0,numberofavailabledashes=0,start=*startcolumnindex,wordoffset=0;
    index=wordindex;
    while(index>0)
    {
        wordoffset=wordoffset+1+wordslength[index-1];
        index--;
    }
    for(index=start;index<10;index++)
    {
        numberofavailabledashes=0;
        if((crosswordrow[index]=='-')||(crosswordrow[index]==*(words+wordoffset)))
        {
            start=index;
            while(((crosswordrow[index]=='-')||(crosswordrow[index]==*(words+wordoffset+index-start)))&&(index<10))
            {
                 numberofavailabledashes++;
                 index++;
            }
            if(numberofavailabledashes>=wordslength[wordindex])
            {
                numberofoccuipieddashes[wordindex]=0;
                break;
            }
        }
    }
    if(numberofavailabledashes>=wordslength[wordindex])
    {
        *startcolumnindex=start;
        for(index=0;index<wordslength[wordindex];index++)
        {
            if((crosswordrow[start+index]=='-')||(crosswordrow[start+index]==*(words+wordoffset+index)))
            {
                if(crosswordrow[start+index]=='-')
                {
                    numberofoccuipieddashes[wordindex]++;
                }
                crosswordrow[start+index]=*(words+wordoffset+index);
            }
        }
        return 1;
    }
    else
    {
        return 0;
    }
    return 0;
}
void Getwordslength (char *w)
{
    char index1=0,index2=0,counter=0;
    while((w[index1]!='\0'))
    {
        if(w[index1]==';')
        {
            index1++;
            index2++;
            counter=0;
            totalnumberofwords++;
        }
        else
        {
            wordslength[index2]=++counter;
            index1++;
        }
    }
    totalnumberofwords++;
}
char solvecrossword(char** crossword,char* words,char wordindex)
{
    char rowindex,startcolumnindex=0,columnindex,startrowindex=0,foundflag=0,index=0,wordoffset=0;
    index=wordindex;
    while(index>0)
    {
        wordoffset=wordoffset+1+wordslength[index-1];
        index--;
    }
    if(wordindex>totalnumberofwords) //base case to stop recursion
    {
        solutionfoundedflag=1; //indication that a proper solution founded
        return 0; //return and stop recursion
    }
    else //recursive case
    {
        if(solutionfoundedflag==0) //check if a prorper solution founded or not
        {
            for(rowindex=0;rowindex<10;rowindex++) //horizontal
            {
                foundflag=try_to_solve_hr(crossword[rowindex],words,wordindex,                                                     rowindex,&startcolumnindex);
                if((foundflag!=0)&&(solutionfoundedflag==0))
                {
                    solvecrossword(crossword,words,wordindex+1);
                    if(solutionfoundedflag==0)
                    {
                    clearrow(crossword[rowindex],&startcolumnindex,wordindex,&rowindex);
                    foundflag=0;
                    continue;
                    }
                }
            }
            if(foundflag==0) //if no solution founded in horizontal try vertical
            {
                for(columnindex=0;columnindex<10;columnindex++) //vertical
                {
                    foundflag=try_to_solve_vr(crossword,words,wordindex,                                                     columnindex,&startrowindex,wordoffset);
                    if((foundflag!=0)&&(solutionfoundedflag==0))
                    {
                        solvecrossword(crossword,words,wordindex+1);
                        if(solutionfoundedflag==0)
                        {
                        clearcolumn(crossword,&startrowindex,wordindex,&columnindex);
                        foundflag=0;
                        continue;
                        }
                    }
                }
            }
        }
    }
    return foundflag; //just for the sake of preventg compilation error
}
char** crosswordPuzzle(int crossword_count, char** crossword, char* words, int* result_count) {
Getwordslength(words); //utility to calculate each word length
solvecrossword(crossword,words,0);
*result_count=crossword_count;
return crossword;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** crossword = malloc(10 * sizeof(char*));

    for (int i = 0; i < 10; i++) {
        char* crossword_item = readline();

        *(crossword + i) = crossword_item;
    }
    int crossword_count = 10;

    char* words = readline();

    int result_count;
    char** result = crosswordPuzzle(crossword_count, crossword, words, &result_count);
    for (int i = 0; i < result_count; i++) {
        fprintf(fptr, "%s", *(result + i));

        if (i != result_count - 1) {
            fprintf(fptr, "\n");
        }
    }

    fprintf(fptr, "\n");

    fclose(fptr);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!line) {
            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);
    } else {
        data = realloc(data, data_length + 1);

        data[data_length] = '\0';
    }

    return data;
}