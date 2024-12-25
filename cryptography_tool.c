#include <unistd.h>
#include "shift_cypher.h"
#include "caesar_cypher.h"
#include "base_change.h"
#include "rsa.h"

const char *usage_msg = "Usage: ctool [options]\n\n-e tells the program you want to encrypt the input file\n-d tells the program you want to decrypt te input file\n-c tells the program that you want to create an encryption key. This causes the key file specified by -k to be the output file of the key and the output file specified by -o to be the public key if needed. If neither are specified than key.txt and public-key.txt will be created\n-i the name of the input file to encrypt/decrypt. This file must always be specified.\n-o the name of the file to output the encrypted/decrypted file. If this option is not specified the output will overwrite the input file\n-k the name of the file to write the key to if encryption is occuring or read the key from if decrytion is occuring. If encrypting this option is optional and if decrypting this option is necessary for all non-standard base changes\n-h prints this message\n";

// Start Create

void create_key(const char *output, const char *key)
{
    while (true)
    {
        char choice[4];
        printf("What algorithm would you like to use to generate your key:\n1 - rsa\nQ - Quit\n");
        xfgets(choice, 4, stdin);
        switch (choice[0])
        {
        case '1':
            rsa_create_key(output, key);
            return;
        case 'Q':
            return;
        }
        printf("Invalid choice\n");
    }
}

// Start Encrypt

void encrypt(const char *input_file, const char *output_file, const char *key_file)
{
    while (true)
    {
        char choice[4];
        printf("What encryption algorithm would you like to use:\n1 - Shift Cypher\n2 - Ceaser Cypher\n3 - To Binary\n4 - To Octal\n5 - To Hex\n6 - To Base32\n7 - To Base64\n8 - To Base\n9 - RSA\nQ - Quit\n");
        xfgets(choice, 4, stdin);
        switch (choice[0])
        {
        case '1':
            shift_cypher_encrypt(input_file, output_file, key_file);
            return;
        case '2':
            caesar_cypher_encrypt(input_file, output_file, key_file);
            return;
        case '3':
            binary_encrypt(input_file, output_file, key_file);
            return;
        case '4':
            octal_encrypt(input_file, output_file, key_file);
            return;
        case '5':
            hex_encrypt(input_file, output_file, key_file);
            return;
        case '6':
            base32_encrypt(input_file, output_file, key_file);
            return;
        case '7':
            base64_encrypt(input_file, output_file, key_file);
            return;
        case '8':
            base_encrypt(input_file, output_file, key_file);
            return;
        case '9':
            rsa_encrypt(input_file, output_file, key_file);
            return;
        case 'Q':
            return;
        }
        printf("Invalid choice\n");
    }
}

// End Encrypt

// Begin Decrypt

void decrypt(const char *input_file, const char *output_file, const char *key_file)
{
    while (true)
    {
        char choice[4];
        printf("What decryption algorithm would you like to use:\n1 - Shift Cypher\n2 - Ceaser Cypher\n3 - From Binary\n4 - From Octal\n5 - From Hex\n6 - From Base32\n7 - From Base64\n8 - From Base\nQ - Quit\n");
        xfgets(choice, 4, stdin);
        switch (choice[0])
        {
        case '1':
            if (key_file == NULL)
            {
                printf("Can't decrypt a shift cyper without a key\n");
                break;
            }
            shift_cypher_decrypt(input_file, output_file, key_file);
            return;
        case '2':
            if (key_file == NULL)
            {
                printf("Can't decrypt a caesar cyper without a key\n");
                break;
            }
            caesar_cypher_decrypt(input_file, output_file, key_file);
            return;
        case '3':
            binary_decrypt(input_file, output_file);
            return;
        case '4':
            octal_decrypt(input_file, output_file);
            return;
        case '5':
            hex_decrypt(input_file, output_file);
            return;
        case '6':
            base32_decrypt(input_file, output_file);
            return;
        case '7':
            base64_decrypt(input_file, output_file);
            return;
        case '8':
            if (key_file == NULL)
            {
                printf("Can't decrypt from an arbitrary base without a key\n");
                break;
            }
            base_decrypt(input_file, output_file, key_file);
            return;
        case 'Q':
            return;
        }
        printf("Invalid choice\n");
    }
}

// End Decrypt

int main(int argc, char *argv[])
{
    setbuf(stdin, NULL);
    bool encrypt_flag = true;
    bool create_flag = false;
    const char *input = NULL;
    const char *output = NULL;
    const char *key = NULL;
    for (int opt = getopt(argc, argv, "hedci:o:k:"); opt != -1; opt = getopt(argc, argv, "hedi:o:k:"))
    {
        switch (opt)
        {
        case 'e':
            encrypt_flag = true;
            break;
        case 'd':
            encrypt_flag = false;
            break;
        case 'c':
            create_flag = true;
            break;
        case 'i':
            input = optarg;
            break;
        case 'o':
            output = optarg;
            break;
        case 'k':
            key = optarg;
            break;
        default:
            printf(usage_msg);
            exit(1);
        }
    }

    if (key == NULL)
    {
        key = "key.txt";
    }

    if (create_flag)
    {
        if (output == NULL)
        {
            output = "public-key.txt";
        }
        create_key(output, key);
        return 0;
    }

    if (input == NULL)
    {
        printf(usage_msg);
        exit(1);
    }

    if (output == NULL)
    {
        output = input;
    }

    if (encrypt_flag)
    {
        encrypt(input, output, key);
    }
    else
    {
        decrypt(input, output, key);
    }
    return 0;
}