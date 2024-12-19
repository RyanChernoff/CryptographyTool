#ifndef CAESAR_CYPHER_H
#define CAESAR_CYPHER_H

#include <stdbool.h>
#include "read_file.h"

void caesar_cypher_encrypt(char *input_file, char *output_file, char *key_file);
void caesar_cypher_decrypt(char *input_file, char *output_file, char *key_file);

#endif