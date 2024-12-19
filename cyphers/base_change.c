#include "dict.h"
#include "read_file.h"

// Start Base Change Dictionaries

unsigned int char_hash_fn(key k)
{
    return (unsigned int)*((char *)k);
}

bool char_equiv_fn(key k1, key k2)
{
    return *((char *)k1) == *((char *)k2);
}

dict_t make_dict(char *str, bool rev, char padding)
//@requires 2 <= strlen(str) && strlen(str) <= 256;
{
    unsigned int len = strlen(str);
    dict_t map = dict_new(len, &char_hash_fn, &char_equiv_fn, &free, &free);

    for (char i = 0; ((unsigned int)i) < len; i++)
    {
        char *k = xmalloc(sizeof(char));
        char *e = xmalloc(sizeof(char));
        *k = i;
        *e = str[i];
        if (!rev)
            dict_add(map, (key)k, (entry)e);
        else
            dict_add(map, (key)e, (entry)k);
    }

    if (rev)
    {
        char *k = xmalloc(sizeof(char));
        char *e = xmalloc(sizeof(char));
        *k = padding;
        *e = 0;
        dict_add(map, (key)k, (entry)e);
    }
    return map;
}

// End Base Change Dictionaries

// Start Bit Reading Functions

bool get_bit(char *str, unsigned long index)
//@requires strlen(str) >= index/8;
{
    return (str[index / 8] & (1 << (7 - (index % 8)))) != 0;
}

char get_bits(char *str, unsigned long index, size_t size)
//@requires size > 0;
//@requires strlen(str) >= (index*size + size - 1)/8;
{
    index *= size;
    char c = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (get_bit(str, index + i))
            c += (1 << (size - i - 1));
    }
    return c;
}

void set_bit(char *str, unsigned long index, bool bit)
//@requires strlen(str) >= index/8;
{
    unsigned long mod = index % 8;
    index /= 8;
    if (bit)
        str[index] = str[index] | (1 << (7 - mod));
    else
        str[index] = str[index] & (0xFF - (1 << (7 - mod)));
}

void set_bits(char *str, unsigned long index, char c, size_t size)
//@requires 0 < size && size <= 8;
//@requires strlen(str) >= (index*size + size - 1)/8;
{
    index *= size;
    for (unsigned long i = 0; i < size; i++)
    {
        char bit = c & (1 << (size - i - 1));
        set_bit(str, index + i, bit != 0);
    }
}

// End Bit Reading Functions

// Start Encryptions

void change_base_encrypt(char *input_file, char *output_file, char *key_file, size_t size, dict_t map, char padding)
//@requires size > 0;
{
    // Gets input to length divisible by size
    char *input = read_file(input_file);
    unsigned long len = strlen(input) * 8;
    while (len % size != 0)
    {
        len++;
        input = xrealloc(input, len);
    }
    len /= size;

    // Calculates buffer
    char *buf = xcalloc(8, sizeof(char));
    unsigned long buf_size = 0;
    while ((len + buf_size) * size % 8 != 0)
    {
        buf[buf_size] = padding;
        buf_size++;
    }

    // Calculates cyphertext
    char *cyphertext = xmalloc((len + 8) * sizeof(char));
    cyphertext[len] = '\0';

    for (unsigned long i = 0; i < len; i++)
    {
        char *k = xmalloc(sizeof(char));
        *k = get_bits(input, i, size);
        char *e = (char *)dict_get(map, (key)k);
        free(k);
        cyphertext[i] = *e;
    }
    strcat(cyphertext, buf);
    free(buf);
    free(input);
    dict_free(map);

    FILE *output = xfopen(output_file, "w");
    xfprintf(output, "%s", cyphertext);
    fclose(output);
    free(cyphertext);

    FILE *key = xfopen(key_file, "w");
    xfprintf(key, "%lu", size);
    fclose(key);
}

// Start Encryptions

void binary_encrypt(char *input_file, char *output_file, char *key_file)
{
    dict_t map = make_dict("01", false, '=');
    change_base_encrypt(input_file, output_file, key_file, 1, map, '=');
}

void octal_encrypt(char *input_file, char *output_file, char *key_file)
{
    dict_t map = make_dict("01234567", false, '=');

    char *input = read_file(input_file);
    unsigned long len = strlen(input);

    char *oct = xmalloc(len * 4 * sizeof(char));
    for (unsigned long i = 0; i < len; i++)
    {
        char *k = xmalloc(sizeof(char));
        *k = ((input[i] & 0x80) + (input[i] & 0x40)) >> 6;
        char *e = (char *)dict_get(map, (key)k);
        oct[i * 4] = *e;

        *k = ((input[i] & 0x20) + (input[i] & 0x10) + (input[i] & 0x08)) >> 3;
        e = (char *)dict_get(map, (key)k);
        oct[i * 4 + 1] = *e;

        *k = (input[i] & 0x04) + (input[i] & 0x02) + (input[i] & 0x01);
        e = (char *)dict_get(map, (key)k);
        free(k);
        oct[i * 4 + 2] = *e;

        oct[i * 4 + 3] = ' ';
    }
    oct[len * 4 - 1] = '\0';
    free(input);
    dict_free(map);

    FILE *output = xfopen(output_file, "w");
    xfprintf(output, "%s", oct);
    fclose(output);
    free(oct);

    FILE *key = xfopen(key_file, "w");
    xfprintf(key, "%d", 8);
    fclose(key);
}

void hex_encrypt(char *input_file, char *output_file, char *key_file)
{
    dict_t map = make_dict("0123456789ABCDEF", false, '=');
    change_base_encrypt(input_file, output_file, key_file, 4, map, '=');
}

void base32_encrypt(char *input_file, char *output_file, char *key_file)
{
    dict_t map = make_dict("ABCDEFGHIJKLMNOPQRSTUVWXYZ234567", false, '=');
    change_base_encrypt(input_file, output_file, key_file, 5, map, '=');
}

void base64_encrypt(char *input_file, char *output_file, char *key_file)
{
    dict_t map = make_dict("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/", false, '=');
    change_base_encrypt(input_file, output_file, key_file, 6, map, '=');
}

void base_encrypt(char *input_file, char *output_file, char *key_file)
{
    char base = 0;
    char buf[259];
    while (true)
    {
        printf("Choose the number of bits to use for the base (1 - 8):\n");
        xfgets(buf, 4, stdin);
        base = buf[0];
        if (base >= '1' && base <= '8')
        {
            base -= '0';
            break;
        }
        printf("Invalid size\n");
    }

    dict_t map;
    dict_t map_rev;
    while (true)
    {
        unsigned int len = ((unsigned int)0x0001) << base;
        printf("Choose an alpahabet of %d characters for mapping:\n", len);
        xfgets(buf, len + 3, stdin);
        if (strlen(buf) != len + 1)
        {
            printf("Invalid length\n");
            continue;
        }
        buf[len] = '\0';

        map_rev = make_dict(buf, true, buf[0]);
        if (dict_size(map_rev) == len)
        {
            map = make_dict(buf, false, '=');
            break;
        }
        dict_free(map_rev);
        printf("Alphabet contains repeats\n");
    }

    char padding;
    while (true)
    {
        char padbuf[4];
        printf("Choose a character to be used as padding (can't be in your alphabet):\n");
        xfgets(padbuf, 4, stdin);
        if (padbuf[1] == '\0')
        {
            printf("Invalid character\n");
            continue;
        }
        padding = padbuf[0];
        if (dict_get(map_rev, (key)&padding) == NULL)
            break;
        printf("Charactor already in alphabet\n");
    }
    dict_free(map_rev);

    change_base_encrypt(input_file, output_file, key_file, (size_t)base, map, padding);

    FILE *key = xfopen(key_file, "a");
    xfprintf(key, "%s", buf);
    xfprintf(key, "%c", padding);
    fclose(key);
}

// End Encryptions

// Start Decryptions

void change_base_decrypt(char *input_file, char *output_file, size_t size, dict_t map)
//@requires size > 0;
{
    char *input = read_file(input_file);
    unsigned long len = strlen(input);
    if (len * size % 8 != 0)
    {
        free(input);
        dict_free(map);
        printf("Input has invalid length\n");
        exit(1);
    }

    unsigned long max = len * size / 8;
    char *message = xmalloc((max + 1) * sizeof(char));
    message[max] = '\0';
    for (unsigned long i = 0; i < len; i++)
    {
        char *k = xmalloc(sizeof(char));
        *k = input[i];
        char *e = (char *)dict_get(map, (key)k);
        free(k);
        if (e == NULL)
        {
            free(input);
            dict_free(map);
            printf("Input contains invalid characters\n");
            exit(1);
        }
        set_bits(message, i, *e, size);
    }
    free(input);
    dict_free(map);

    FILE *output = xfopen(output_file, "w");
    xfprintf(output, "%s", message);
    fclose(output);
    free(message);
}

void binary_decrypt(char *input_file, char *output_file, char *key_file)
{
    dict_t map = make_dict("01", true, '=');
    change_base_decrypt(input_file, output_file, 1, map);
}

void octal_decrypt(char *input_file, char *output_file, char *key_file)
{
    char *input = read_file(input_file);
    unsigned long len = strlen(input);
    if (len % 4 != 3)
    {
        free(input);
        printf("Input has invalid length\n");
        exit(1);
    }

    dict_t map = make_dict("01234567", true, '=');

    unsigned long max = (len + 1) / 4;
    char *message = xmalloc((max + 1) * sizeof(char));
    message[max] = '\0';
    for (unsigned long i = 0; i < len; i += 4)
    {
        char *k = xmalloc(sizeof(char));
        *k = input[i];
        char *e = (char *)dict_get(map, (key)k);
        if (e == NULL)
        {
            free(input);
            free(k);
            dict_free(map);
            printf("Input contains invalid characters\n");
            exit(1);
        }
        message[i / 4] = *e << 6;

        *k = input[i + 1];
        e = (char *)dict_get(map, (key)k);
        if (e == NULL)
        {
            free(input);
            free(k);
            dict_free(map);
            printf("Input contains invalid characters\n");
            exit(1);
        }
        message[i / 4] += (*e & 0x07) << 3;

        *k = input[i + 2];
        e = (char *)dict_get(map, (key)k);
        free(k);
        if (e == NULL)
        {
            free(input);
            dict_free(map);
            printf("Input contains invalid characters\n");
            exit(1);
        }
        message[i / 4] += *e & 0x07;
    }
    free(input);
    dict_free(map);

    FILE *output = xfopen(output_file, "w");
    xfprintf(output, "%s", message);
    fclose(output);
    free(message);
}

void hex_decrypt(char *input_file, char *output_file, char *key_file)
{
    dict_t map = make_dict("0123456789ABCDEF", true, '=');
    change_base_decrypt(input_file, output_file, 4, map);
}

void base32_decrypt(char *input_file, char *output_file, char *key_file)
{
    dict_t map = make_dict("ABCDEFGHIJKLMNOPQRSTUVWXYZ234567", true, '=');
    change_base_decrypt(input_file, output_file, 5, map);
}

void base64_decrypt(char *input_file, char *output_file, char *key_file)
{
    dict_t map = make_dict("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/", true, '=');
    change_base_decrypt(input_file, output_file, 6, map);
}

void base_decrypt(char *input_file, char *output_file, char *key_file)
{
    char *key = read_file(key_file);
    if (key[0] < '1' || key[0] > '8')
    {
        printf("Invalid key\n");
        free(key);
        return;
    }

    size_t size = key[0] - '0';
    unsigned int len = ((unsigned int)0x0001) << size;
    if (strlen(key) != len + 2)
    {
        printf("Invalid key\n");
        free(key);
        return;
    }

    dict_t map = make_dict(&key[1], true, key[len + 1]);
    if (dict_size(map) != len + 1)
    {
        printf("Invalid key\n");
        dict_free(map);
        free(key);
        return;
    }
    free(key);

    change_base_decrypt(input_file, output_file, size, map);
}

// End Decryptions