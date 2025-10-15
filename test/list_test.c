#include "list_test.h"

#include "../src/util/list.h"
#include <stdio.h>

#define TEST_PREFIX    "[LIST_TEST] "

#define SZ        ((size_t)10)
#define ELEM_TYPE int

int list_test(void) {
  struct list* list_p = list_init(SZ, sizeof(ELEM_TYPE));
  if (list_p == NULL) {
    fprintf(stderr, TEST_PREFIX "List not allocated\n");
    return -1;
  }
  if (list_p->sz != SZ || list_p->elem_sz != sizeof(ELEM_TYPE)) {
    fprintf(stderr, TEST_PREFIX "List sizes do not match (expected [%ld,%ld], got [%ld,%ld] instead)\n",
            SZ, sizeof(ELEM_TYPE),
            list_p->sz, list_p->elem_sz);
    list_free(list_p);
    list_p = NULL;
    return -1;
  }
  for (size_t i = 0; i < SZ; ++i) {
    ELEM_TYPE elem = ((ELEM_TYPE*)(list_p->data))[i];
    if (elem != (ELEM_TYPE)0) {
      fprintf(stderr, TEST_PREFIX "Not all elements are set to zero\n");
      list_free(list_p);
      list_p = NULL;
      return -1;
    }
  }
  list_free(list_p);
  list_p = NULL;
  printf(TEST_PREFIX "OK\n");
  return 0;
}
