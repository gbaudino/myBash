#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "prompt.h"

#define GREEN "\033[0;32m"
#define BLUE "\033[0;34m"
#define COLOR_R "\033[0m"
//color reset

void show_prompt(void) {
	char hostname[256];
	hostname[255] = '\0';
	gethostname(hostname, 256);

	char username[256]; 
	username[255] = '\0';
	getlogin_r(username, 256);

	char curr_dir[2048];
	curr_dir[2047] = '\0';
	getcwd(curr_dir, 2048);

	char *prompt = getenv("PS2");
	if (prompt == NULL) {
		prompt = "JASH> ";
	}

	printf(BLUE"%s@%s:" GREEN"%s" COLOR_R" %s", username, hostname, curr_dir, prompt);

	fflush(stdout);
}

