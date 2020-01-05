#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// #define MAX_BUFF_SIZE 976892
#define MAX_BUFF_SIZE 980784
#define URL_LIMIT 300

__attribute__((weak)) void describe_url(char *url)
{
    if (strstr(url, "centralesupelec") != NULL)
    {
        printf("Engineering education\n");
        return;
    }
    if (strstr(url, "github") != NULL)
    {
        printf("Code sharing\n");
        return;
    }
    if (strstr(url, "marmiton") != NULL)
    {
        printf("Cooking recipes for home cooks\n");
        return;
    }
    printf("Unknown type of URL\n");
    return;
}

__attribute__((weak))
uint64_t
describe_truncated_url(const char *url, void **buff, int offset)
{
    // truncate URL (URL_LIMIT first characters)
    char truncated_url[URL_LIMIT];
    strcpy(truncated_url, url);
    truncated_url[URL_LIMIT] = '\0';
    // describe the truncated URL
    describe_url(truncated_url);

    buff[offset] = truncated_url;
    return strlen(truncated_url);
}

__attribute__((weak))
uint64_t
process_url(const char *url, int size, bool debug)
{
    // char buff[976889-8*20];
    const char *allowed_chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ/."; //{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '/'};
    const size_t n_allowed = 64;
    // is the i-th character allowed?
    bool is_allowed = false;
    // for loops
    int i = 0;
    int j = 0;
    // url is allowed to have \n at the end
    char allowed_last_char = '\n';
    int last_char_index = size - 1;
    char *err_string = "Invalid character in URL\n";
    // offset for truncated URL
    int offset = -1;
    uint64_t buffer_length = 0;
    if (debug)
    {
        printf("allowed_chars: %s\n", allowed_chars);
        printf("n_allowed: %d\n", n_allowed);
        printf("is_allowed: %d\n", is_allowed);
        printf("i: %d\n", i);
        printf("j: %d\n", j);
        printf("allowed_last_char: %c\n", allowed_last_char);
        printf("last_char: %d\n", last_char_index);
        printf("err_string: %s\n", err_string);
        printf("offset: %d\n", offset);
        printf("buffer_length: %d\n", buffer_length);
    }

    // set offset to the size of the URL for future debug printing
    if (size <= URL_LIMIT)
    {
        offset = size;
    }

    char buff[MAX_BUFF_SIZE];
    strcpy(buff, url);

    for (i = 0; i < size; i++)
    {
        // check if the buffer character is among allowed characters
        for (j = 0; j < n_allowed; j++)
        {
            if (buff[i] == allowed_chars[j])
            {
                is_allowed = true;
            }
        }
        if (i == last_char_index && buff[i] == allowed_last_char)
        {
            is_allowed = true;
        }
        if (!is_allowed)
        {
            printf(err_string);
            return 1;
        }
        is_allowed = false;
    }
    buffer_length = describe_truncated_url(url, (void **)buff, offset);
    // print truncated URL
    if (debug)
    {
        // printf(&buff[offset]);
    }
    return buffer_length;
}

int main(void)
{
    bool debug = false;
    char *debug_mode = getenv("URL_PROCESSING_DEBUG");
    if (debug_mode == NULL)
    {
        debug_mode = "";
    }
    if (strcmp(debug_mode, "DEBUG") == 0)
    {
        debug = true;
    }
    char *url = NULL;
    size_t n;
    ssize_t status = __getline(&url, &n, stdin);
    if (status == -1)
    {
        fprintf(stderr, "getline failed\n");
        exit(1);
    }
    int result = process_url(url, status, debug);
    free(url);
    return result;
}
