#ifndef BASE_CHANGE_H
#define BASE_CHANGE_H

#include "dict.h"
#include "read_file.h"

void binary_encrypt(char *input_file, char *output_file, char *key_file);
void octal_encrypt(char *input_file, char *output_file, char *key_file);
void hex_encrypt(char *input_file, char *output_file, char *key_file);
void base32_encrypt(char *input_file, char *output_file, char *key_file);
void base64_encrypt(char *input_file, char *output_file, char *key_file);
void base_encrypt(char *input_file, char *output_file, char *key_file);

void binary_decrypt(char *input_file, char *output_file, char *key_file);
void octal_decrypt(char *input_file, char *output_file, char *key_file);
void hex_decrypt(char *input_file, char *output_file, char *key_file);
void base32_decrypt(char *input_file, char *output_file, char *key_file);
void base64_decrypt(char *input_file, char *output_file, char *key_file);
void base_decrypt(char *input_file, char *output_file, char *key_file);

#endif