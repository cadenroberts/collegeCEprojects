#include "names.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
typedef enum { DOT, LEFT, CENTER, RIGHT } Position;
const Position die[6] = { DOT, DOT, DOT, LEFT, CENTER, RIGHT };
int main(void) {
    int num_players = 3;
    printf("Number of players (3 to 10)? ");
    int scanf_result = scanf("%d", &num_players);
    if (scanf_result < 1 || num_players < 3 || num_players > 10) {
        fprintf(stderr, "Invalid number of players. Using 3 instead.\n");
        num_players = 3;
    }
    int player[10] = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };
    unsigned seed = 4823;
    printf("Random-number seed? ");
    scanf_result = scanf("%u", &seed);
    if (scanf_result < 1) {
        fprintf(stderr, "Invalid seed. Using 4823 instead.\n");
        seed = 4823;
    }
    srandom(seed);
    bool winner = false;
    int n = 0, win = 0;
    while (!winner) {
        int rolls = player[n];
        if (rolls > 3)
            rolls = 3;
        if (rolls > 0) {
            while (rolls-- > 0) {
                Position roll = die[random() % 6];
                if (roll == LEFT) {
                    if (n + 1 < num_players)
                        player[n + 1]++;
                    else
                        player[0]++;
                } else if (roll == DOT)
                    player[n]++;
                else if (roll == RIGHT) {
                    if (n == 0)
                        player[num_players - 1]++;
                    else
                        player[n - 1]++;
                }
                player[n]--;
            }
            printf("%s: ends her turn with %d\n", player_name[n], player[n]);
        }
        if (n + 1 < num_players)
            n++;
        else
            n = 0;
        int checker = -1;
        for (int i = 0; i < num_players; i++) {
            if (player[i] > 0) {
                checker++;
                win = i;
            }
        }
        if (checker == 0)
            winner = true;
    }
    printf("%s won!\n", player_name[win]);
    return 0;
}
