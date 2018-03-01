/* Autorzy rozwiązania : Michał Radecki, Wojciech Ciszewski */

#include "dict.h"
#include "dictglobal.h"

namespace jnp1 {

unsigned long dict_global() {
    static unsigned long global_id = dict_new();
    return global_id;
}

}
