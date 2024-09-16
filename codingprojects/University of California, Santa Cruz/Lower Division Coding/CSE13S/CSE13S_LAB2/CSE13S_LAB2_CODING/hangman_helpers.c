#include "hangman_helpers.h"

bool is_lowercase_letter(char c) {
    return (c >= 'a' && c <= 'z');
}

bool validate_secret(const char *secret) {
    int i = 0;
    while (*secret != '\0') {
        if (i > 255) {
            printf("the secret phrase is over 256 characters\n");
            return 0;
        }
        if ((!(string_contains_character(punctuation, *secret)))
            && (!(is_lowercase_letter(*secret)))) {
            printf("invalid character: \'%c\'\nthe secret phrase must contain only "
                   "lowercase letters, spaces, hyphens, and apostrophes\n",
                *secret);
            return 0;
        }
        secret++;
        i++;
    }
    return 1;
}

bool string_contains_character(const char *s, char c) {
    while (*s != '\0') {
        if (*s == c)
            return 1;
        s++;
    }
    return 0;
}

char read_letter(void) {
    char letter = '\n';
    do {
        letter = (char) getchar();
    } while (letter == '\n');
    return letter;
}
