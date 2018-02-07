#include <time.h>
#include <string.h>
#include <stdlib.h>

export char * from_date(const char * ext) {
	size_t length = 27 + strlen(ext);
	char * out = (char*) malloc(length);

	time_t timer;
	time(&timer);
    struct tm* tm_info = localtime(&timer);

	strftime(out, length, "%Y-%m-%d_%H:%M:%S", tm_info);
	strcat(out, ".");
	strcat(out, ext);

	return out;
}
