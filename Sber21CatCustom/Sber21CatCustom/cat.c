#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct options {
  bool bFlag;  // -b Number of non-empty strings
  bool eFlag;  // -e $ at the end of string
  bool nFlag;  // -n Number of strings
  bool sFlag;  // -s Skipping of unnecessary strings
  bool tFlag;  // -t ^| instead of tabulation
  bool vFlag;  // -v prints chars with codes: <= 31 on ^+64.
               // code 127 on ^? besides tabulation and the end of the line */
  int flagsCounter;       // should be renamed.
  int emptyLinesCounter;  // property we need for correct work of -s flag
} options;

void catPrintFileContent(FILE *file, options *currentOptionsSet);
void catOpenFile(int argc, char *argv[], options *currentOptionsSet);
void parseOptions(int argc, char *argv[], options *currentOptionsSet);
void parseShortFlags(char *arg, options *currentOptionsSet);
void getCatFinalResult(int *content, int *previous, options *currentOptionsSet,
                       FILE *file);
void applyFlagsETV(int *content, options *currentOptionsSet);
void countEmptyLines(int *content, int *previous, options *currentOptionsSet);
void applyFlagS(int *content, FILE *file, options *currentOptionsSet);
void applyFlagN(int previous, options *currentOptionsSet);
void parseLongFlags(char *argv, options *currentOptionsSet);

int main(int argc, char *argv[]) {
  if (argc == 1) {
    catPrintFileContent(stdin, NULL);

  } else {
    // if there is a flag:
    if (argv[1][0] == '-') {
      options currentOptionsSet = {0};
      parseOptions(argc, argv, &currentOptionsSet);

      // if there is zero flags:
      if (!currentOptionsSet.flagsCounter) {
        fprintf(stderr, "s21_cat: invalid options\n");

      } else {
        // apply parsed options and proceed to the file:
        catOpenFile(argc, argv, &currentOptionsSet);
      }
    } else
      catOpenFile(argc, argv, NULL);  // Proceed to the file without any flags
  }
  return 0;
}

// Read char by char until the end of the file and print to the stdout
void catPrintFileContent(FILE *file, options *currentOptionsSet) {
  int content;
  int previous = '\n';
  if (currentOptionsSet) {
    currentOptionsSet->emptyLinesCounter = 0;
    currentOptionsSet->flagsCounter = 1;
  }
  while ((content = fgetc(file)) != EOF) {
    if (currentOptionsSet) {
      getCatFinalResult(&content, &previous, currentOptionsSet, file);
    }
    if (content != EOF) {
      fputc(content, stdout);
    }
    previous = content;
  }
}

// Attempt to open file for reading. If success let's read file char by char.
// Otherwise print an error
void catOpenFile(int argc, char *argv[], options *currentOptionsSet) {
  // If there is a few flags let's count it, otherwise move towards the file
  int i = currentOptionsSet ? 1 + currentOptionsSet->flagsCounter : 1;
  for (; i < argc; i++) {
    FILE *file = fopen(argv[i], "r");
    if (file) {
      catPrintFileContent(file, currentOptionsSet);
      fclose(file);
    } else {
      fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
    }
  }
}

// iterate thgrough an array of arguments in attemt to see some flags. If there
// is uncorrect form of it, leave the function with unsuccessful result of
// reading flags.
void parseOptions(int argc, char *argv[], options *currentOptionsSet) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      currentOptionsSet->flagsCounter++;

      if (!strstr(argv[i] + 1, "-")) {
        parseShortFlags(argv[i], currentOptionsSet);

      } else if (argv[i][1] == '-') {
        parseLongFlags(argv[i], currentOptionsSet);
      }
    }
    if (!currentOptionsSet->flagsCounter) {
      break;
    }
  }
}

void parseShortFlags(char *arg, options *currentOptionsSet) {
  int len = strlen(arg);
  for (int j = 1; j < len; j++) {
    if (currentOptionsSet->flagsCounter) {
      switch (arg[j]) {
        case 'v':
          currentOptionsSet->vFlag = true;
          break;
        case 'b':
          currentOptionsSet->bFlag = true;
          break;
        case 'e':
          currentOptionsSet->vFlag = true;
          currentOptionsSet->eFlag = true;
          break;
        case 'n':
          currentOptionsSet->nFlag = true;
          break;
        case 's':
          currentOptionsSet->sFlag = true;
          break;
        case 't':
          currentOptionsSet->vFlag = true;
          currentOptionsSet->tFlag = true;
          break;
        case 'E':
          currentOptionsSet->eFlag = true;
          break;
        case 'T':
          currentOptionsSet->tFlag = true;
          break;

        // if flag is not valid let's end the search
        default:
          currentOptionsSet->flagsCounter = 0;
          break;
      }
    } else
      break;
  }
}

// Final output of the program
void getCatFinalResult(int *content, int *previous, options *currentOptionsSet,
                       FILE *file) {
  countEmptyLines(content, previous, currentOptionsSet);
  applyFlagS(content, file, currentOptionsSet);
  if (*content != EOF) {
    applyFlagN(*previous, currentOptionsSet);
    applyFlagsETV(content, currentOptionsSet);
  }
}

// There should be placed implementation of -v, -e and -t flags
void applyFlagsETV(int *content, options *currentOptionsSet) {
  if (*content == '\n') {
    if (currentOptionsSet->eFlag) {
      printf("$");
    }
  } else if (*content == '\t') {
    if (currentOptionsSet->tFlag) {
      printf("^");
      *content = 'I';
    }

  } else if (currentOptionsSet->vFlag) {
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

// Empty lines counter here
void countEmptyLines(int *content, int *previous, options *currentOptionsSet) {
  if (*previous == '\n' && *content == '\n') {
    currentOptionsSet->emptyLinesCounter++;
  } else {
    currentOptionsSet->emptyLinesCounter = 0;
  }
}

// Implementation of flag -s
void applyFlagS(int *content, FILE *file, options *currentOptionsSet) {
  if (currentOptionsSet->sFlag && currentOptionsSet->emptyLinesCounter > 1) {
    while (*content == '\n') {
      *content = fgetc(file);
    }
    currentOptionsSet->emptyLinesCounter = 0;
  }
}

// Implemetation of flag -n
void applyFlagN(int previous, options *currentOptionsSet) {
  if (previous == '\n' &&
      (currentOptionsSet->nFlag || currentOptionsSet->bFlag)) {
    if (!(currentOptionsSet->bFlag &&
          currentOptionsSet->emptyLinesCounter > 0)) {
      printf("%6d\t", currentOptionsSet->flagsCounter);
      currentOptionsSet->flagsCounter++;
    }
  }
}

void parseLongFlags(char *argv, options *currentOptionsSet) {
  if (!strcmp(argv + 2, "number-nonblank")) {
    currentOptionsSet->bFlag = true;
  } else if (!strcmp(argv + 2, "number")) {
    currentOptionsSet->nFlag = true;
  } else if (!strcmp(argv + 2, "squeeze-blank")) {
    currentOptionsSet->sFlag = true;
  } else {
    currentOptionsSet->flagsCounter = 0;
  }
}
