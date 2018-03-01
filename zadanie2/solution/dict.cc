/* Autorzy rozwiązania : Michał Radecki, Wojciech Ciszewski */

#include <string>
#include <unordered_map>
#include <iostream>
#include "dict.h"
#include "dictglobal.h"

using namespace std;

namespace {

#ifndef NDEBUG
const bool& debug() {
    static const bool debug = true;
    return debug;
}
#else
const bool& debug() {
    static const bool debug = false;
    return debug;
}
#endif

using Dict = unordered_map<string, string>;

unsigned long& next_id() {
    static unsigned long id = 0;
    return id;
}

unordered_map<unsigned long, Dict>& dicts() {
    static unordered_map<unsigned long, Dict> dictionaries;
    return dictionaries;
}

string dict_name(unsigned long id) {
    if (id == jnp1::dict_global()) {
        return "the Global Dictionary";
    } else {
        return "dict " + to_string(id);
    }
}

string string_to_print(const char* ptr) {
    if (ptr == NULL) {
        return "NULL";
    } else {
        string s(ptr);
        return "\"" + s + "\"";
    }
}

const char* get_from_dict(unsigned long id, const char* key) {
    auto dict_iterator = dicts().find(id);

    if (dict_iterator == dicts().end()) {
        if (debug()) {
            cerr << "dict_find: " << dict_name(id) << " does not exist\n";
            cerr << "dict_find: the key " << string_to_print(key) << " not found, looking up the Global Dictionary\n";
        }
        return get_from_dict(jnp1::dict_global(), key);
    }

    string key_str(key);
    Dict& dict = dict_iterator->second;
    auto value_iterator = dict.find(key_str);
    if (value_iterator == dict.end()) {
        if (id == jnp1::dict_global()) {
            if (debug()) cerr << "dict_find: the key " << string_to_print(key) << " not found\n";
            return NULL;
        } else {
            if (debug()) {
                cerr << "dict_find: the key " << string_to_print(key)
                        << " not found, looking up the Global Dictionary\n";
            }
            return get_from_dict(jnp1::dict_global(), key);
        }
    } else {
        const char* value = value_iterator->second.c_str();
        if (debug()) {
            cerr << "dict_find: " << dict_name(id) << ", the key " << string_to_print(key) << " has the value "
                    << string_to_print(value) << "\n";
        }
        return value;
    }
}

bool can_copy_to_global (const Dict& src_dict, const Dict& global_dict) {
    size_t quantity_to_insert = 0;
    for (auto src_value_iterator : src_dict) {
        string key_str = src_value_iterator.first;
        if (global_dict.find(key_str) == global_dict.end()) {
            quantity_to_insert++;
        }
    }
    if (global_dict.size() + quantity_to_insert > jnp1::MAX_GLOBAL_DICT_SIZE) {
        return false;
    }

    return true;
}

}

namespace jnp1 {

unsigned long dict_new() {
    Dict new_dict;
    dicts()[next_id()] = new_dict;
    if (debug()) {
        cerr << "dict_new()\n";
        cerr << "dict_new: dict " << next_id() << " has been created\n";
    }
    next_id()++;
    return next_id() - 1;
}

void dict_delete(unsigned long id) {
    if (debug()) cerr << "dict_delete(" << id << ")\n";
    unordered_map<unsigned long, Dict>::iterator it = dicts().find(id);
    if (it != dicts().end()) {
        if (id != dict_global()) {
            dicts().erase(it);
            if (debug()) cerr << "dict_delete: dict " << id << " has been deleted\n";
        } else {
            if (debug()) cerr << "dict_delete: an attempt to remove the Global Dictionary\n";
        }
    } else {
        if (debug()) cerr << "dict_delete: dict " << id << " does not exist\n";
    }
}

size_t dict_size(unsigned long id) {
    if (debug()) cerr << "dict_size(" << id << ")\n";
    unordered_map<unsigned long, Dict>::iterator it = dicts().find(id);
    if (it != dicts().end()) {
        if (debug()) cerr << "dict_size: " << dict_name(id) << " contains " << it->second.size() << " element(s)\n";
        return it->second.size();
    } else {
        if (debug()) cerr << "dict_size: dict " << id << " does not exist\n";
        return 0;
    }
}

void dict_insert(unsigned long id, const char* key, const char* value) {
    if (debug()) {
        cerr << "dict_insert(" << id << ", " << string_to_print(key) << ", " << string_to_print(value) << ")\n";
    }
    auto it = dicts().find(id);
    if (it == dicts().end()) {
        if (debug()) cerr << "dict_insert: dict " << id << " does not exist\n";
        return;
    } else {
        if (key != NULL && value != NULL) {
            Dict& dict = it->second;
            string key_str(key);
            if (id == dict_global() && dict.size() == MAX_GLOBAL_DICT_SIZE && dict.find(key_str) == dict.end()) {
                if (debug()) {
                    cerr << "dict_insert: the Global Dictionary has reached its maximum size, "
                            << "can not insert a new element\n";
                }
            } else {
                string value_str(value);
                dict[key_str] = value_str;
                if (debug()) {
                    cerr << "dict_insert: " << dict_name(id) << ", the pair (" << string_to_print(key) << ", "
                            << string_to_print(value) << ") has been inserted\n";
                }
            }
        } else if (debug()) {
            if (key == NULL) cerr << "dict_insert: " << dict_name(id) << ", an attempt to insert NULL key\n";
            if (value == NULL) cerr << "dict_insert: " << dict_name(id) << ", an attempt to insert NULL value\n";
        }
    }
}

void dict_remove(unsigned long id, const char* key) {
    if (debug()) cerr << "dict_remove(" << id << ", " << string_to_print(key) << ")\n";

    if (key == NULL) {
        if (debug()) cerr << "dict_remove: " << dict_name(id) << ", an attempt to remove NULL key\n";
        return;
    }

    string key_str(key);

    auto dict_iterator = dicts().find(id);
    if (dict_iterator == dicts().end()) {
        if (debug()) cerr << "dict_remove: " << dict_name(id) << " does not exist\n";
    } else {
        auto removed_quantity = dict_iterator->second.erase(key_str);
        if (removed_quantity == 0) {
            if (debug()) {
                cerr << "dict_remove: " << dict_name(id) << " does not contain the key " << string_to_print(key)
                        << "\n";
            }
        } else {
            if (debug()) {
                cerr << "dict_remove: " << dict_name(id) << ", the key " << string_to_print(key)
                        << " has been removed\n";
            }
        }
    }
}

const char* dict_find(unsigned long id, const char* key) {
    if (debug()) cerr << "dict_find(" << id << ", " << string_to_print(key) << ")\n";
    if (key == NULL) {
        if (debug()) cerr << "dict_find: " << dict_name(id) << ", an attempt to find NULL key\n";
        return NULL;
    }
    return get_from_dict(id, key);
}

void dict_clear(unsigned long id) {
    if (debug()) cerr << "dict_clear(" << id << ")\n";

    auto dict_iterator = dicts().find(id);
    if (dict_iterator == dicts().end()) {
        if (debug()) cerr << "dict_clear: " << dict_name(id) << " does not exist\n";
    } else {
        if (debug()) cerr << "dict_clear: " << dict_name(id) << " has been cleared\n";
        dict_iterator->second.clear();
    }
}

void dict_copy(unsigned long src_id, unsigned long dst_id) {
    if (debug()) cerr << "dict_copy(" << src_id << ", " << dst_id << ")\n";

    if (src_id == dst_id) {
        if (debug()) cerr << "dict_copy: source and destination are the same\n";
        return;
    }

    auto src_iterator = dicts().find(src_id);
    auto dst_iterator = dicts().find(dst_id);

    if (src_iterator == dicts().end() || dst_iterator == dicts().end()) {
        if (debug()) {
            if (src_iterator == dicts().end()) cerr << "dict_copy: " << dict_name(src_id) << " does not exist\n";
            if (dst_iterator == dicts().end()) cerr << "dict_copy: " << dict_name(dst_id) << " does not exist\n";
        }
        return;
    }

    Dict& src_dict = src_iterator->second;
    Dict& dst_dict = dst_iterator->second;

    if (dst_id == dict_global()) {
        if (!can_copy_to_global(src_dict, dst_dict)) {
            if (debug()) cerr << "dict_copy: no room in the Global Dictionary\n";
            return;
        }
    }

    for (auto src_value_iterator : src_dict) {
        string key_str = src_value_iterator.first;
        string value_str = src_value_iterator.second;
        dst_dict[key_str] = value_str;
    }

    if (debug()) cerr << "dict_copy: " << dict_name(src_id) << " has been copied to " << dict_name(dst_id) << "\n";
}

}
