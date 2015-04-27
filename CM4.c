/*************************************************************
                birthday.c
 This program is use to calculate difference between two dates
 which is today and your next birthday. Checking by invoking
 function from another file.
                
                Created by
 Titanawat   Thammawijitdaj  55070503420
 Maytas      Tirachitman     55070503437
                20 Nov. 2012
*************************************************************/
#include<stdio.h>
#include<string.h>
#include "dateFunctions.h"

int main(int argc, char* argv[])
{
    char input[32];
    char dateString[32];
    int status;
    long difference;
    DATE_T birthday;
    DATE_T today;
    printf("Enter your next birthday(dd/mm/yyyy):");
    fgets(input,32,stdin);
    sscanf(input,"%s",dateString);
    status = checkDate(dateString,&birthday);
    while (status != DATE_OK)
        status = checkDate(dateString,&birthday);
    
    dateToday(&today);
    
    difference = dateDifference(birthday,today);
    printf("The difference of today and your birthday is %ld",difference);
}
