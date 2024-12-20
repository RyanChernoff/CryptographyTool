#ifndef BASE_CHANGE_H
#define BASE_CHANGE_H

#include "dict.h"
#include "read_file.h"

void binary_encrypt(const char *input_file, const char *output_file, const char *key_file);
void octal_encrypt(const char *input_file, const char *output_file, const char *key_file);
void hex_encrypt(const char *input_file, const char *output_file, const char *key_file);
void base32_encrypt(const char *input_file, const char *output_file, const char *key_file);
void base64_encrypt(const char *input_file, const char *output_file, const char *key_file);
void base_encrypt(const char *input_file, const char *output_file, const char *key_file);

void binary_decrypt(const char *input_file, const char *output_file);
void octal_decrypt(const char *input_file, const char *output_file);
void hex_decrypt(const char *input_file, const char *output_file);
void base32_decrypt(const char *input_file, const char *output_file);
void base64_decrypt(const char *input_file, const char *output_file);
void base_decrypt(const char *input_file, const char *output_file, const char *key_file);

#endif