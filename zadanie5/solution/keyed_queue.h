/* Autorzy rozwiązania : Michał Radecki, Tomasz Kanas */

#ifndef KONTENER_KEYED_QUEUE_H
#define KONTENER_KEYED_QUEUE_H

#include <list>
#include <map>
#include <memory>

class lookup_error : public std::exception {
private:
    std::string message;
public:
	explicit lookup_error(const std::string& message) {
	    this->message = message;
	}
   virtual const char *what() const throw() {
        return "Lookup error";
   }
};

template <class K, class V>
class Common_queue_class {
public:
	typedef std::pair<K, V> pair_type;
	typedef std::list<pair_type> list_type;
	typedef std::list<typename list_type::iterator> list_of_iterators;
	typedef std::map<K, list_of_iterators> map_type;
	typedef std::pair<K, list_of_iterators> map_pair_type;

	class k_iterator : public map_type::iterator {
	public:
	    k_iterator() : map_type::iterator() {};
	    k_iterator(typename map_type::iterator it) : map_type::iterator(it) {};
	    K const *operator -> () {
			return (K const *) &(map_type::iterator::operator -> ()->first);
	    }
	    K operator * () {
			return map_type::iterator::operator * ().first;
	    }
	};

	list_type my_list;
	map_type my_map;
	bool given_ref = false;

	Common_queue_class() {
	}

	~Common_queue_class() {
	}

	void push(K const &k, V const &v) {
		V cv = v;
		K ck = k;
		pair_type p = pair_type(ck, cv);
		my_list.push_back(p);
		try {
		    if(my_map.count(ck) == 0) {
				list_of_iterators list2;
				typename list_type::iterator it = std::next(my_list.end(), -1);
				list2.push_back(it);
				my_map.insert(map_pair_type(ck, list2));
		    }
		    else {
				list_of_iterators nowa_lista = my_map.at(k);
				typename list_type::iterator it = std::next(my_list.end(), -1);
				nowa_lista.push_back(it);
				my_map[ck] = nowa_lista;
		    }
		}
		catch (...) {
		    my_list.pop_back();
		    throw;
		}
	}

	void pop() {
		if(my_list.empty()) throw lookup_error("Error");
		else {
			pair_type para = my_list.front();
			my_list.pop_front();
			list_of_iterators nowa_lista = my_map.at(para.first);
			nowa_lista.pop_front();
			if(nowa_lista.empty()) my_map.erase(para.first);
			else {
				my_map[para.first] = nowa_lista;
			}
		}
	}

	void pop(K const &k) {
		if(my_map.count(k) == 0) {
			throw lookup_error("Error");
		}
		else {
			list_of_iterators nowa_lista = my_map.at(k);
			typename list_type::iterator it = nowa_lista.front();
			my_list.erase(it);
			nowa_lista.pop_front();
			if(nowa_lista.empty()) my_map.erase(k);
			else {
				my_map[k] = nowa_lista;
			}
		}
	}
	void move_to_back(K const &k) {
		if(my_map.count(k) == 0) {
			throw lookup_error("Error");
		}
		else {
			list_type kopia;
			list_of_iterators stara_lista = my_map.at(k);
			for(typename list_type::iterator it : stara_lista) {
				kopia.push_back(pair_type(it->first, it->second));
			}
			list_of_iterators nowa_lista;
			for(pair_type para : kopia) {
				my_list.push_back(para);
				typename list_type::iterator it = std::next(my_list.end(), -1);
				nowa_lista.push_back(it);
			}
			try {
			    my_map[k] = nowa_lista;
			    for(typename list_type::iterator it : stara_lista) {
				my_list.erase(it);
			    }
			} catch (...) {
				for(size_t i = 0 ; i < stara_lista.size(); ++i) my_list.pop_back();
				throw;
			}
		}
	}

	size_t size() const {
		return my_list.size();
	}

	bool empty() const {
		return my_list.empty();
	}

	size_t count(K const &k) const {
		if(my_map.count(k) == 0) return 0;
		else {
			list_of_iterators nowa_lista = my_map.at(k);
			return nowa_lista.size();
		}
	}

	void clear() {
		my_map.clear();
		my_list.clear();
	}

	std::pair<K const &, V &> front() {
	    if(my_list.empty()) throw lookup_error("Error");
	    return std::make_pair(std::ref(my_list.front().first), std::ref(my_list.front().second));
	}

	std::pair<K const &, V &> back() {
	    if(my_list.empty()) throw lookup_error("Error");
	    return std::make_pair(std::ref(my_list.back().first), std::ref(my_list.back().second));
	}

	std::pair<K const &, V const &> front() const {
	    if(my_list.empty()) throw lookup_error("Error");
	    return std::make_pair(std::ref(my_list.front().first), std::ref(my_list.front().second));
	}

	std::pair<K const &, V const &> back() const {
	    if(my_list.empty()) throw lookup_error("Error");
	    return std::make_pair(std::ref(my_list.back().first), std::ref(my_list.back().second));
	}

	std::pair<K const &, V &> first(K const &key) {
	    if(my_map.count(key) == 0) throw lookup_error("Error");
	    return std::make_pair(std::ref(my_map.at(key).front()->first), std::ref(my_map.at(key).front()->second));
	}

	std::pair<K const &, V &> last(K const &key) {
	    if(my_map.count(key) == 0) throw lookup_error("Error");
	    return std::make_pair(std::ref(my_map.at(key).back()->first), std::ref(my_map.at(key).back()->second));
	}

	std::pair<K const &, V const &> first(K const &key) const {
	    if(my_map.count(key) == 0) throw lookup_error("Error");
	    return std::make_pair(std::ref(my_map.at(key).front()->first), std::ref(my_map.at(key).front()->second));
	}

	std::pair<K const &, V const &> last(K const &key) const {
	    if(my_map.count(&key) == 0) throw lookup_error("Error");
	    return std::make_pair(std::ref(my_map.at(key).back()->first), std::ref(my_map.at(key).back()->second));
	}

	k_iterator k_begin() {
	    return k_iterator(my_map.begin());
	}

	k_iterator k_end() {
	    return k_iterator(my_map.end());
	}
};

template<class K, class V>
class keyed_queue {

private:
	typedef std::pair<K, V> pair_type;
	typedef std::list<pair_type> list_type;
	typedef std::list<typename list_type::iterator> list_of_iterators;
	typedef std::map<K, list_of_iterators> map_type;
	typedef std::pair<K, list_of_iterators> map_pair_type;

	std::shared_ptr<Common_queue_class<K, V>> common_queue_ptr;

	void rewrite() {
		list_type copy;
		for(typename list_type::iterator it = common_queue_ptr->my_list.begin(); it != common_queue_ptr->my_list.end(); it++) {
			K k = it->first;
			V v = it->second;
			copy.push_back(pair_type(k, v));
		}
		std::shared_ptr<Common_queue_class<K, V>> ptr = std::make_shared<Common_queue_class<K, V>>();
		for(pair_type pair1 : copy) {
			ptr->push(pair1.first, pair1.second);
		}
		common_queue_ptr = ptr;
	}
public:
	typedef typename Common_queue_class<K, V>::k_iterator k_iterator;

	keyed_queue() {
		common_queue_ptr = std::make_shared<Common_queue_class<K, V>>();
	}

	keyed_queue(keyed_queue const &to_copy) {
		common_queue_ptr = to_copy.common_queue_ptr;
		try {
		    if(common_queue_ptr->given_ref) rewrite();
		} catch (...) {
		    common_queue_ptr = nullptr;
		    throw;
		}
	}

	keyed_queue(keyed_queue && to_move) noexcept {
		common_queue_ptr=std::move(to_move.common_queue_ptr);
	}

	keyed_queue &operator=(keyed_queue other) {
		auto cpy = common_queue_ptr;
		common_queue_ptr = other.common_queue_ptr;
		try {
		    if(common_queue_ptr->given_ref) rewrite();
		} catch (...) {
		    common_queue_ptr = cpy;
		    throw;
		}
		return *this;
	}

	void push(K const &k, V const &v) {
		if(common_queue_ptr.use_count() > 1) {
			rewrite();
		}
		common_queue_ptr->push(k, v);
	}
	void pop() {
		if(common_queue_ptr->empty()) throw lookup_error("Error");
		else {
			if(common_queue_ptr.use_count() > 1) {
				rewrite();
			}
			common_queue_ptr->pop();
		}
	}
	void pop(K const &k) {
		if(common_queue_ptr->count(k) == 0) throw lookup_error("Error");
		else {
			if(common_queue_ptr.use_count() > 1) {
				rewrite();
			}
			common_queue_ptr->pop(k);
		}
	}
	void move_to_back(K const &k) {
		if(common_queue_ptr->count(k) == 0) throw lookup_error("Error");
		else {
			if(common_queue_ptr.use_count() > 1) {
				rewrite();
			}
			common_queue_ptr->move_to_back(k);
		}
	}

	size_t size() const {
		return common_queue_ptr->size();
	}

	bool empty() const {
		return common_queue_ptr->empty();
	}

	size_t count(K const &k) const {
		return common_queue_ptr->count(k);
	}

	void clear() {
		if(common_queue_ptr.use_count() > 1) {
			rewrite();
		}
		common_queue_ptr->clear();
	}

	std::pair<K const &, V &> front() {
	    auto res = common_queue_ptr->front();
	    if(common_queue_ptr.use_count() > 1) {
		rewrite();
	    }
	    common_queue_ptr->given_ref = true;
	    return res;
	}

	std::pair<K const &, V &> back() {
	    auto res = common_queue_ptr->back();
	    if(common_queue_ptr.use_count() > 1) {
		rewrite();
	    }
	    common_queue_ptr->given_ref = true;
	    return res;
	}

	std::pair<K const &, V const &> front() const {
	    return common_queue_ptr->front();
	}

	std::pair<K const &, V const &> back() const {
	    return common_queue_ptr->back();
	}

	std::pair<K const &, V &> first(K const &key) {
	    auto res = common_queue_ptr->first(key);
	    if(common_queue_ptr.use_count() > 1) {
		rewrite();
	    }
	    common_queue_ptr->given_ref = true;
	    return res;
	}

	std::pair<K const &, V &> last(K const &key) {
	    auto res = common_queue_ptr->last(key);
	    if(common_queue_ptr.use_count() > 1) {
		rewrite();
	    }
	    common_queue_ptr->given_ref = true;
	    return res;
	}

	std::pair<K const &, V const &> first(K const &key) const {
	    return common_queue_ptr->first(key);
	}

	std::pair<K const &, V const &> last(K const &key) const {
	    return common_queue_ptr->last(key);
	}

	k_iterator k_begin() {
	    return common_queue_ptr->k_begin();
	}

	k_iterator k_end() {
	    return common_queue_ptr->k_end();
	}

};
#endif //KONTENER_KEYED_QUEUE_H
