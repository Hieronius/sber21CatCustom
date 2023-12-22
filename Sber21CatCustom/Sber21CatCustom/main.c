//
//  main.c
//  Sber21CatCustom
//
//  Created by Арсентий Халимовский on 22.12.2023.
//

// MARK: S21_cat

/*
 
 --Long Agruments
 -n Number of strings
 -b Number of non-empty strings
 -s Skipping of unnecessary strings
 -E $ at the end of string
 -T ^| instead of tabulation
 -v Выводит символы с кодами <= 31 через ^+64
 - код 127 через ^? Кроме табуляции и конца строкки */

// MARK: int getopt long (int argc, char **argv, const char *shortopts,const struct option *longopts, int *longind)
/*
 1. argc/argv - amount and arguments of command line that our program got
 2. optsting - string that contains a list of short options. Each char in this string it's a short option like "r" that programm can run. If char has ":" it's mean that this option is require an argument
 3. longtops - it's an array of structs "option" that is defines long options (like "number").
               Each struct contains a name of option, defines if options need an arguments, and can define a variable and return value when function was detected.
 4. longindex - it's a pointer on variable where "getopt_long" will place an index of current option to an array "longopts"
 5. return value - "getopt_long" - return a code of operation:
                    - usually one-char for short options
                    - "0" for long options
                    - "-1" when all options have been processed
                    - "?" when function returns any errors
 */

// int optind - global variable

// const char *shortopts - it's a set of flags for our cat.
// it's variable need to understand when our flags are over and file path is started

/*
struct option {
    const char *name; - name of the long option for example - "number"
    int has_arg; - we should use "no_argument"
    int *flag; - we doesn't need it because flag mean - "write a result of the function above to specific variable. We should just return the function during the cat method
    int val; - name for the short option for example - "n" from "number"
}
*/

#include <stdio.h>
#include <getopt.h> // "get options"
#include <stdlib.h> // need because we should use dynamic memory + "exit" func

typedef struct {
    int b;
    int n;
    int s;
    int E;
    int T;
    int v;
} arguments; // struct with all possible flags for our project "sber21_cat"

arguments *argument_parser(int argc, char **argv);

int main(int argc, char *argv[]) {
    // argc - Arguments count
    // argv - Arguments values with two asterisks because an array of strings(names) it's two-dimensional array of chars. **argv = *argv[]
    
    for (int i = 0; i < argc; i++) {
        printf("argv is %s", *(argv + i));
    }
    
    return 0;
}

arguments *argument_parser(int argc, char **argv) {
    arguments arg = {0}; // our programm will fill all other properties with zeroes
    int opt; // result of flag parsing. opt - option
    opt = getopt_long(argc, argv, "bnsEeTt", NULL, 0);
    switch (opt) {
        case 'b':
            arg.b = 1; // seems like it's just mean "true/false" of being used one of the options
            break;
        case 'n':
            arg.n = 1;
            break;
        case 's':
            arg.s = 1;
            break;
        case 'e':
            arg.E = 1;
            arg.v = 1; // mean the same
            break;
        case 'E':
            arg.E = 1;
            break;
        case 't':
            arg.T = 1;
            arg.v = 1; // mean the same
            break;
        case 'T':
            arg.T = 1;
            break;
        case '?': // if func will return ? print an error in specific place
            perror("ERROR");
            break;
        default:
            break;
    }
    return arg;
}
