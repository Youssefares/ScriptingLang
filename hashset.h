#ifndef _HashMap_
#define _HashMap_
#include "vector.h"

/* File: hashtable.h
 * ------------------
 * Defines the interface for the HashMap.
 */

/**
 * Type: HashMapHashFunction
 * -------------------------
 * Class of function designed to map the figure at the specied
 * elemAddr to some number (the hash code) between 0 and numBuckets - 1.
 * The hashing routine must be stable in that the same number must
 * be returned every single time the same element (where same is defined
 * in the HashMapCompareFunction sense) is hashed.  Ideally, the
 * hash routine would manage to distribute the spectrum of client elements
 * as uniformly over the [0, numBuckets) range as possible.
 */

typedef int (*HashMapHashFunction)(const void *elemAddr, int numBuckets);

/**
 * Type: HashMapCompareFunction
 * ----------------------------
 * Class of function designed to compare two elements, each identified
 * by address.  The HashMapCompareFunction compares these elements and
 * decides whether or not they are logically equal or or.  The
 * return value identifies relative ordering:
 *
 *   - A negative return value means that the item addressed by elemAddr1
 *     is less than the item addressed by elemAddr2.  The definition of
 *     'less than' is dictated by the implementation's interpretation of the data.
 *   - A zero return value means that the two items addressed by elemAddr1
 *     and elemAddr2 are equal as far as the comparison routine is concerned.
 *   - A positive return value means that the item addressed by elemAddr2
 *     is less that the item addressed by elemAddr1.
 */

typedef int (*HashMapCompareFunction)(const void *elemAddr1, const void *elemAddr2);

/**
 * Type: HashMapMapFunction
 * ------------------------
 * Class of function that can me mapped over the elements stored in a HashMap.
 * These map functions accept a pointer to a client element and a pointer
 * to a piece of auxiliary data passed in as the second argument to HashMapMap.
 */

typedef void (*HashMapMapFunction)(void *elemAddr, void *auxData);

/**
 * Type: HashMapFreeFunction
 * -------------------------
 * Class of functions designed to dispose of and/or clean up
 * any resources embedded within the element at the specified
 * address.  Typically, such resources are dynamically allocated
 * memory, open file pointers, and data structures requiring that
 * some specific cleanup routine be called.
 */

typedef void (*HashMapFreeFunction)(void *elemAddr);

/**
 * Type: HashMap
 * -------------
 * The concrete representation of the HashMap.
 * In spite of all of the fields being publicly accessible, the
 * client is absolutely required to initialize, dispose of, and
 * otherwise interact with all HashMap instances via the suite
 * of the six HashMap-related functions described below.
 */

typedef struct {
  // to be filled in by you
  int elemSize;
  //vector v;
  HashMapFreeFunction freeFunc;
  int numBuckets;
  vector *buckets;
  HashMapCompareFunction compareFunc;
  HashMapHashFunction hashFunc;
} HashMap;

/**
 * Function:  HashMapNew
 * ---------------------
 * Initializes the identified HashMap to be empty.  It is assumed that
 * the specified HashMap is either raw memory or one previously initialized
 * but later destroyed (using HastSetDispose.)
 *
 * The elemSize  parameter specifies the number of bytes that a single element of the
 * table should take up. For example, if you want to store elements of type
 * Binky, you would pass sizeof(Binky) as this parameter. An assert is
 * raised if this size is less than or equal to 0.
 *
 * The numBuckets parameter specifies the number of buckets that the elements
 * will be partitioned into.  Once a HashMap is created, this number does
 * not change.  The numBuckets parameter must be in sync with the behavior of
 * the hashfn, which must return a hash code between 0 and numBuckets - 1.
 * The hashfn parameter specifies the function that is called to retrieve the
 * hash code for a given element.  See the type declaration of HashMapHashFunction
 * above for more information.  An assert is raised if numBuckets is less than or
 * equal to 0.
 *
 * The comparefn is used for testing equality between elements.  See the
 * type declaration for HashMapCompareFunction above for more information.
 *
 * The freefn is the function that will be called on an element that is
 * about to be overwritten (by a new entry in HashMapEnter) or on each element
 * in the table when the entire table is being freed (using HashMapDispose).  This
 * function is your chance to do any deallocation/cleanup required,
 * (such as freeing any pointers contained in the element). The client can pass
 * NULL for the freefn if the elements don't require any handling.
 * An assert is raised if either the hash or compare functions are NULL.
 *
 * An assert is raised unless all of the following conditions are met:
 *    - elemSize is greater than 0.
 *    - numBuckets is greater than 0.
 *    - hashfn is non-NULL
 *    - comparefn is non-NULL
 */

void HashMapNew(HashMap *h, int elemSize, int numBuckets,
		HashMapHashFunction hashfn, HashMapCompareFunction comparefn, HashMapFreeFunction freefn);

/**
 * Function: HashMapDispose
 * ------------------------
 * Disposes of any resources acquired during the lifetime of the
 * HashMap.  It does not dispose of client elements properly unless the
 * HashMapFreeFunction specified at construction time does the right
 * thing.  HashMapDispose will apply this cleanup routine to all
 * of the client elements stored within.
 *
 * Once HashMapDispose has been called, the HashMap is rendered
 * useless.  The diposed of HashMap should not be passed to any
 * other HashMap routines (unless for some reason you want to reuse
 * the same HashMap variable and re-initialize is by passing it to
 * HashMapNew... not recommended.)
 */

void HashMapDispose(HashMap *h);

/**
 * Function: HashMapCount
 * ----------------------
 * Returns the number of elements residing in
 * the specified HashMap.
 */

int HashMapCount(const HashMap *h);

/**
 * Function: HashMapEnter
 * ----------------------
 * Inserts the specified element into the specified
 * HashMap.  If the specified element matches an
 * element previously inserted (as far as the hash
 * and compare functions are concerned), the the
 * old element is replaced by this new element.
 *
 * An assert is raised if the specified address is NULL, or
 * if the embedded hash function somehow computes a hash code
 * for the element that is out of the [0, numBuckets) range.
 */

void HashMapEnter(HashMap *h, const void *elemAddr, char * key);

/**
 * Function: HashMapLookup
 * -----------------------
 * Examines the specified HashMap to see if anything matches
 * the item residing at the specified elemAddr.  If a match
 * is found, then the address of the stored item is returned.
 * If no match is found, then NULL is returned as a sentinel.
 * Understand that the key (residing at elemAddr) only needs
 * to match a stored element as far as the hash and compare
 * functions are concerned.
 *
 * An assert is raised if the specified address is NULL, or
 * if the embedded hash function somehow computes a hash code
 * for the element that is out of the [0, numBuckets) range.
 */

void *HashMapLookup(const HashMap *h, const char *elemAddr);

/**
 * Function: HashMapMap
 * --------------------
 * Iterates over all of the stored elements, applying the specified
 * mapfn to the addresses of each.  The auxData parameter can be
 * used to propagate additional data to each of the mapfn calls; in
 * fact, the auxData value is passed in as the second argument to
 * each mapfn application.  It is the responsibility of the mapping
 * function to received that auxData and handle it accordingly.  NULL
 * should be passed in as the auxData if (and only if) the mapping
 * function itself ignores its second parameter.
 *
 * An assert is raised if the mapping routine is NULL.
 */

void HashMapMap(HashMap *h, HashMapMapFunction mapfn, void *auxData);

#endif
