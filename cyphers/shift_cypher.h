#ifndef SHIFT_CYPHER_H
#define SHIFT_CYPHER_H

#include "read_file.h"

void shift_cypher_encrypt(char *input_file, char *output_file, char *key_file);
void shift_cypher_decrypt(char *input_file, char *output_file, char *key_file);

#endif