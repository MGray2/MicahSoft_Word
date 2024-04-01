# include <stdio.h>
# include <string.h>
# include <ctype.h>
# include <errno.h>
# include <unistd.h>
# include <unistd.h>

# include "include/menu.c"

# ifdef _WIN32 // for Windows users
# include <direct.h>
# define mkdir(path, mode) _mkdir(path)
# else // for UNIX users (mac and linux)
# include <sys/stat.h>
# include <sys/types.h>
# endif


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

