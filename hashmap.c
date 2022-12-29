#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hashmap.h"
#include <stdbool.h>

// Robin Hood hashing.
// The variance of the key distances from their 'home' slots is minimised.
// Based on Open Addressing.
// 	Open Addressing - 
// 	Stores all elements directly in the hash table array (one element per bucket).
//
//

#define DEFAULT_BUCKETS 32

struct HashMap* new_hashmap(HashFunction hfunc, CompareFunction cfunc) {
		return new_hashmap_default(hfunc, cfunc, DEFAULT_BUCKETS);
}

void initBuckets(struct HashMap* map, size_t num_buckets) {
	for (size_t i = 0; i < num_buckets; i++) {
		map->buckets[i] = NULL;
	}
}

struct HashMap* new_hashmap_default(HashFunction hfunc, CompareFunction cfunc, size_t buckets) {
		struct HashMap* hmap;
		hmap = malloc(sizeof(*hmap));
		assert(hmap);

		hmap->buckets = malloc(buckets* sizeof(*hmap->buckets));
		hmap->num_buckets = buckets;
		hmap->hfunc = hfunc;
		hmap->cfunc = cfunc;

		assert(hmap->buckets);

		// Initialise each bucket.
		initBuckets(hmap, buckets);

		return hmap;
}

void free_hashmap(struct HashMap* hmap) {
	free(hmap->buckets);
	free(hmap);
}

void insert_hashmap(struct HashMap* hmap, int key, void* value) {
	// Compute the bucket index based on the hashfunction.
	size_t hashed_key = hmap->hfunc(key);
	// Get the item at the index.
	struct HashMapItem* item = hmap->buckets[hashed_key];
	struct HashMapItem* prev = NULL;

	// If the item is occupied, check if any of the 'nexts' equal
	// the current item key.
	while (item != NULL) {
		if (hmap->cfunc(item->key, key)) {
			prev = item;
			break;
		}
		item = item->next;
	}


	if (item == 0) {
		item = (struct HashMapItem*) malloc(sizeof(struct HashMapItem));
		item->hash_id = hashed_key;
		item->key = key;
		item->value = value;
		item->next = NULL;
	
		// If there is no previous item, this item is the first
		// with this hashed_key,
		if (prev == NULL) {
			hmap->buckets[hashed_key] = item;
		} else {
			// There is a previous item with this hashed_key.
			// Set the previous item to point to this new item.
			prev->next = item;
		}
	} else {
		// Item already exists.
		// Only update the value.
		item->value = value;
	}
}

void * get_hashmap(struct HashMap* hmap, int key) {
	size_t hashed_key = hmap->hfunc(key);
	struct HashMapItem* entry = hmap->buckets[hashed_key];

	while (entry != NULL) {
		if (hmap->cfunc(entry->key, key)) return entry->value;
		entry = entry->next;
	}
	return NULL;
}

void remove_hashmap(struct HashMap* map, int key) {
	size_t hashed_key = map->hfunc(key);
	struct HashMapItem* prev = NULL;
	struct HashMapItem* entry = map->buckets[hashed_key];
		
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

int get_size_hashmap(struct HashMap* map) {
	// Iterate over every bucket.
	int count = 0;
	for (size_t i = 0; i < DEFAULT_BUCKETS; i++) {
		struct HashMapItem* entry = map->buckets[i];

		while (entry != NULL) {
			count++;
			entry = entry->next;
		}
	}	

	return count;
}

void print_hashmap(struct HashMap* map) {
	for (size_t i = 0; i < DEFAULT_BUCKETS; i++) {
		struct HashMapItem* entry = map->buckets[i];
		
		while (entry != NULL) {
			entry = entry->next;
		}
	}	
}

