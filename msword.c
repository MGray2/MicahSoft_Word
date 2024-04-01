# include <stdio.h>
# include <string.h>
# include <ctype.h>
# include <errno.h>
# include <unistd.h>
# include <unistd.h>

# ifdef _WIN32 // for Windows users
# include <direct.h>
# define mkdir(path, mode) _mkdir(path)
# else // for UNIX users (mac and linux)
# include <sys/stat.h>
# include <sys/types.h>
# endif



void title_screen(void);

int title_selection(void);

void clear_screen(void);

void new_file_screen(void);

int main() {
    clear_screen();
    const char *directory = "mscache";
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("getcwd() error");
    return 1;
}
    if (mkdir(directory, 0777) == 0) {
        printf("\x1b[32mDirectory created. Your documents will be located in '%s'.\x1b[0m", directory);
        sleep(1);
    } else {
        perror("\x1b[31mCannot create directory\x1b[0m");
        sleep(1);
    }

    
    // FILE *target_file;

    // target_file = fopen("test.txt", "w");
    // fclose(target_file);

    int response = title_selection();
    switch (response) {
        case 0:
        break;
        case 5:
        break;
        default:
        break;
    }

    return 0;

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

void clear_screen(void) {
    printf("\x1b[2J");
}
