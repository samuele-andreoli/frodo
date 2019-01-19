/*
   Copyright 2018 Samuele Andreoli

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef FRODO_DISTRIBUTION_H
#define FRODO_DISTRIBUTION_H

#include "amcl.h"

#include "frodo_config.h"

/* Inverse sample API */

/** @brief Generate FRODO_N * FRODO_BAR_N samples from the distribution.
 *
 * @param RNG, an instance of a csprng. It MUST be initialized
 * @param samples, array to store the generated samples. It MUST be zeroed
 * @param n_samples, number of samples to generate. n_samples must be even
 */
void FRODO_inverse_sample(csprng* RNG, uint16_t* samples, size_t n_samples);

#endif