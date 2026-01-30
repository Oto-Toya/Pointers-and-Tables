/*
 * File: status.c
 * Author: Gary Hubley
 * Date: 2026/01/17
 * Description: Contains function definitions for HTTP status lookup utility
 */

#include "status.h"

 // ---- Sorted table of HTTP status codes ----
static const StatusEntry STATUSTABLE[] = {
    {100, "Continue", "Informational"},
    {101, "Switching Protocols", "Informational"},
    {102, "Processing", "Informational"},
    {103, "Early Hints", "Informational"},
    {200, "OK", "Success"},
    {201, "Created", "Success"},
    {202, "Accepted", "Success"},
    {203, "Non-Authoritative Information", "Success"},
    {204, "No Content", "Success"},
    {205, "Reset Content", "Success"},
    {206, "Partial Content", "Success"},
    {207, "Multi-Status", "Success"},
    {208, "Already Reported", "Success"},
    {226, "IM Used", "Success"},
    {300, "Multiple Choices", "Redirection"},
    {301, "Moved Permanently", "Redirection"},
    {302, "Found", "Redirection"},
    {303, "See Other", "Redirection"},
    {304, "Not Modified", "Redirection"},
    {305, "Use Proxy (since HTTP/1.1)", "Redirection"},
    {306, "Switch Proxy", "Redirection"},
    {307, "Temporary Redirect", "Redirection"},
    {308, "Permanent Redirect", "Redirection"},
    {400, "Bad Request", "Client Error"},
    {401, "Unauthorized", "Client Error"},
    {402, "Payment Required", "Client Error"},
    {403, "Forbidden", "Client Error"},
    {404, "Not Found", "Client Error"},
    {405, "Method Not Allowed", "Client Error"},
    {408, "Request Timeout", "Client Error"},
    {409, "Conflict", "Client Error"},
    {410, "Gone", "Client Error"},
    {413, "Payload Too Large", "Client Error"},
    {415, "Unsupported Media Type", "Client Error"},
    {418, "I'm a teapot", "Client Error"},
    {429, "Too Many Requests", "Client Error"},
    {500, "Internal Server Error", "Server Error"},
    {501, "Not Implemented", "Server Error"},
    {502, "Bad Gateway", "Server Error"},
    {503, "Service Unavailable", "Server Error"},
    {504, "Gateway Timeout", "Server Error"},
    {511, "Network Authentication Required", "Server Error"},
};

const StatusEntry* statustablebegin(void) { return STATUSTABLE; }

const StatusEntry* statustableend(void) {
    return STATUSTABLE + (sizeof(STATUSTABLE) / sizeof(STATUSTABLE[0]));
}

size_t statustablesize(void) {
    return (size_t)(statustableend() - statustablebegin());
}

/* name: LookupStatus
 *
 * Description: Finds the given HTTP status given the code number.
 * @param code: the HTTP error code.
 * @return: Pointer to the status object in the STATUSTABLE if found. NULL
 * otherwise side effects: N/A
 */
const StatusEntry* LookupStatus(int code) {
	const StatusEntry* ptr = STATUSTABLE;            // ptr points to the beginning of the table
	while (ptr < statustableend())                  // iterate until ptr reaches the end of the table
    {
		if (ptr->code == code)                      // check if the current entry's code matches the input code
			return ptr;                             // return pointer to the matching entry
		ptr++;                                      // move to the next entry in the table
    }
    return NULL;
    // Example of accessing the last element:
	// int lastCode =

    // TODO: Implement search using pointers only.
}

/* name: PrintRange
 *
 * Description: Prints the range of status from lo to hi (inclusive)
 * @param stream: the file to print to (stdout, stderr, filename, etc)
 * @param lo: the lowest status code to print
 * @param hi: the highest status code to print
 * @return: N/A
 * side effects: N/A
 */
void PrintRange(FILE* stream, int lo, int hi) {
	const StatusEntry* start = LookupStatus(lo);        // find the start of the range
	const StatusEntry* end = LookupStatus(hi);          // find the end of the range
	int locallo = lo;
	int localhi = hi;

	// If start or end is NULL, adjust to nearest valid entries
    if (start == NULL) {
		if (lo <= (STATUSTABLE + 0)->code)              // if lo is less than or equal to the lowest code
        {
			start = statustablebegin();                 // set start to the beginning of the table
			locallo = start->code;                      // update locallo to the lowest code
        }
        else
        { 
			// increment locallo until a valid status is found
            while (start == NULL) {
			    locallo = locallo + 1;
				start = LookupStatus(locallo);          // look for the next valid status
            }
        }
    }
	// Similar logic for end
    if (end == NULL) {
		if (hi >= statustableend)                       // if hi is greater than or equal to the highest code
		{
			end = statustableend() - 1;                 // set end to the last valid entry
			localhi = end->code;
        }
        else{
			// decrement localhi until a valid status is found
            while (end == NULL) {
			localhi = localhi - 1;
            end = LookupStatus(localhi);
            }
        }
    }
	
	const StatusEntry* ptr = start;                     // ptr starts at the beginning of the range
    
	// Print statuses from locallo to localhi, if within bounds
    while (locallo <= ptr->code && ptr->code <= localhi && ptr != statustableend()-1)
    {
		PrintStatus(stream, ptr);                       // print the current status
		ptr++;
    }
    
}

void PrintStatus(FILE* stream, const StatusEntry* status) {
	fprintf(stream, "%d %s (%s)\n", status->code, status->name, status->category);  // Print formatted status information
}
