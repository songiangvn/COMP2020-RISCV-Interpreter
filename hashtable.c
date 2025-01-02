#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"
#include "hashtable.h"

struct hashtable
{
    // TODO: define hashtable struct to use linkedlist as buckets
    int cntBuckets;         // number of Buckets
    int cntMappings;        // number of cntMappings
    linkedlist_t **buckets; // array of linkedlist_t pointers
};

/**
 * Hash function to hash a key into the range [0, max_range)
 */
static int hash(int key, int max_range)
{
    // TODO: feel free to write your own hash function (NOT REQUIRED)
    key = (key > 0) ? key : -key;
    return key % max_range;
}
// use default hash function

hashtable_t *ht_init(int num_buckets)
{
    // TODO: create a new hashtable
    hashtable_t *myTable = malloc(sizeof(hashtable_t));
    // init value
    myTable->cntBuckets = num_buckets;
    myTable->cntMappings = 0;
    myTable->buckets = malloc(sizeof(linkedlist_t *) * num_buckets);
    for (int i = 0; i < num_buckets; i++)
        myTable->buckets[i] = ll_init(); // init an empty linked list for bucket i
    return myTable;
}

void ht_add(hashtable_t *table, int key, int value)
{
    // TODO: create a new mapping from key -> value.
    // If the key already exists, replace the value.
    int indexOfBuckets = hash(key, table->cntBuckets);
    linkedlist_t *myBucket = table->buckets[indexOfBuckets];
    table->cntMappings -= ll_size(myBucket); // -= size before update
    ll_add(myBucket, key, value);
    table->cntMappings += ll_size(myBucket); // += size after update
}

int ht_get(hashtable_t *table, int key)
{
    // TODO: retrieve the value mapped to the given key.
    // If it does not exist, return 0
    if (table == NULL)
        return 0;
    int indexOfBuckets = hash(key, table->cntBuckets);
    linkedlist_t *myBucket = table->buckets[indexOfBuckets];
    return ll_get(myBucket, key);
}

int ht_size(hashtable_t *table)
{
    // TODO: return the number of mappings in this hashtable
    if (table == NULL)
        return 0;
    return table->cntMappings;
}
