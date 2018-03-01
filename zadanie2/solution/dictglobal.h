/* Autorzy rozwiązania : Michał Radecki, Wojciech Ciszewski */

#ifndef DICT_GLOBAL_H_
#define DICT_GLOBAL_H_

#include <stddef.h>

#ifdef __cplusplus
#include <iostream>

namespace jnp1 {
extern "C" {
#endif

static const size_t MAX_GLOBAL_DICT_SIZE = 42;
unsigned long dict_global();

#ifdef __cplusplus
}
}
#endif

#endif /* DICT_GLOBAL_H_ */
