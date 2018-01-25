//    AGBSTRING - A utility that builds an asm file from a set of strings
//    translated to a variety of languages supported by Pokémon Ruby and Sapphire.
//    Copyright (C) 2018 PikalaxALT
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
//    This repository is hosted at https://github.com/pikalaxalt/agbstring.
//    You may contact the developer and raise bug reports there.

#include <stdio.h>
#include <errno.h>
#include <stdnoreturn.h>
#include <stdlib.h>
#include <strings.h>

#define NELEM(x) ((sizeof(x)) / (sizeof(*(x))))

char * languages[] = {
    "",
    "japanese",
    "english",
    "spanish",
    "french",
    "german",
    "italian"
};

noreturn void usage(void) {
    fprintf(stderr, "usage: agbstring STRINGS_FILE OUTPUT_FILE LANGUAGE\n");
    exit(1);
}

int main(int argc, char * argv[]) {
    if (argc < 4) usage();

    FILE * stringfile = fopen(argv[1], "r");
    FILE * asmfile = fopen(argv[2], "w");

    if (errno) {
        fclose(stringfile);
        fclose(asmfile);
        usage();
    }

    int language = 0;
    for (language = 0; language < NELEM(languages); language++) {
        if (strcasecmp(languages[language], argv[3]) == 0)
            break;
    }
    if (language == NELEM(languages)) {
        fprintf(stderr, "Unrecognized language: %s\n", argv[3]);
        usage();
    }

    char *line = NULL;
    size_t line_len = 0;

    fprintf(asmfile, "\t.include \"include/macros.inc\"\n");
    fprintf(asmfile, "\t.include \"constants/constants.inc\"\n\n");
    fprintf(asmfile, "\t.section .rodata\n\n");

    char *pos;
    while (getline(&line, &line_len, stringfile) != -1) {
        if (!*line) continue;
        if ((pos=strchr(line, '\n')) != NULL)
            *pos = '\0';
        fprintf(asmfile, "%s::\n\t.string \"", line);
        for (int i = 1; i < NELEM(languages) && getline(&line, &line_len, stringfile) != -1; i++) {
            if (*line == '\n') break;
            if ((pos=strchr(line, '\n')) != NULL)
                *pos = '\0';
            if (i == language) {
                fprintf(asmfile, "%s", line);
            }
        }
        fprintf(asmfile, "$\"\n\n");
    }

    free(line);
    return errno;
}
