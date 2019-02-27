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

#ifndef FRODO_DISTRIBUTION_H
#define FRODO_DISTRIBUTION_H

#include "frodo_config.h"
#include "backend.h"

/* Inverse sample API */

/** @brief Generate FRODO_N * FRODO_BAR_N samples from the distribution.
 *
 * @param RNG, an instance of a csprng. It MUST be initialized
 * @param samples, array to store the generated samples. It MUST be zeroed
 * @param n_samples, number of samples to generate. n_samples must be even
 */
void FRODO_inverse_sample(FRODO_CSPRNG* RNG, uint16_t* samples, size_t n_samples);

#endif
