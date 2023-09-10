#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../parsing.h"
#include "../parser.h"
#include "../command.h"


int main(void){
    Parser p = NULL;
    char * line = NULL;
    pipeline pipe = NULL;
    FILE * input = fopen("input.txt", "r");
    FILE * output = fopen("output.txt", "w");
    p = parser_new(input);
    while(!feof(input)){
        pipe = parse_pipeline(p);
        if (pipe != NULL){
            line = pipeline_to_string(pipe);
            fprintf(output, "%s\n", line);
            free(line);
            line = NULL;
            pipeline_destroy(pipe);
            pipe = NULL;
        }
    }

    fclose(input);
    fclose(output);

    return 0;
}   