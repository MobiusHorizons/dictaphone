#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
export {
#include <stdbool.h>
}

build append CFLAGS "-D_GNU_SOURCE";

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
		.bits       = 16,
		.channels   = 2,
		.endianness = 0,
	};
	return params;
}

export FILE * encode(params_t params, const char * out) {
  char * cmd;
  asprintf(&cmd, "opusenc --raw --raw-rate %d --raw-bits %d --raw-chan %d --raw-endianness %d - %s 2>/dev/null",
        params.rate,
        params.bits,
        params.channels,
        params.endianness,
        out
  );

  if (params.verbose) fprintf(stderr, "'%s'\n", cmd);
  return popen(cmd, "w");
}
