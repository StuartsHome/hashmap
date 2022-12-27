#include <stdio.h>
#include <assert.h>
#include "hashmap.h"

// Robin Hood hashing.
// The variance of the key distances from their 'home' slots is minimised.
// Based on Open Addressing.
// 	Open Addressing - 
// 	Stores all elements directly in the hash table array (one element per bucket).
//
//

#define DEFAULT_BUCKETS 750000

struct HashMap* new_hashmap(HashFunction hfunc, CompareFunction cfunc) {
		return new_hashmap_c(hfunc, cfunc, DEFAULT_BUCKETS);
}

struct HashMap* new_hashmap_c(
				HashFunction hfunc,
				CompareFunction cfunc,
				size_t buckets) {
		struct HashMap* hmap;
		hmap = malloc(sizeof(*hmap));
		assert(hmap);

		hmap->buckets = malloc(buckets * sizeof(*hmap->buckets));
		hmap->num_buckets = buckets;
		hmap->hfunc = hfunc;
		hmap->cfunc = cfunc;

		assert(hmap->buckets);

		for (size_t i = 0; i < buckets; i++) {
			hmap->buckets[i] = NULL;
		}
		
		return hmap;
}

void free_hashmap(struct HashMap* hmap) {
	free(hmap->buckets);
	free(hmap);
	hmap = NULL;
}

void insert_hashmap(struct HashMap* hmap, void* key, void* value) {
	size_t hashed_key = hmap->hfunc(key);
	struct Pair* prev = NULL;
	struct Pair* entry = hmap->buckets[hashed_key];

	while (entry != NULL) {
		if (hmap->cfunc(entry->key, key)) {
			prev = entry;
			break;
		}
	}

	if (entry == NULL) {
		entry = malloc(sizeof(struct Pair));
		entry->hash_id = hashed_key;
		entry->key = key;
		entry->value = value;
		entry->next = NULL;
		
		if (prev == NULL) {
			hmap->buckets[hashed_key] = entry;
		} else {
			prev->next = entry;
		}
	} else {
		entry->value = value;
	}
}

void * get_hashmap(struct HashMap* hmap, void* key) {
	size_t hashed_key = hmap->hfunc(key);
	struct Pair* entry = hmap->buckets[hashed_key];

	while (entry != NULL) {
		if (hmap->cfunc(entry->key, key)) return entry->value;
		entry = entry->next;
	}
	return NULL;
}

void remove_hashmap(struct HashMap* map, void* key) {
	size_t hashed_key = map->hfunc(key);
	struct Pair* prev = NULL;
	struct Pair* entry = map->buckets[hashed_key];
		
	while (entry != NULL) {
		if (map->cfunc(entry->key, key)) {
			prev = entry;
			break;
		}
		entry = entry->next;
	}

	if (entry == NULL) return; 
	if (prev == NULL) {
		map->buckets[hashed_key] = entry->next;	
	} else {
		prev->next = entry->next;
	}
	free(entry);
	map->buckets[hashed_key] = NULL;
}

void print_hashmap(struct HashMap* map) {
	for (size_t i = 0; i < DEFAULT_BUCKETS; i++) {
		struct Pair* entry = map->buckets[i];
		
		while (entry != NULL) {
			printf("%d\n", *(int *)entry->value);
			entry = entry->next;
		}
	}	
}
