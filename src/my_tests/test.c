#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../parsing.h"
#include "../parser.h"
#include "../command.h"


int main(int argc, char *argv[]){
    if (argc < 2 || argc > 2) {
        printf("Usage: %s <output_filename>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    Parser parser = NULL;
    char * line = NULL;
    pipeline pipe = NULL;

    FILE * cases = fopen("cases", "r");
    FILE * output = fopen(filename, "w");
    if (cases == NULL || output == NULL){
        printf("Error opening files\n");
        return 1;
    }
    
    parser = parser_new(cases);
    while(!feof(cases)){
        pipe = parse_pipeline(parser);
        if (pipe != NULL){
            line = pipeline_to_string(pipe);
            fprintf(output, "%s\n", line);
            free(line);
            line = NULL;
            pipeline_destroy(pipe);
            pipe = NULL;
        }
    }

    fclose(cases);
    fclose(output);

    return 0;
}   