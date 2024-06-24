# MicahSoft Word

## History

> MicahSoft Word, a terminal-based word processor. I made this project originally when I was completing my advanced python course and I learned how to manipulate files using Python. If you want to see the original project you may visit it here: [Off-Brand MS Word](https://replit.com/@MicahGray3/Off-Brand-MS-Word).

## This Project

> In this project, I am looking to remaster MicahSoft Word. With my current intrest in C programming, I wanted to create a project to practice and demonstrate my skills. I chose my old Python project to try and replicate and possibly improve using only C.

> The project is now complete as of 6/24/24. MicahSoft Word C edition is a major improvement compared to the original.

### Changes

---

- In the original msword, you can only make text files (so that you cannot modify the source code).
- In this version, you can create any kind of file and files are handled in the regenerating "mscache" folder.
- I added file checking and loops to prompt the user to rename files if a duplicate name is spotted or a file name was not found.
- In the original, there was no way to see the files you were writing in.
- In this version, the file can be viewed in read mode or while you write.
- Lines of text are saved after each newline (enter pressed) instead of when the writing screen is closed.
- Using arrays, you are now able to efficiently add lines, remove lines and modify lines while writing, the original could only modify existing lines.

More details of the program is explained in the information file.

## Installation

> Once you clone this project, you may need to compile the program if you cannot used the already compiled _msword.exe_ object. I have used both clang and gcc during development of MicahSoft Word.

> To compile with gcc use this command:
> `gcc -o msword main.c include/actor.c include/menu.c inlclude/tools.c`

> For clang it should be:
> `clang -o msword main.c include/actor.c include/menu.c inlclude/tools.c`

> The "msword" after the -o argument can be whatever you want to name the program, but for this example when the compiled object is created you simply need to navigate to the project folder and type in `./msword.exe` or `./msword` to run the project

I recommend that you use a terminal with a dark theme rather than a light theme when running MicahSoft Word.
