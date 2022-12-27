#include <stdlib.h>
#include <stdbool.h>

typedef size_t (*HashFunction)(void*);
typedef bool (*CompareFunction)(void*, void*);

struct Pair {
	size_t hash_id;
	void* key;
	void* value;
	struct Pair* next;
};

struct HashMap {
	struct Pair** buckets;
	size_t num_buckets;
	HashFunction hfunc;
	CompareFunction cfunc;
};

struct HashMap* new_hashmap(HashFunction, CompareFunction);
struct HashMap* new_hashmap_c(HashFunction, CompareFunction, size_t);

void free_hashmap(struct HashMap*);
void* get_hashmap(struct HashMap*, void*);
void remove_hashmap(struct HashMap*, void*);
void print_hashmap(struct HashMap*);

