// MARK: S21_cat

/*
`cat` | дублирует построчно поток ввода.
`cat [file]` | выводит файл в консоль.
`cat [file1] [file2]` | выводит последовательно file1 и file2.
`cat [file1] - [file2]` | выводит file1 затем дублирует построчно поток ввода до `EOF` затем выводит file2. |
`cat -v [file]` | заменяет все символы с учетом ^ и M- notation.
`cat -n [file]` | нумерует все строки.
`cat -n [file] [file]` | нумерует все строки, начиная с начала для каждого файла .
`cat -e [file]` | выводит \n как $.
`cat -s [file]` | не выводит больше одной пустой строки подряд.
`cat -b [file]` | нумерует все не пустые строки.
`cat -t [file]` | выводит \t как ^I.
`cat -bensvt [file]` | всe выше перечисленные опции с учетом приоритетов.
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>


typedef struct options {
    bool non_blank_rows; // -b Number of non-empty strings
    bool show_end; // -e $ at the end of string
    bool all_rows; // -n Number of strings
    bool squeezed; // -s Skipping of unnecessary strings
    bool show_tabs; // -t ^| instead of tabulation
    bool show_all; // -v prints chars with codes: <= 31 on ^+64.
    // code 127 on ^? besides tabulation and the end of the line */
    int count; // should be renamed.
    int emptyLinesCounter; // property we need for correct work of -s flag
} options;

void catPrintFileContent(FILE *file, options *currentOptionsSet);
void catOpenFile(int argc, char *argv[], options *currentOptionsSet);
void parseOptions(int argc, char *argv[], options *currentOptionsSet);
void parseShortFlags(char *arg, options *currentOptionsSet);
void getCatFinalResult(int *content, int *previous, options *currentOptionsSet, FILE *file);
void applyFlagV(int *content, options *currentOptionsSet);
void getEmptyLines(int *content, int *previous, options *currentOptionsSet);
void applyFlagS(int *content, FILE *file, options *currentOptionsSet);
void applyFlagN(int previous, options *currentOptionsSet);
void parseLongFlags(char *argv, options *currentOptionsSet);

int main(int argc, char *argv[]) {
    
    if (argc == 1) {
        options currentOptionsSet = {0};
        catPrintFileContent(stdin, &currentOptionsSet);
        
    } else {
        
      // if there is a flag:
      if (argv[1][0] == '-') {
        options currentOptionsSet = {0};
          parseOptions(argc, argv, &currentOptionsSet);
          
          // if there is zero flags:
        if (!currentOptionsSet.count)
          fprintf(stderr, "s21_cat: invalid options\n");
          
        else {
          printf("Parsed options successfully: %d\n", currentOptionsSet.count);
          // apply parsed options and proceed to the file:
          catOpenFile(argc, argv, &currentOptionsSet);
        }
      } else
        catOpenFile(argc, argv, NULL); // Proceed to the file without any flags
    }
    return 0;
}

// Read char by char until the end of the file and print to the stdout
void catPrintFileContent(FILE *file, options *currentOptionsSet) {
    int content;
    int previous;
    if (currentOptionsSet) {
        currentOptionsSet->emptyLinesCounter = 0;
        currentOptionsSet->count = 1;
    }
    while ((content = fgetc(file)) != EOF) {
        if (currentOptionsSet) {
            getCatFinalResult(&content, &previous, currentOptionsSet, file);
        }
        if (content != EOF) {
            fputc(content, stdout);
        }
    }
}

// Attempt to open file for reading. If success let's read file char by char. Otherwise print an error
void catOpenFile(int argc, char *argv[], options *currentOptionsSet) {
    // If there is a few flags let's count it, otherwise move towards the file
    int i = currentOptionsSet ? 1 + currentOptionsSet->count : 1;
    for (; i < argc; i++) {
        FILE *file = fopen(argv[1], "r");
        if (file) {
            catPrintFileContent(file, currentOptionsSet);
            fclose(file);
        } else {
            fprintf(stderr, "cat: %s: No such file or directory\n", argv[1]);
        }
    }
}

// iterate thgrough an array of arguments in attemt to see some flags. If there is uncorrect form of it, leave the function with unsuccessful result of reading flags.
void parseOptions(int argc, char *argv[], options *currentOptionsSet) {
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            currentOptionsSet->count++;
            
            if (!strstr(argv[i] + 1, "-")) {
                parseShortFlags(argv[i], currentOptionsSet);
                
            } else if (argv[i][1] == '-') {
                parseLongFlags(argv[i], currentOptionsSet);
            }
            
        }
        if (!currentOptionsSet->count) {
            break;
        }
    }
}

void parseShortFlags(char *arg, options *currentOptionsSet) {
    int len = strlen(arg);
    for (int j = 1; j < len; j++) {
        if (currentOptionsSet->count) {
            switch (arg[j]) {
                case 'b':
                    currentOptionsSet->non_blank_rows = true;
                    break;
                case 'e':
                    currentOptionsSet->show_end = true;
                    currentOptionsSet->show_all = true;
                    break;
                case 'n':
                    currentOptionsSet->all_rows = true;
                    break;
                case 's':
                    currentOptionsSet->squeezed = true;
                    break;
                case 't':
                    currentOptionsSet->show_tabs = true;
                    currentOptionsSet->show_all = true;
                    break;
                case 'E':
                    currentOptionsSet->show_end = true;
                    break;
                case 'T':
                    currentOptionsSet->show_tabs = true;
                    break;
                    
                // if flag is not valid let's end the search
                default:
                    currentOptionsSet->count = 0;
                    break;
            }
        } else
            break;
    }
}

void getCatFinalResult(int *content, int *previous, options *currentOptionsSet, FILE *file) {
    getEmptyLines(content, previous, currentOptionsSet);
    applyFlagS(content, file, currentOptionsSet);
    if (*content != EOF) {
        applyFlagV(content, currentOptionsSet);
        applyFlagN(*previous, currentOptionsSet);
    }

}

void applyFlagV(int *content, options *currentOptionsSet) {
    if (*content == '\n') {
        if (currentOptionsSet->show_end) {
            printf("$");
        } else if (*content == '\t') {
            if (currentOptionsSet->show_tabs) {
                printf("^");
                *content = 'I';
            }
        } else if (currentOptionsSet->show_all) {
            if (*content <= 31) {
                printf("^");
                *content += 64;
            } else if (*content == 127) {
                printf("^");
                *content = '?';
            } else if (*content >= 128 && *content < 128 + 32) {
                printf("M-^");
                *content -= 64;
            }
        }
    }
}

// probably flag S
void getEmptyLines(int *content, int *previous, options *currentOptionsSet) {
  if (*previous == '\n' && *content == '\n')
    currentOptionsSet->emptyLinesCounter++;
  else
    currentOptionsSet->emptyLinesCounter = 0;
}

// probably flag S
void applyFlagS(int *content, FILE *file, options *currentOptionsSet) {
    if (currentOptionsSet->squeezed && currentOptionsSet->emptyLinesCounter > 1) {
        while (*content == '\n') {
            *content = fgetc(file);
        }
        currentOptionsSet->emptyLinesCounter = 0;
    }
}

void applyFlagN(int previous, options *currentOptionsSet) {
  if (previous == '\n' && (currentOptionsSet->all_rows || currentOptionsSet->non_blank_rows)) {
    if (!(currentOptionsSet->non_blank_rows && currentOptionsSet->emptyLinesCounter > 0)) {
      printf("%6d\t", currentOptionsSet->count);
      currentOptionsSet->count++;
    }
  }
}

void parseLongFlags(char *argv, options *currentOptionsSet) {
    if (!strcmp(argv + 2, "number-nonblank")) {
        currentOptionsSet->non_blank_rows = true;
    } else if (!strcmp(argv + 2, "number")) {
        currentOptionsSet->all_rows = true;
    } else if  (!strcmp(argv + 2, "squeeze-blank")) {
        currentOptionsSet->squeezed = true;
    }else {
        currentOptionsSet->count = 0;
    }
}
