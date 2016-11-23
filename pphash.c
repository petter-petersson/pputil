#include "pphash.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

// 
// Based largely on https://gist.github.com/tonious/1377667
// - added destroy methods, 
// - support for generic value obj
// - tests
//

#ifdef TARGET_OS_MAC
//should be declared in string.h but seems not to be
extern char *strdup(const char *);

#endif


hashtable_t *ht_create( int size, int(*free_value)(void *) ) {

  hashtable_t *hashtable = NULL;
  int i;

  if( size < 1 ) return NULL;

  if( ( hashtable = malloc( sizeof( hashtable_t ) ) ) == NULL ) {
    return NULL;
  }

  if( ( hashtable->table = malloc( sizeof( ht_kv_t * ) * size ) ) == NULL ) {
    return NULL;
  }
  for( i = 0; i < size; i++ ) {
    hashtable->table[i] = NULL;
  }

  hashtable->size = size;
  hashtable->item_count = 0;

  hashtable->destroy_value = free_value;

  return hashtable; 
}


/* values should be freed by delegate method supplied by user*/
void ht_destroy( hashtable_t *hashtable){
  ht_kv_t *pair;
  if(hashtable != NULL){

    if(hashtable->table != NULL){
      for(int i=0;i< hashtable->size; i++){


        pair = hashtable->table[i];
        while( pair != NULL) {
          ht_kv_t * obj = pair;
          pair = pair->next;

          if(obj->key != NULL){
            free(obj->key);
          }
          if(obj->value != NULL){
            //printf("calling value destroy method\n");
            hashtable->destroy_value(obj->value);
          }
          //printf("freeing pair\n");
          free(obj);
        }
      }
      //printf("freeing table\n");
      free(hashtable->table);
    }
    //printf("freeing hashtable\n");
    free(hashtable);
  }
}

int ht_hash( hashtable_t *hashtable, char *key ) {

  unsigned long int hashval = 0;
  int i = 0;

  while( hashval < ULONG_MAX && i < strlen( key ) ) {
    hashval = hashval << 8;
    hashval += key[ i ];
    i++;
  }

  return hashval % hashtable->size;
}

ht_kv_t *ht_pair( char *key, void *value ) {
  ht_kv_t *pair;

  if( ( pair = malloc( sizeof( ht_kv_t ) ) ) == NULL ) {
    return NULL;
  }

  if( ( pair->key = strdup( key ) ) == NULL ) {
    return NULL;
  }

  pair->value = value;
  pair->next = NULL;
  return pair;
}

void ht_set( hashtable_t *hashtable, char *key, void *value ) {
  int bin = 0;
  ht_kv_t *pair = NULL;
  ht_kv_t *next = NULL;
  ht_kv_t *last = NULL;

  bin = ht_hash( hashtable, key );

  next = hashtable->table[ bin ];

  while( next != NULL && next->key != NULL && strcmp( key, next->key ) > 0 ) {
    last = next;
    next = next->next;
  }

  if( next != NULL && next->key != NULL && strcmp( key, next->key ) == 0 ) {

    hashtable->destroy_value(next->value);
    next->value = value;

  } else {
    pair = ht_pair( key, value );

    if( next == hashtable->table[ bin ] ) {
      pair->next = next;
      hashtable->table[ bin ] = pair;

    } else if ( next == NULL ) {
      last->next = pair;

    } else  {
      pair->next = next;
      last->next = pair;
    }

    hashtable->item_count++;
  }
}

void * ht_get( hashtable_t *hashtable, char *key ) {
  int bin = 0;
  ht_kv_t *pair;

  bin = ht_hash( hashtable, key );

  pair = hashtable->table[ bin ];
  while( pair != NULL && pair->key != NULL && strcmp( key, pair->key ) > 0 ) {
    pair = pair->next;
  }

  if( pair == NULL || pair->key == NULL || strcmp( key, pair->key ) != 0 ) {
    return NULL;

  } else {
    return pair->value;
  }
}

int  ht_size(hashtable_t *hashtable){
  return hashtable->item_count;
}


