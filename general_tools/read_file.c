#include <string.h>
#include "safety.h"

char *read_file(char *file_name)
{
    FILE *file = xfopen(file_name, "r");

    char *str = xmalloc(sizeof(char));
    char buffer[1024];
    size_t len = 0;
    size_t size = fread(buffer, 1, 1023, file);

    *str = '\0';

    while (size)
    {
        len += size;
        buffer[size] = '\0';
        str = xrealloc(str, len + 2);
        strcat(str, buffer);
        size = fread(buffer, 1, 1023, file);
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

    return str;
}