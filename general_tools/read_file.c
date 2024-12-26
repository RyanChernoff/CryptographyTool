#include <string.h>
#include "safety.h"

char *read_file(const char *file_name, size_t *len_loc)
{
    FILE *file = xfopen(file_name, "r");

    char *str = xmalloc(sizeof(char));
    char buffer[1024];
    size_t len = 0;
    size_t size = fread(buffer, sizeof(char), 1024, file);

    *str = '\0';

    while (size)
    {
        str = xrealloc(str, len + size + 1);
        memcpy(&str[len], buffer, size);
        len += size;
        str[len] = '\0';
        size = fread(buffer, sizeof(char), 1024, file);
    }
    fclose(file);

    if (len == 0)
    {
        printf("Trying to read empty file\n");
        exit(1);
    }
    else if (errno)
    {
        printf("Error: %s\n", strerror(errno));
        exit(1);
    }

    if (len_loc)
        *len_loc = len;
    return str;
}