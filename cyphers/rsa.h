#ifndef RSA_H
#define RSA_H

#include <stdbool.h>
#include "read_file.h"

void rsa_create_key(const char *public_key_file, const char *private_key_file);
void rsa_encrypt(const char *input_file, const char *output_file, const char *key_file);

#endif