#include "hangman_helpers.h"

#include <stdio.h>

int main(int argc, char *agrv[]) {
    if (argc != 2) {
        printf("usage: ");
        for (int i = 0; i < argc; i++)
            printf("%s ", agrv[i]);
        printf("\nif the secret is multiple words, you must quote it.");
        return 1;
    }
    if (!validate_secret(agrv[1]))
        return 1;
    const char *SECRET = agrv[1];
    int badguess = 0, win = 1;
    char alphabet[26] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
    char DISPLAY[257]
        = "________________________________________________________________________________________"
          "________________________________________________________________________________________"
          "________________________________________________________________________________";
    char guess;
    for (unsigned long i = 0; i < strlen(SECRET); i++) {
        if (string_contains_character(punctuation, SECRET[i]))
            DISPLAY[i] = SECRET[i];
    }
    DISPLAY[strlen(SECRET)] = '\0';
    do {
        printf("%s", CLEAR_SCREEN);
        printf("%s", arts[badguess]);
        printf("\n\n    Phrase: ");
        win = 1;
        for (unsigned long i = 0; i < strlen(SECRET); i++) {
            printf("%c", DISPLAY[i]);
            if (DISPLAY[i] == '_')
                win = 0;
        }
        printf("\nEliminated: ");
        for (unsigned long i = 0; i < 26; i++) {
            if (alphabet[i] != ' ')
                printf("%c", alphabet[i]);
        }
        if (badguess == LOSING_MISTAKE) {
            printf("\n\nYou lose! The secret phrase was: ");
            for (unsigned long i = 0; i < strlen(SECRET); i++) {
                printf("%c", SECRET[i]);
            }
            printf("\n");
            break;
        }
        if (win == 1) {
            printf("\n\nYou win! The secret phrase was: ");
            for (unsigned long i = 0; i < strlen(SECRET); i++) {
                printf("%c", SECRET[i]);
            }
            printf("\n");
            break;
        }
        printf("\n\n");
        do {
            printf("Guess a letter: ");
            guess = read_letter();
        } while (string_contains_character(alphabet, guess)
                 || string_contains_character(punctuation, guess)
                 || string_contains_character(DISPLAY, guess) || !is_lowercase_letter(guess));
        if (string_contains_character(SECRET, guess)) {
            for (unsigned long i = 0; i < strlen(SECRET); i++) {
                if (SECRET[i] == guess)
                    DISPLAY[i] = guess;
            }
        } else {
            alphabet[guess - 'a'] = guess;
            badguess++;
        }
    } while (1);
    return 0;
}
