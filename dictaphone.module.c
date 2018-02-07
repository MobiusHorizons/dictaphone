package "main";

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

import cards    from "cards.module.c";
import rec      from "rec.module.c";
import opus     from "opus.module.c";
import filename from "filename.module.c";

FILE * input, * output;

void on_signal(int signal) {
	pclose(input);
}

const char * progress = "-\\|/";

int main(int argc, const char ** argv) {
	signal(SIGUSR1, on_signal);

	char * device = NULL;
	int card = cards.get_default_input(&device);

	char * name = filename.from_date("opus");
	printf("Recording '%s' \r", name);
	fflush(stdout);

	input  = rec.record(rec.defaults(), device);
	output = opus.encode(opus.defaults(), name);
	
	int count = 0;	
	char buf[4096];
	size_t length;
	while((length = fread(buf, sizeof(char), 4096, input)) > 0) {
		fwrite(buf, sizeof(char), length, output);
		printf("Recording '%s' %c\r", name, progress[count / 4]);
		fflush(stdout);
		count = (count + 1) % 16;
	}
	printf("Recording '%s' - Done\n", name);
}
