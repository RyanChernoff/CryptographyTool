#ifndef CAESAR_CYPHER_H
#define CAESAR_CYPHER_H

#include <stdbool.h>
#include "read_file.h"

void caesar_cypher_encrypt(const char *input_file, const char *output_file, const char *key_file);
void caesar_cypher_decrypt(const char *input_file, const char *output_file, const char *key_file);

#endif