#include "shift_cypher.h"
#include "caesar_cypher.h"
#include "base_change.h"

typedef void encrypt_fn(char *input_file, char *output_file, char *key_file);
typedef void decrypt_fn(char *input_file, char *output_file, char *key_file);

unsigned int str_hash_fn(key k)
{
    char *str = (char *)k;
    unsigned long len = strlen(str);
    unsigned int hash = 0;
    for (unsigned long i = 0; i < len; i++)
    {
        hash = hash * 31 + ((unsigned int)str[i]);
    }
    return hash;
}

bool str_equiv_fn(key k1, key k2)
{
    char *str1 = (char *)k1;
    char *str2 = (char *)k2;
    return strcmp(str1, str2) == 0;
}

// Begin Encrypt

dict_t make_dict_encrypt()
{
    dict_t D = dict_new(10, &str_hash_fn, &str_equiv_fn, NULL, NULL);
    dict_add(D, (key) "1", &shift_cypher_encrypt);
    dict_add(D, (key) "2", &caesar_cypher_encrypt);
    dict_add(D, (key) "3", &binary_encrypt);
    dict_add(D, (key) "4", &octal_encrypt);
    dict_add(D, (key) "5", &hex_encrypt);
    dict_add(D, (key) "6", &base32_encrypt);
    dict_add(D, (key) "7", &base64_encrypt);
    dict_add(D, (key) "8", &base_encrypt);
    return D;
}

void encrypt(char *input_file, char *output_file, char *key_file)
{
    dict_t D = make_dict_encrypt();
    while (true)
    {
        char choice[4];
        printf("What encryption algorithm would you like to use:\n1 - Shift Cypher\n2 - Ceaser Cypher\n3 - To Binary\n4 - To Octal\n5 - To Hex\n6 - To Base32\n7 - To Base64\n8 - To Base\nQ - Quit\n");
        xfgets(choice, 4, stdin);
        choice[strlen(choice) - 1] = '\0';

        encrypt_fn *F = (encrypt_fn *)dict_get(D, (key)choice);
        if (F != NULL)
        {
            (*F)(input_file, output_file, key_file);
            break;
        }
        if (strcmp(choice, "Q") == 0)
            break;

        printf("Invalid choice\n");
    }
    dict_free(D);
}

// End Encrypt

// Begin Decrypt

dict_t make_dict_decrypt()
{
    dict_t D = dict_new(10, &str_hash_fn, &str_equiv_fn, NULL, NULL);
    dict_add(D, (key) "1", &shift_cypher_decrypt);
    dict_add(D, (key) "2", &caesar_cypher_decrypt);
    dict_add(D, (key) "3", &binary_decrypt);
    dict_add(D, (key) "4", &octal_decrypt);
    dict_add(D, (key) "5", &hex_decrypt);
    dict_add(D, (key) "6", &base32_decrypt);
    dict_add(D, (key) "7", &base64_decrypt);
    dict_add(D, (key) "8", &base_decrypt);
    return D;
}

void decrypt(char *input_file, char *output_file, char *key_file)
{
    dict_t D = make_dict_decrypt();
    while (true)
    {
        char choice[4];
        printf("What decryption algorithm would you like to use:\n1 - Shift Cypher\n2 - Ceaser Cypher\n3 - From Binary\n4 - From Octal\n5 - From Hex\n6 - From Base32\n7 - From Base64\n8 - From Base\nQ - Quit\n");
        xfgets(choice, 4, stdin);
        choice[strlen(choice) - 1] = '\0';

        decrypt_fn *F = (decrypt_fn *)dict_get(D, (key)choice);
        if (F != NULL)
        {
            (*F)(input_file, output_file, key_file);
            break;
        }
        if (strcmp(choice, "Q") == 0)
            break;

        printf("Invalid choice\n");
    }
    dict_free(D);
}

// End Decrypt

int main(int argc, char *argv[])
{
    setbuf(stdin, NULL);
    if (argc != 5)
    {
        printf("Usage: %s [-e/-d] <input_file> <output_file> <key_file>\n", argv[0]);
    }
    else if (strcmp(argv[1], "-e") == 0)
    {
        encrypt(argv[2], argv[3], argv[4]);
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        decrypt(argv[2], argv[3], argv[4]);
    }
    else
    {
        printf("Usage: %s [-e/-d] <input_file> <output_file> <key_file>\n", argv[0]);
    }
    return 0;
}