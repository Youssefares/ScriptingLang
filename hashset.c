#include "HashMap.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void HashMapNew(HashMap *h, int elemSize, int numBuckets,
		HashMapHashFunction hashfn, HashMapCompareFunction comparefn, HashMapFreeFunction freefn)
{
  h->elemSize = elemSize;
  h->numBuckets = numBuckets;
  h->buckets = malloc(sizeof(vector) * numBuckets);
  h->compareFunc = comparefn;
  h->freeFunc = freefn;
  h->hashFunc = hashfn;
  //now initalize everysingle bucket
  for(int i = 0; i < numBuckets; i++){
    VectorNew(&h->buckets[i],elemSize,freefn,1);
  }
}

void Dispose(HashMap *h)
{
  for(int i = 0; i < h->numBuckets; i++){
    vector *v = &h->buckets[i];
    VectorDispose(v);
  }
}

int HashMapCount(const HashMap *h)
{ return h->numBuckets; }

void HashMapMap(HashMap *h, HashMapMapFunction mapfn, void *auxData)
{
  for(int i = 0; i < h->numBuckets; i++){
    vector *v = &h->buckets[i];
    for(int j = 0; j < VectorLength(v); j++)
      mapfn(VectorNth(v,j),auxData);
  }
}

void HashMapEnter(HashMap *h, const void *elemAddr, char * key)
{
  int index = h->hashFunc(key,h->numBuckets);
  vector *v = &h->buckets[index];
  for(int i = 0; i < VectorLength(v); i++){
    void * elem = VectorNth(v,i);
    if(h->compareFunc(elem,elemAddr) == 0){
      VectorReplace(v,elemAddr,i);
      return;
    }
  }
  VectorAppend(v,elemAddr);
}

void *HashMapLookup(const HashMap *h, const char *elemAddr)
{
  int elemAddress = h->hashFunc(elemAddr, h->numBuckets);
  vector elems = h->buckets[elemAddress];
  int length = VectorLength(&elems);
  if(length == 0)
    return NULL;
  for(int i = 0; i < length; i++){
    void * element = VectorNth(&elems,i);
    if(h->compareFunc(element, elemAddr)== 0)
       return element;
  }
    return NULL;
 }
