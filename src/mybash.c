#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "builtin.h"
#include "command.h"
#include "execute.h"
#include "parser.h"
#include "parsing.h"
#include "prompt.h"

int main(int argc, char* argv[]) {
    pipeline pipe = pipeline_new();
    Parser input = parser_new(stdin);
    bool quit = false;

    while (!quit) {
        show_prompt();
        pipe = parse_pipeline(input);

        quit = parser_at_eof(input);

        if (pipe != NULL) {
            execute_pipeline(pipe);
            pipeline_destroy(pipe);
        } else if (quit) {
            printf("\n");
        }
    }

    if (input != NULL) {
        parser_destroy(input);
        input = NULL;
    }

    if (pipe != NULL) {
        pipeline_destroy(pipe);
        pipe = NULL;
    }

    return EXIT_SUCCESS;
}

