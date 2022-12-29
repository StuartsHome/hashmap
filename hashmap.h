#include <stdlib.h>
#include <stdbool.h>

typedef size_t (*HashFunction)(int);
typedef bool (*CompareFunction)(int, int);

struct HashMapItem {
	size_t 				hash_id;
	int 				key;
	void* 				value;
	struct HashMapItem* next;
};

struct HashMap {
	struct HashMapItem** 	buckets;
	size_t        			num_buckets;
	HashFunction			hfunc;
	CompareFunction 		cfunc;
};

struct HashMap* new_hashmap(HashFunction, CompareFunction);
struct HashMap* new_hashmap_default(HashFunction, CompareFunction, size_t);

void  free_hashmap(struct HashMap*);
void  insert_hashmap(struct HashMap* hmap, int key, void* value);
void* get_hashmap(struct HashMap*, int);
void  remove_hashmap(struct HashMap*, int);
void  print_hashmap(struct HashMap*);
int   get_size_hashmap(struct HashMap*);

