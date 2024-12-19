#include "read_file.h"

void shift_cypher_encrypt(char *input_file, char *output_file, char *key_file)
{
    char *input = read_file(input_file);
    char choice[5];
    int shift = 0;

    while (shift == 0)
    {
        printf("How far should the cypher shift?\n");
        xfgets(choice, 4, stdin);
        choice[2] = '\0';
        shift = atoi(choice);

        if (shift <= 0 || shift >= 26)
        {
            printf("Invalid Shift: must be between 1 and 25\n");
            shift = 0;
        }
    }

    size_t len = strlen(input);
    for (size_t i = 0; i < len; i++)
    {
        char c = input[i];
        if (c >= 'a' && c <= 'z')
        {
            c = (c - 'a' + shift) % 26 + 'a';
        }
        else if (c >= 'A' && c <= 'Z')
        {
            c = (c - 'A' + shift) % 26 + 'A';
        }
        input[i] = c;
    }

    FILE *output = xfopen(output_file, "w");
    xfprintf(output, "%s", input);
    free(input);
    fclose(output);

    FILE *key = xfopen(key_file, "w");
    xfprintf(key, "%d", shift);
    fclose(key);
}

void shift_cypher_decrypt(char *input_file, char *output_file, char *key_file)
{
    char *input = read_file(input_file);
    char *key = read_file(key_file);

    int shift = 26 - atoi(key);
    free(key);
    if (shift <= 0 || shift >= 26)
    {
        printf("Invalid Key\n");
        free(input);
        return;
    }

    size_t len = strlen(input);
    for (size_t i = 0; i < len; i++)
    {
        char c = input[i];
        if (c >= 'a' && c <= 'z')
        {
            c = (c - 'a' + shift) % 26 + 'a';
        }
        else if (c >= 'A' && c <= 'Z')
        {
            c = (c - 'A' + shift) % 26 + 'A';
        }
        input[i] = c;
    }

    FILE *output = xfopen(output_file, "w");
    xfprintf(output, "%s", input);
    free(input);
    fclose(output);
}