# include "menu.h"
# include <ctype.h>
# include <stdio.h>

void clear_screen(void) {
    printf("\x1b[2J");
}

void title_screen(void) {
    printf(" \n");
    printf("\x1b[44m                                              \x1b[0m\n");
    printf("\x1b[44m         \x1b[0m       MicahSoft Word       \x1b[44m         \x1b[0m\n");
    printf("\x1b[44m         \x1b[0m         C Edition          \x1b[44m         \x1b[0m\n");
    printf("\x1b[44m                                              \x1b[0m\n");
    printf("\x1b[44m  \x1b[0m                                          \x1b[44m  \x1b[0m\n");
    printf("\x1b[44m  \x1b[0m       \x1b[47mn\x1b[0mew file        \x1b[47mf\x1b[0mind file          \x1b[44m  \x1b[0m\n");
    printf("\x1b[44m  \x1b[0m       \x1b[47mc\x1b[0mopy file       \x1b[47md\x1b[0melete file        \x1b[44m  \x1b[0m\n");
    printf("\x1b[44m  \x1b[0m       \x1b[47mi\x1b[0mnformation     \x1b[47mq\x1b[0muit               \x1b[44m  \x1b[0m\n");
    printf("\x1b[44m  \x1b[0m                                          \x1b[44m  \x1b[0m\n");
    printf("\x1b[44m                                              \x1b[0m\n");
}

int title_selection(void) {
    char response; 

    do {
        clear_screen();
        title_screen();
        printf("->");
        response = getchar();

    } while (response != 'n' && response != 'f' && response !='c' && response != 'd' && response != 'i' && response != 'q');

    switch (tolower(response)) {
        case 'n':
        return 0;
        case 'f':
        return 1;
        case 'c':
        return 2;
        case 'd':
        return 3;
        case 'i':
        return 4;
        case 'q':
        return 5;
        default:
        return 9;
    }
}


