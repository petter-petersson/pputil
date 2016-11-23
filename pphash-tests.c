#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pphash.h"

static int test_passed = 0;
static int test_failed = 0;

#define fail()  return __LINE__

#define check(cond) do { if (!(cond)) fail(); } while (0)


// destroy method for values
static int free_jambo(void * v){

  char * str = (char *) v;
  printf("free_jambo called: %s\n", str);
  return 0;
}

/* Test runner */
static void test(int (*func)(void), const char *name) {
  int r = func();
  if (r == 0) {
    test_passed++;
  } else {
    test_failed++;
    printf("FAILED: %s (at line %d)\n", name, r);
  }
}

int test_add_to_same_key(){
  hashtable_t *ht = ht_create( 65536, free_jambo );

  ht_set( ht, "key1", "blorg" );

  ht_set( ht, "key1", "bloff" );
  ht_set( ht, "key1", "gladd" );
  ht_set( ht, "key1", "grutt" );
  ht_set( ht, "key1", "twerky" );
  ht_set( ht, "key1", "lennart" );
  ht_set( ht, "key2", "sverker" );
  ht_set( ht, "key3", "Rogge" );
  ht_set( ht, "key4", "Swutt" );


  check( strcmp( ht_get(ht, "key1"), "lennart") == 0 );
  check( strcmp( ht_get(ht, "key2"), "sverker") == 0);
  check( strcmp( ht_get(ht, "key3"), "Rogge") == 0);
  check( strcmp( ht_get(ht, "key4"), "Swutt") == 0);

  check(ht_size(ht) == 4);

  ht_destroy(ht);

  return 0;
}

struct gv_s {
  int i1;
  char * s1;
  char * s2;
};
typedef struct gv_s gv_t;

static gv_t * create_gv_t(int i1, char * s1, char * s2){
  gv_t * obj;
  if( ( obj = malloc( sizeof( gv_t ) ) ) == NULL ) {
    return NULL;
  }

  if( ( obj->s1 = strdup( s1 ) ) == NULL ) {
    return NULL;
  }
  if( ( obj->s2 = strdup( s2 ) ) == NULL ) {
    return NULL;
  }
  obj->i1 = i1;

  return obj;
}
static int destroy_gv_t(void * v){

  //printf("destroying gv_t\n");

  gv_t * obj = (gv_t *) v;

  if( obj->s1 != NULL ) {
    free(obj->s1);
  }
  if( obj->s2 != NULL ) {
    free(obj->s2);
  }
  if( obj != NULL ) {
    free(obj);
  }
  return 0;
}

int test_add_custom_struct_as_value(){
  hashtable_t *ht = ht_create( 65536, destroy_gv_t );

  ht_set( ht, "key4", create_gv_t(10,"jamse", "fjamse") );
  check( strcmp( ((gv_t *) ht_get(ht, "key4"))->s1, "jamse") == 0 );
  check( strcmp( ((gv_t *) ht_get(ht, "key4"))->s2, "fjamse") == 0 );
  check(((gv_t *) ht_get(ht, "key4"))->i1 == 10 );
  ht_destroy(ht);
 

  return 0;
}

int test_perfomance(){
  hashtable_t *ht = ht_create( 65536, destroy_gv_t );

  char key[10];
  char s1[10];
  char s2[10];

  for(int i=0;i < 50000; i++){

    sprintf(key,"key-%d", i);
    sprintf(s1,"s1-%d", i);
    sprintf(s2,"s2-%d", i);

    ht_set( ht, key, create_gv_t(i, s1, s2) );

    check( strcmp( ((gv_t *) ht_get(ht, key))->s1, s1) == 0 );
    check( strcmp( ((gv_t *) ht_get(ht, key))->s2, s2) == 0 );
    check(((gv_t *) ht_get(ht, key))->i1 == i );

  }
  ht_destroy(ht);
 

  return 0;
}

int main() {
  test(test_add_to_same_key, "add and assert returned values");
  //FOR more versatility - add destroy method to struct
  test(test_add_custom_struct_as_value, "add and destroy custom struct as value");
  test(test_perfomance, "test performance");
}
