#ifndef SAFETY_H
#define SAFETY_H

#include <stdio.h>
#include <stdlib.h>

char *xfgets(char *str, int n, FILE *stream);
int xfprintf(FILE *stream, const char *format, ...);
void *xmalloc(size_t size);
void *xcalloc(size_t nmem, size_t size);
void *xrealloc(void *ptr, size_t size);
FILE *xfopen(const char *filename, char *mode);

#endif