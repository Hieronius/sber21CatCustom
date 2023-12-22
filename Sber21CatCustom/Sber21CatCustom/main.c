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

#include <stdio.h>
#include <getopt.h> // "get options"

// int getopt long (int argc, char **argv, const char *shortopts,
// const struct option *longopts, int *longind)

// int optind - global variable

// const char *shortopts - it's a set of flags for our cat.
// it's variable need to understand when our flags are over and file path is started

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    return 0;
}
