# include <stdio.h>
# include <string.h>
# include <ctype.h>
# include <errno.h>

# include "headers/menu.h"

# ifdef _WIN32 // for Windows users
# include <direct.h>
# define mkdir(path, mode) _mkdir(path)
# else // for UNIX users (mac and linux)
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>
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
    } else {
        perror("\x1b[31mCannot create directory\x1b[0m");
    }


    int response = title_selection();
    switch (response) {
        case 0:
        // New File
        new_file_screen();
        break;
        case 1:
        // Find Existing File
        file_find_screen();
        break;
        case 2:
        copy_file_screen();
        break;
        case 3:
        // Delete File
        case 4:
        // Information
        case 5:
        break;
        default:
        break;
    }

    return 0;

}

