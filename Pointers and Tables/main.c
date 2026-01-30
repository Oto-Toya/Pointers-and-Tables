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

	while (*s && isspace((unsigned char)*s)) {      // Skip leading whitespace
        s++;
    }

	number = strtol(s, &end, 10);                   // Convert string to long integer

	if (end == s) {                                 // No digits were found
        return false;   
    }

	while (*end && isspace((unsigned char)*end)) {  // Skip trailing whitespace
        end++;
    }

	if (*end != '\0') {                             // Extra characters after number
        return false;
    }

	*out = (int)number;                             // Store the result

    return true;
}

// Main program entry point, processes command-line arguments.
// Stores the numbers of argument into variables and calls appropriate functions.
int main(int argc, char* argv[]) {
	int arugCount = 2;                  // Expected number of arguments
	int arrayNumOfData = 1;             // Index of the argument to process

	// Check for correct number of arguments
    if (argc != arugCount) {
        Usage(argv[0]); // Calls usage with program name (first argument from CLI)
        return 1;
    }

    char* arg = argv[arrayNumOfData];   // Argument passed in from CLI
	char* result = strchr(arg, '-');    // Check if argument contains a '-'

	// Process range input if '-' is found
    if (result) {
        int i = 0;
		int j = 0;
		char first[BUF_LEN];            // Buffer for first number
		char second[BUFSIZ];            // Buffer for second number

		// Extract first number, while not reaching '-' or end of string
        while (*(arg + i) != '-' && *(arg + i) != '\0') {
            *(first+i) = *(arg + i);
            i++;
        }
		*(first + i) = '\0';        // Null-terminate the first number string

		i++;                        // Move past the '-'

		// Extract second number until end of string
        while (*(arg + i) != '\0') {

            *(second +j) = *(arg + i);
			i++;    // Move to next character in input
			j++;    // Move to next position in second buffer
        }
		*(second + j) = '\0';       // Null-terminate the second number string

		int hi = 0;                 // High end of range
		int lo = 0;                 // Low end of range

		// Parse the two numbers
		// If parsing fails, print error and usage
		if (!ParseInt(first, &lo) || !ParseInt(second, &hi)) {
			fprintf(stderr, "Error: Invalid input\n");
			Usage(argv[0]);
            return 2;
        }

		// If low is less than or equal to high, print the range
        if (lo <= hi){
            PrintRange(stdout, lo, hi);
            return 1;
        }
		// If low is greater than high, print error and usage
        else {
            fprintf(stderr, "Error: Invalid Input\n");
            Usage(argv[0]);
			return 2;
        }
    }
	// Process single number input, if no '-' is found
	else {
		int singleNum = 0;                              // Variable to hold the single number

		// Parse the single number, if parsing fails, print error and usage
        if (!ParseInt(arg, &singleNum)) {
            fprintf(stderr, "Error: Invalid input\n");
            Usage(argv[0]);
            return 2;
        }
		// Lookup the status for the single number
        else {
			const StatusEntry* status = LookupStatus(singleNum);    // Lookup status
			// If status is found, print it; otherwise, print error
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

    return 0;
}
