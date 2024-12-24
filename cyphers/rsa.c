#include <stdbool.h>
#include "read_file.h"

/** @pre a and b must both be non-negative */
long long gcd(long long a, long long b)
{
    if (b == 0)
        return a;

    return gcd(b, a % b);
}

/**
 * @brief Uses the euclidean algorithm to find the inverse of b % a
 *
 * @pre first call should have s = 0 and t = 1
 */
long long compute_inverse(long long a, long long b, long long s, long long t)
{
    if (b == 0)
        return s;

    return compute_inverse(b, a % b, t, s - (a / b) * t);
}

void rsa_create_key(const char *public_key_file, const char *private_key_file)
{
    char buf[1024];
    long long p = 0;
    long long q = 0;

    while (true)
    {
        printf("Pick a positive prime number to use for p:\n");
        xfgets(buf, 1024, stdin);
        p = strtoll(buf, NULL, 0);
        if (errno == EINVAL)
        {
            printf("Not a valid number\n");
        }
        else if (errno == ERANGE)
        {
            printf("Your number to large\n");
        }
        else if (p <= 0)
        {
            printf("Your number must be positive\n");
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
        q = strtoll(buf, NULL, 0);
        if (errno == EINVAL)
        {
            printf("Not a valid number\n");
        }
        else if (errno == ERANGE)
        {
            printf("Your number to large\n");
        }
        else if (q <= 0)
        {
            printf("Your number must be positive\n");
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
    xfprintf(private_key, "%llu", d);
    fclose(private_key);
}