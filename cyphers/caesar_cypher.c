#include <stdbool.h>
#include "read_file.h"

static bool is_alphabet(char *map)
//@requires strlen(map) == 27 || strlen(map) == 26;
//@ensures strlen(map) == 26;
{
    if (strlen(map) == 27)
        map[26] = map[27];
    bool *tracker = xcalloc(26, sizeof(bool));
    for (int i = 0; i < 26; i++)
    {
        if (map[i] >= 'A' && map[i] <= 'Z')
        {
            map[i] = map[i] - 'A' + 'a';
        }

        if (map[i] >= 'a' && map[i] <= 'z')
        {
            map[i] -= 'a';
            if (tracker[(int)map[i]])
            {
                free(tracker);
                return false;
            }
            tracker[(int)map[i]] = true;
        }
        else
        {
            free(tracker);
            return false;
        }
    }
    free(tracker);
    return true;
}

static long get_index(char *str, char c, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        if (str[i] == c)
            return i;
    }
    return -1;
}

void caesar_cypher_encrypt(const char *input_file, const char *output_file, const char *key_file)
{
    char *input = read_file(input_file);
    char *map = xcalloc(28, sizeof(char));

    while (true)
    {
        printf("Please input a permutation of the alphabet.\n");
        xfgets(map, 28, stdin);
        if (strlen(map) == 27 && is_alphabet(map))
            break;
        printf("Invalid Input.\n");
    }

    size_t len = strlen(input);
    for (size_t i = 0; i < len; i++)
    {
        if (input[i] >= 'a' && input[i] <= 'z')
        {
            input[i] = map[input[i] - 'a'] + 'a';
        }
        else if (input[i] >= 'A' && input[i] <= 'Z')
        {
            input[i] = map[input[i] - 'A'] + 'A';
        }
    }

    FILE *key = xfopen(key_file, "w+");
    for (int i = 0; i < 26; i++)
        map[i] += 'a';
    xfprintf(key, "%s", map);
    free(map);
    fclose(key);

    FILE *output = xfopen(output_file, "w+");
    xfprintf(output, "%s", input);
    fclose(output);
    free(input);
}

void caesar_cypher_decrypt(const char *input_file, const char *output_file, const char *key_file)
{
    char *map = read_file(key_file);
    if (strlen(map) != 26 || !is_alphabet(map))
    {
        printf("Invalid Key\n");
        free(map);
        return;
    }

    char *cypher = read_file(input_file);
    size_t len = strlen(cypher);
    for (size_t i = 0; i < len; i++)
    {
        if (cypher[i] >= 'a' && cypher[i] <= 'z')
        {
            char c = (char)get_index(map, cypher[i] - 'a', 26) + 'a';
            cypher[i] = c;
        }
        else if (cypher[i] >= 'A' && cypher[i] <= 'Z')
        {
            char c = (char)get_index(map, cypher[i] - 'A', 26) + 'A';
            cypher[i] = c;
        }
    }

    free(map);

    FILE *output = xfopen(output_file, "w+");
    xfprintf(output, "%s", cypher);
    free(cypher);
    fclose(output);
}