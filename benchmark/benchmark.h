/*
 * ISC License
 *
 * Copyright (c) 2019
 * Samuele Andreoli <samuele dot andreoli at studenti dot unitn dot it>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <time.h>
#include "frodo.h"

float start, total;

#define BENCHTEST(name, func, iterations) \
    printf("------------------------------------------------------------\n"); \
    printf("Benchmark %s\n", name); \
    total = 0;\
    for(int i = 0; i < iterations; i++) \
    { \
        start = (float)clock(); \
        func; \
        total += (float)clock() - start;\
    } \
    printf("Time elapsed %f\n", total/CLOCKS_PER_SEC); \
    printf("Iterations %d\n", iterations); \
    printf("Time per iteration %lfms\n", total * 1000 / (CLOCKS_PER_SEC * iterations)); \
    printf("------------------------------------------------------------\n");
