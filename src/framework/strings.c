#include <string.h>
#include <stdlib.h>


void copyText(char **dest, char *text) {
	*dest = malloc(strlen(text) + 1);
	*dest[0] = '\0';
	
	strcpy(*dest, text);
}

void appendText(char **dest, char *text1, char *text2) {
	*dest = malloc(strlen(text1) + strlen(text2) + 1);
	*dest[0] = '\0';
	
	strcat(*dest, text1);
    strcat(*dest, text2);
}
