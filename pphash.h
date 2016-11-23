#ifndef __PPHASH_H
#define __PPHASH_H

struct ht_kv_s {
  char *key;
  void *value;
  struct ht_kv_s *next;
};

typedef struct ht_kv_s ht_kv_t;

struct hashtable_s {
  int size;
  int (*destroy_value)(void *);
  int item_count;
  struct ht_kv_s **table; 
};

typedef struct hashtable_s hashtable_t;

hashtable_t *ht_create( int size, int (*free_value)(void *) ); 
void  ht_destroy( hashtable_t *hashtable);
int ht_hash( hashtable_t *hashtable, char *key );
ht_kv_t *ht_pair( char *key, void *value );
void ht_set( hashtable_t *hashtable, char *key, void *value );
void *ht_get( hashtable_t *hashtable, char *key );
int  ht_size(hashtable_t *hashtable);

#endif
