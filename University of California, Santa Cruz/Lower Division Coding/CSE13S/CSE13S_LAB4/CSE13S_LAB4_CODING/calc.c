#include "messages.h"
#include "operators.h"
#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int a;
    char *save, *token, buffer[1024];
    bool e = false;
    double dtoken;
    while ((a = getopt(argc, argv, "mh")) != -1) {
        switch (a) {
        case 'm': break;
        case 'h': printf(USAGE, argv[0]); return 0;
        case '?': fprintf(stderr, USAGE, argv[0]); return 1;
        }
    }

    while (!feof(stdin)) {
        fprintf(stderr, "> ");
        fgets(buffer, 1024, stdin);
        token = strtok_r(buffer, " ", &save);
        e = false;
        while (token != NULL && !e && !feof(stdin)) {
            for (int i = 0; i < (int) strlen(token); ++i) {
                if (token[i] == '\n')
                    token[i] = '\0';
            }
            if (parse_double(token, &dtoken)) {
                if (stack_push(dtoken) == false) {
                    fprintf(stderr, ERROR_NO_SPACE, dtoken);
                    e = true;
                }
            } else if (strcmp(token, "+") == 0 || strcmp(token, "-") == 0 || strcmp(token, "*") == 0
                       || strcmp(token, "/") == 0 || strcmp(token, "%") == 0) {
                if (apply_binary_operator(binary_operators[(int) token[0]]) == false) {
                    fprintf(stderr, ERROR_BINARY_OPERATOR);
                    e = true;
                }
            } else if (strcmp(token, "s") == 0 || strcmp(token, "c") == 0 || strcmp(token, "t") == 0
                       || strcmp(token, "a") == 0 || strcmp(token, "r") == 0) {
                if (apply_unary_operator(my_unary_operators[(int) token[0]]) == false) {
                    fprintf(stderr, ERROR_UNARY_OPERATOR);
                    e = true;
                }
            } else {
                if (strlen(token) == 1) {
                    fprintf(stderr, ERROR_BAD_CHAR, token[0]);
                    e = true;
                } else {
                    fprintf(stderr, ERROR_BAD_STRING, token);
                    e = true;
                }
            }
            token = strtok_r(NULL, " ", &save);
        }
        if (e == false && !feof(stdin)) {
            stack_print();
            printf("\n");
        }
        stack_clear();
    }
    return 0;
}
