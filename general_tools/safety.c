#include <stdio.h>
#include <stdlib.h>

char *xfgets(char *str, int n, FILE *stream)
{
    char *ret = fgets(str, n, stream);
    if (ret == NULL)
    {
        printf("Error reading from file\n");
        exit(1);
    }
    return ret;
}

int xfprintf(FILE *stream, const char *format, va_list args)
{
    int ret = fprintf(stream, format, args);
    if (ret < 0)
    {
        printf("Error writing to file\n");
        exit(1);
    }
    return ret;
}

FILE *xfopen(const char *filename, char *mode)
{
    FILE *ret = fopen(filename, mode);
    if (ret == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }
    return ret;
}

void *xmalloc(size_t size)
{
    void *ret = malloc(size);
    if (ret == NULL)
    {
        printf("Out of memory\n");
        exit(1);
    }
    return ret;
}

void *xcalloc(size_t nmemb, size_t size)
{
    void *ret = calloc(nmemb, size);
    if (ret == NULL)
    {
        printf("Out of memory\n");
        exit(1);
    }
    return ret;
}

void *xrealloc(void *ptr, size_t size)
{
    void *ret = realloc(ptr, size);
    if (ret == NULL)
    {
        printf("Out of memory\n");
        exit(1);
    }
    return ret;
}