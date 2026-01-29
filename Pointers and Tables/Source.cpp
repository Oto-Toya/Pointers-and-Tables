/*
 * File: main.c
 * Author: WenXing Tan
 * Date: 01/27/2026
 * Description: Main program to lookup and print HTTP status codes
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "status.h"

 // Prints usage information to stderr
static void Usage(const char* prog) {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  %s <CODE>\n", prog);
    fprintf(stderr, "  %s <LOW>-<HIGH>\n", prog);
    fprintf(stderr, "Examples:\n");
    fprintf(stderr, "  %s 404\n", prog);
    fprintf(stderr, "  %s 200-204\n", prog);
}

// Takes a string and parses it for an integer value. Ignore surrounding whitespace.
static bool ParseInt(const char* s, int* out) {
    long number;
    char* end;

    while (*s && isspace((unsigned char)*s)) {
        s++;
    }

    number = strtol(s, &end, 10);

    if (end == s) {
        return false;
    }

    while (*end && isspace((unsigned char)*end)) {
        end++;
    }

    if (*end != '\0') {
        return false;
    }

    *out = (int)number;

    // TODO
    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        Usage(argv[0]); // Calls usage with program name (first argument from CLI)
        return 1;
    }

    char* arg = argv[1]; // argument passed in from CLI
    char* result = strchr(arg, '-');

    if (result) {
        int i = 0;
		int j = 0;
        char first[BUF_LEN];
        char second[BUFSIZ];

        while (*(arg + i) != '-' && *(arg + i) != '\0') {
            *(first+i) = *(arg + i);
            i++;
        }
        *(first+i) = '\0';

        i++;

        while (*(arg + i) != '\0') {

            *(second +j) = *(arg + i);
            i++;
            j++;
        }
        *(second + j) = '\0';

        int hi = 0;
        int lo = 0;

		if (!ParseInt(first, &lo) || !ParseInt(second, &hi)) {
			fprintf(stderr, "Error: Invalid input\n");
			Usage(argv[0]);
            return 2;
        }

        if (lo <= hi){
            PrintRange(stdout, lo, hi);
            return 1;
        }
        else {
            fprintf(stderr, "Error: Invalid Input\n");
            Usage(argv[0]);
			return 2;
        }
    }
	else {
        int singleNum = 0;
        if (!ParseInt(arg, &singleNum)) {
            fprintf(stderr, "Error: HTTP status code %d not found\n", singleNum);
            Usage(argv[0]);
            return 2;
        }
        else {
            const StatusEntry* status = LookupStatus(singleNum);
            if (status) {
                PrintStatus(stdout, status);
                return 1;
            }
            else {
                printf("Error: HTTP status code %d not found\n", singleNum);
                return 2;
            }
		}
    }
    //    ParseInt(first, &lo);
    //    ParseInt(second, &hi);

    //    if (lo < hi) {
    //        PrintRange(stdout, lo, hi);
    //    }
    //    else {
    //        printf("Incorrect Inputs");
    //    }
    //}
    //else {
    //    int singleNum = 0;

    //    if (!ParseInt(arg, &singleNum)) {
    //        printf("Incorrect Inputs");
    //    }
    //    else {
    //        const StatusEntry* status = LookupStatus(singleNum);
    //        if (status) {
    //            PrintStatus(stdout, status);
    //        }
    //        else {
    //            printf("Status code not found\n");
    //        }
    //    }
    //}

    // TODO
    // Detect range "A-B"
    // parse first integer
    // parse second integer
    // if first < second, then print range
    // otherwise print error msg and exit
    //
    // Single code
    // Parse integer
    // if invalid int, print error msg and exit
    // lookup value in table
    // print to stdout

    return 0;
}
