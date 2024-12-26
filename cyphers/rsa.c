#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "read_file.h"
#include "safety.h"

// Start helper functions

/** @pre a and b must both be non-negative */
static long long gcd(long long a, long long b)
{
    if (b == 0)
        return a;

    return gcd(b, a % b);
}

/**
 * @brief Uses the euclidean algorithm to find the inverse of b % a
 *
 * @pre first call should have s = 0 and t = 1
 * @pre a and b must be coprime
 */
static long long compute_inverse(long long a, long long b, long long s, long long t)
{
    if (b == 0)
        return s;

    return compute_inverse(b, a % b, t, s - (a / b) * t);
}

/**
 * @brief returns the larges amount of bits that can only represent numbers less than n
 *
 * @pre n > 1
 */
static size_t get_block_size(long long n)
{
    size_t size = 0;
    long long test = 1;
    while (n >= test && test > 0) // need to ensure test doesn't overflow
    {
        size++;
        test = test << 1;
    }
    return size - 1;
}

static bool get_bit(char *str, unsigned long index)
//@requires strlen(str) >= index/8;
{
    return (str[index / 8] & (1 << (7 - (index % 8)))) != 0;
}

static void set_bit(char *str, unsigned long index, bool bit)
//@requires strlen(str) >= index/8;
{
    unsigned long mod = index % 8;
    index /= 8;
    if (bit)
        str[index] = str[index] | (1 << (7 - mod));
    else
        str[index] = str[index] & (0xFF - (1 << (7 - mod)));
}

/**
 * @brief computes b^e mod n
 * @pre n must be > 1
 * @pre e and b must be non-negative
 */
static long long mod_power(long long b, long long e, long long n)
{
    if (e == 0 || b == 0)
        return 0;

    long long total = 1;
    while (e > 0)
    {
        if (e & 1LL)
            total = (total * b) % n;
        b = (b * b) % n;
        e = e >> 1;
    }
    return total;
}

// End helper functions

void rsa_create_key(const char *public_key_file, const char *private_key_file)
{
    char buf[1024];
    long long p = 0;
    long long q = 0;

    while (true)
    {
        printf("Pick a positive prime number to use for p:\n");
        xfgets(buf, 1024, stdin);
        errno = 0;
        p = strtoll(buf, NULL, 0);
        if (errno == EINVAL)
        {
            printf("Not a valid number\n");
        }
        else if (errno == ERANGE)
        {
            printf("Your number to large\n");
        }
        else if (p <= 1)
        {
            printf("Your number is too small\n");
        }
        else
        {
            break;
        }
    }

    while (true)
    {
        printf("Pick a positive prime number to use for q:\n");
        xfgets(buf, 1024, stdin);
        errno = 0;
        q = strtoll(buf, NULL, 0);
        if (errno == EINVAL)
        {
            printf("Not a valid number\n");
        }
        else if (errno == ERANGE)
        {
            printf("Your number to large\n");
        }
        else if (q == p)
        {
            printf("q can't equal p\n");
        }
        else if (q <= 1 || q < 10 / p) // ensure n is large enough so an e will exist
        {
            printf("Your number is too small\n");
        }
        else if (p > LLONG_MAX / q)
        {
            printf("p*q is too big\n");
        }
        else
        {
            break;
        }
    }

    long long n = p * q;
    long long totient = (p - 1) * (q - 1) / gcd(p - 1, q - 1);
    long long e = 0;

    while (true)
    {
        printf("Pick a number between 1 and %llu exclusive that is coprime with %llu for e:\n", totient, totient);
        xfgets(buf, 1024, stdin);
        errno = 0;
        e = strtoll(buf, NULL, 0);
        if (errno == EINVAL)
        {
            printf("Not a valid number\n");
        }
        else if (errno == ERANGE || e >= totient)
        {
            printf("Your number to large\n");
        }
        else if (e <= 1)
        {
            printf("Your number must be greater than 1\n");
        }
        else if (gcd(e, totient) != 1)
        {
            printf("Your number must be coprime with %llu\n", totient);
        }
        else
        {
            break;
        }
    }

    long long d = compute_inverse(totient, e, 0, 1) % totient;
    if (d < 0)
    {
        d = totient + d;
    }

    FILE *public_key = xfopen(public_key_file, "w+");
    xfprintf(public_key, "%llu\n%llu", e, n);
    fclose(public_key);

    FILE *private_key = xfopen(private_key_file, "w+");
    xfprintf(private_key, "%llu\n%llu", d, n);
    fclose(private_key);
}

void rsa_encrypt(const char *input_file, const char *output_file, const char *key_file)
{
    char *key = read_file(key_file, NULL);
    errno = 0;
    char *end_ptr;
    long long e = strtoll(key, &end_ptr, 0);
    if (errno == EINVAL || errno == ERANGE || e <= 1 || end_ptr[0] != '\n')
    {
        printf("Invalid key\n");
        free(key);
        return;
    }

    errno = 0;
    long long n = strtoll(end_ptr, NULL, 0);
    if (errno == EINVAL || errno == ERANGE || n <= e || n < 10)
    {
        printf("Invalid key\n");
        free(key);
        return;
    }
    free(key);

    // Calculate padding size
    size_t len;
    char *input = read_file(input_file, &len);
    size_t block_size = get_block_size(n);
    size_t padding_size = 3;
    // since the encrypted text will add 1 bit per block there must be some multiple of 8 blocks
    while (((len + padding_size) * 8) % block_size || ((len + padding_size) * 8) / block_size % 8)
        padding_size++;

    // Create and add padding
    char *plain_text = xmalloc((padding_size + len) * sizeof(char));
    plain_text[0] = 0x0;
    plain_text[1] = 0x2;
    srand(time(NULL));
    for (size_t i = 2; i < padding_size - 1; i++)
        plain_text[i] = (char)((rand() % 255) + 1);
    plain_text[padding_size - 1] = '\0';
    memcpy(&plain_text[padding_size], input, len);
    free(input);

    size_t num_blocks = ((len + padding_size) * 8) / block_size;
    char *cypher = xmalloc((padding_size + len + (num_blocks / 8)) * sizeof(char));
    for (size_t block = 0; block < num_blocks; block++)
    {
        long long num = 0;
        for (size_t i = 0; i < block_size; i++)
        {
            if (get_bit(plain_text, block * block_size + i))
                num += (1 << (block_size - i - 1));
        }
        num = mod_power(num, e, n);
        for (size_t i = 0; i <= block_size; i++)
            set_bit(cypher, block * (block_size + 1) + i, num & (1LL << (block_size - i)));
    }
    free(plain_text);

    FILE *output = xfopen(output_file, "w+");
    fwrite(cypher, sizeof(char), padding_size + len + (num_blocks / 8), output);
    fclose(output);
    free(cypher);
}

void rsa_decrypt(const char *input_file, const char *output_file, const char *key_file)
{
    char *key = read_file(key_file, NULL);
    errno = 0;
    char *end_ptr;
    long long d = strtoll(key, &end_ptr, 0);
    if (errno == EINVAL || errno == ERANGE || d <= 1 || end_ptr[0] != '\n')
    {
        printf("Invalid key\n");
        free(key);
        return;
    }

    errno = 0;
    long long n = strtoll(end_ptr, NULL, 0);
    if (errno == EINVAL || errno == ERANGE || n <= d || n < 10)
    {
        printf("Invalid key\n");
        free(key);
        return;
    }
    free(key);

    size_t len;
    char *input = read_file(input_file, &len);
    size_t block_size = get_block_size(n);
    if ((len * 8) % (block_size + 1))
    {
        printf("Invalid input length\n");
        free(input);
        return;
    }
    size_t num_blocks = len * 8 / (block_size + 1);
    char *decrypt = xmalloc((block_size * num_blocks) / 8 * sizeof(char));
    for (size_t block = 0; block < num_blocks; block++)
    {
        long long num = 0;
        for (size_t i = 0; i <= block_size; i++)
        {
            if (get_bit(input, block * (block_size + 1) + i))
                num += (1 << (block_size - i));
        }
        num = mod_power(num, d, n);
        for (size_t i = 0; i < block_size; i++)
            set_bit(decrypt, block * block_size + i, num & (1LL << (block_size - i - 1)));
    }
    free(input);

    // Varify padding
    if (decrypt[0] || decrypt[1] != 0x2)
    {
        printf("Invalid input\n");
        free(decrypt);
        return;
    }

    // find start of content
    size_t padding_size = strlen(&decrypt[1]) + 2; // add two for starting and ending '\0'

    // write to output
    FILE *output = xfopen(output_file, "w+");
    fwrite(&decrypt[padding_size], sizeof(char), ((block_size * num_blocks) / 8) - padding_size, output);
    fclose(output);
    free(decrypt);
}