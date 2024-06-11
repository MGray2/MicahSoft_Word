#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "include/menu.h"
#include "include/tools.h"

#ifdef _WIN32 // for Windows users
#include <direct.h>
#include <windows.h>
#define mkdir(path, mode) _mkdir(path)

#else // for UNIX users (mac and linux)
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

int main()
{
    clear_screen();
    const char *directory = "mscache";
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("Current working directory: %s\n", cwd);
    }
    else
    {
        perror("getcwd() error");
        return 1;
    }
    if (mkdir(directory, 0777) == 0)
    {
        print_grn("Directory created. Your documents will be located in:", directory, NULL);
#ifdef _WIN32
        Sleep(3000);
#else
        sleep(3);
#endif
    }
    else
    {
        perror("\x1b[31mCannot create directory\x1b[0m");
    }

    int response = title_selection();
    switch (response)
    {
    case 0:
        // New File
        file_write_screen(new_file_screen());
        main();
        break;
    case 1:
        // Find Existing File
        file_write_screen(file_find_screen());
        main();
        break;
    case 2:
        // Information
        break;
    case 3:
        // Quit the program
        break;
    default:
        break;
    }
    return 0;
}
