#include "hangman_helpers.h"

int main(void) {
    char *toomuch
        = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
          "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
          "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    char *justright
        = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
          "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
          "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    char *lower = "abcdefghijklmnopqrstuvwxyzABC!&*123";
    char test = 'f';
    char test1 = '#';
    int a = 0;
    for (int i = 0; i < 35; i++) {
        if (!(is_lowercase_letter(lower[i])))
            a++;
    }
    if ((validate_secret(justright)) && (!(validate_secret(toomuch))) && (a == 9)
        && (string_contains_character(lower, test)) && (!(string_contains_character(lower, test1))))
        printf("success");
    else
        printf("fail");
    return 0;
}
