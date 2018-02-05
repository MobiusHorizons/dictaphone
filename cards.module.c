package "cards";

#include <stdlib.h>
#include <stdio.h>

#include <alsa/asoundlib.h>

build append LDLIBS "-lasound";

char * get_card_input(int card) {
	char **hints;
	int err = snd_device_name_hint(card, "pcm", (void***) &hints);
	if (err != 0) return NULL;

	char** n = hints;
	while (*n != NULL) {
		char *io = snd_device_name_get_hint(*n, "IOID");

		if (io == NULL || strcmp("Input", io) == 0) {
			char * name = snd_device_name_get_hint(*n, "NAME");
			if (strncmp("hw:", name, 2) == 0) {
				free(io);
				snd_device_name_free_hint((void**)hints);
				return name;
			}
		}

		free(io);
		n++;
	}

	snd_device_name_free_hint((void**)hints);
	return NULL;
}

export int get_default_input(char ** device) {
	int card = -1;
	int max_card = -1;

	while (snd_card_next(&card) == 0 && card != -1) max_card = card;

	for (card = max_card; card >= 0; card--) {
		char * name = NULL;
		char * input = get_card_input(card);
		if (input == NULL) continue;
		snd_card_get_name(card, &name);

		if (device == NULL) {
			free(input);
		} else {
			*device = input;
		}
		free(name);
		return card;
	}
}
