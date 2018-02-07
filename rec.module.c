#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
export {
#include <stdbool.h>
}

export typedef struct {
	int rate;
	int bits;
	int channels;
	int endianness;
	bool verbose;
} params_t;

export params_t defaults() {
	params_t params = {
		.rate       = 48000,
		.channels   = 2,
	};
	return params;
}

export FILE * record(params_t params, char * device) {
	char * cmd;
	asprintf(&cmd, "arecord -D %s -t raw -r %d -f dat -c %d 2>/dev/null",
			device,
			params.rate,
			params.channels
	);

	if (params.verbose) fprintf(stderr, "'%s'\n", cmd);
	return popen(cmd, "r");
}

