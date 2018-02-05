package "main";

#include <stdio.h>
import cards from "cards.module.c";

int main() {
	char * name = NULL;
	int card = cards.get_default_input(&name);
	printf("use card %d, '%s'\n", card, name);
}
