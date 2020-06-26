#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/dtypes.h"
#include "../include/csvutils.h"
#include "../include/debug.h"
#include "../include/misc.h"

int skip_header = true;

/*
header_template holds the values that program expects to find in header of csv.
header_map is initially set to -1 (ln ~139)

Example:
date,open,close,volume,high,low

0->0 -> header_template[0]="date"
1->1 -> header_template[1]="open"
2->4 -> header_template[2]="high"
3->5 -> header_template[3]="low"
4->2 -> header_template[4]="close"
5->3 -> header_template[5]="volume"

header_map = {0,1,4,5,2,3,(Optional technical_indicator n)}

technical_indicators is optional! it is by default set to None.
*/

char header_template[MAXCSVHEAD][24] = {"date", "open", "high", "low", "close", "volume", "technical_indicators"};
int header_map[MAXCSVHEAD] = {0};

// this checks if the first char of given string starts with quote.
int is_quoted(char *str){
    if ((str[0] == '"') || (str[0] == '\'')){
        return true;
    }else{
        return false;
    }
}

algoticks_row tokenize_row(char *row){

    char *token;
    struct Row data;
    memset(&data, 0, sizeof(data));

    // set default
    strncpy(data.technical_indicators,"None",1024);

    int row_pos = 0;
    int header_i = 0;

    token = strtok(row, ",");

    
    while (token != NULL && header_i < MAXCSVHEAD) // MAXCSVHEAD+unknown_headers=0..8+unknown_headers
    {
        
        if (header_map[header_i] == -1){
            header_i++;
            continue;
        }

        //for debugging
        //printf("header_i: %d header_map: %d header_template: %s\n",header_i, header_map[header_i], header_template[header_map[header_i]]);

        if (strcmp(header_template[header_map[header_i]],"date") == 0)
        {
            if (is_quoted(token) == true) { remove_quotes(token); }
            strncpy(data.date, token, 32);
        }
        else if (strcmp(header_template[header_map[header_i]],"open") == 0)
        {
            if (is_quoted(token) == true) { remove_quotes(token); }
            data.open = atof(token);
        }
        else if (strcmp(header_template[header_map[header_i]],"high") == 0)
        {
            if (is_quoted(token) == true) { remove_quotes(token); }
            data.high = atof(token);
        }
        else if (strcmp(header_template[header_map[header_i]],"low") == 0)
        {
            if (is_quoted(token) == true) { remove_quotes(token); }
            data.low = atof(token);
        }
        else if (strcmp(header_template[header_map[header_i]],"close") == 0)
        {
            if (is_quoted(token) == true) { remove_quotes(token); }
            data.close = atof(token);
        }
        else if (strcmp(header_template[header_map[header_i]],"volume") == 0)
        {
            if (is_quoted(token) == true) { remove_quotes(token); }
            data.volume = atoi(token);
        }
        else if (strcmp(header_template[header_map[header_i]],"technical_indicators") == 0)
        {
            if (is_quoted(token) == true) { remove_quotes(token); }
            strncpy(data.technical_indicators,token,1024);
        }
        else
        {
            //unknown row position.
            break;
        }

        row_pos++;
        header_i++;
        token = strtok(NULL, ",");
    }

    return data;

}

int read_csv(algoticks_settings settings, FILE *fp, algoticks_row *storage, int seek_offset, algoticks_config config, int debug){

    if (feof(fp) == true)
    {
        return EOF;
    }

    if (seek_offset != 0)
    {
        fseek(fp, seek_offset, SEEK_SET);
    }



    // temp. storage array(s)
    char row[MAXCHARPERLINE];
    int curr_sp;
    

    while(1) {
    

    //get the row
    fgets(row, MAXCHARPERLINE, fp);


    if ((config.skip_header == true) && (skip_header == true)){

        char *token;
        token = strtok(row, ",");
        int header_i = 0;

        //set header_map to -1
        for (int i = 0; i <= MAXCSVHEAD; i++)
        {
            header_map[i] = -1;
        }

        while (token != NULL && header_i < MAXCSVHEAD)
        {
            chomp(token);
            convert_to_lowercase(token);

            debug_msg(settings, 2, "ReadHeader","csvutils.c",token);

            /*
            "date" = 0,
            "open" = 1,
            "high" = 2,
            "low" = 3,
            "close" = 4,
            "volume" = 5,
            "technical_indicators" = 6
            */

            // Date
            if(strcmp(token, "date") == 0){ 
                header_map[header_i] = 0; }

            else if(strcmp(token, "open") == 0){ 
                header_map[header_i] = 1; }

            else if(strcmp(token, "high") == 0){ 
                header_map[header_i] = 2; }

            else if(strcmp(token, "low") == 0){ 

                header_map[header_i] = 3; }

            else if(strcmp(token, "close") == 0){ 

                header_map[header_i] = 4; }

            else if(strcmp(token, "volume") == 0){ 
                header_map[header_i] = 5; }

            else if(strcmp(token, "technical_indicators") == 0){ 
                header_map[header_i] = 6; }

            else { printf("Unknown header element: %s\n", token);
                header_map[header_i] = -1;
                }

            header_i++;
            token = strtok(NULL, ",");
        }


        /* DEBUG 
        for (int i = 0; i < MAXCSVHEAD; i++)
        {
            printf("i:%d %d->%s\n",i, header_map[i], header_template[header_map[i]]);
        }
        */
        
        
        //skip the 1st row i.e header
        skip_header = false;
        continue;
    }


    //remove white space at end
    chomp(row);

    curr_sp = ftell(fp);

    *storage = tokenize_row(row);

    debug_msg(settings, 3, "ReadRow", "csvutils.c", storage->date);
    
    return curr_sp;

    }

}