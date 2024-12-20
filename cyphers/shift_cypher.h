#ifndef SHIFT_CYPHER_H
#define SHIFT_CYPHER_H

#include "read_file.h"

void shift_cypher_encrypt(const char *input_file, const char *output_file, const char *key_file);
void shift_cypher_decrypt(const char *input_file, const char *output_file, const char *key_file);

#endif