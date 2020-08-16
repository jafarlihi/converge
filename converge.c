#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <gmp.h>

void printUsage(char *programName) {
    fprintf(stderr, "usage: %s <integer bigger than 3 and smaller than %lu>\n", programName, ULONG_MAX);
    exit(1);
}

typedef struct {
    int step_count;
    char *sequence_sum;
} Result;

bool isPositiveNumber(char *input);
Result converge(mpz_t integer, int step, mpz_t sum);

int main(int argc, char *argv[]) {
    if (argc != 2 || !isPositiveNumber(argv[1]))
        printUsage(argv[0]);
    unsigned long parsedLong = strtol(argv[1], 0, 10);
    if (parsedLong <= 3 || errno == ERANGE)
        printUsage(argv[0]);

    Result longest = {.step_count = 0, .sequence_sum = NULL};
    Result secondLongest = {.step_count = 0, .sequence_sum = NULL};
    for (unsigned long c = 1; c <= parsedLong; c++) {
        mpz_t sum, integer;
        mpz_init(sum);
        mpz_init(integer);
        mpz_set_ui(sum, 0);
        mpz_set_ui(integer, c);

        Result result = converge(integer, 0, sum);

        mpz_clear(sum);
        mpz_clear(integer);

        if (result.step_count > longest.step_count)
            longest = result;
        else if (result.step_count > secondLongest.step_count)
            secondLongest = result;
    }

    printf("%s\n", secondLongest.sequence_sum);
    return 0;
}

bool isPositiveNumber(char *input) {
    for (int i = 0; input[i] != '\0'; i++)
        if (!isdigit(input[i]))
            return false;
    return true;
}

Result converge(mpz_t integer, int step, mpz_t sum) {
    mpz_add(sum, sum, integer);
    if (mpz_cmp_ui(integer, 1) == 0)
        return (Result) {.step_count = ++step, .sequence_sum = mpz_get_str(NULL, 10, sum)};
    else if (mpz_even_p(integer)) {
        mpz_cdiv_q_ui(integer, integer, 2);
        return converge(integer, ++step, sum);
    } else {
        mpz_mul_ui(integer, integer, 3);
        mpz_add_ui(integer, integer, 1);
        return converge(integer, ++step, sum);
    }
}

