//
// Created by Edw590 on 26/04/2025.
//

#include <string.h>
#include <stdlib.h>
#include "UtilsStrings.h"

/**
 * Splits a string into an array of strings based on a delimiter.
 *
 * @param str the string to split
 * @param delimiter the characters to split the string by
 * @param count a pointer to an integer that will be set to the number of split strings or NULL to ignore
 *
 * @return an array of the split strings, all copied and allocated DYNAMICALLY, or NULL on failure
 */
char **splitString(const char *str, const char *delimiter, int *count) {
	int token_count = 0;
	const char *current = str;
	size_t delim_len = strlen(delimiter);

	// First, count how many tokens
	while (*current) {
		int pos = indexOf(current, delimiter);
		if (pos == -1) {
			break;
		}

		token_count++;
		current += pos + delim_len;
	}
	token_count++;  // Last token

	char **result = malloc(token_count * sizeof(char *));
	if (result == NULL) {
		return NULL;
	}

	current = str;
	int idx = 0;
	while (*current) {
		int pos = indexOf(current, delimiter);
		if (pos == -1) {
			// Last token
			result[idx] = strdup(current);

			break;
		} else {
			result[idx] = malloc(pos + 1);
			if (!result[idx]) {
				return NULL;
			}
			strncpy(result[idx], current, pos);
			result[idx][pos] = '\0';
			current += pos + delim_len;
			idx++;
		}
	}

	if (count != NULL) {
		*count = token_count;
	}

	return result;
}

/**
 * Finds the index of a substring in a string.
 *
 * @param str the string to search
 * @param substr the substring to find
 *
 * @return the index of the substring in the string, or -1 if not found
 */
int indexOf(const char *str, const char *substr) {
	size_t len_str = strlen(str);
	size_t len_sub = strlen(substr);

	if (len_str == 0 || len_sub == 0 || len_sub > len_str) {
		return -1;
	}

	for (int i = 0; i <= len_str - len_sub; i++) {
		if (strncmp(&str[i], substr, len_sub) == 0) {
			return i;
		}
	}

	return -1;
}
