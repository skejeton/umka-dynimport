#include "umka_api.h"
#include <stdio.h>

struct Umka {
  void *ptr;
};

UMKA_EXPORT void test(UmkaStackSlot *params, UmkaStackSlot *result) {
  result->intVal = params[0].intVal + params[1].intVal;
}

void freeumka(UmkaStackSlot *params, UmkaStackSlot *result) {
  void *umka = (void *)result;
  UmkaAPI *api = umkaGetAPI(umka);

  api->umkaRun(((struct Umka *)params[0].ptrVal));
  api->umkaFree(((struct Umka *)params[0].ptrVal)->ptr);
}

UMKA_EXPORT void dyninit(UmkaStackSlot *params, UmkaStackSlot *result) {
  void *umka = (void *)result;
  UmkaAPI *api = umkaGetAPI(umka);

  printf("It reaches here\n");

  printf("Umka version: %s\n", api->umkaGetVersion());

  printf("But not here\n");

  struct Umka *newumka =
      api->umkaAllocData(umka, sizeof(struct Umka), freeumka);

  newumka->ptr = api->umkaAlloc();

  api->umkaInit(newumka->ptr, params[0].ptrVal, NULL, 2048, NULL, 0, NULL, true,
                true, NULL);

  api->umkaCompile(newumka->ptr);

  result->ptrVal = newumka;
}

UMKA_EXPORT void dyncall(UmkaStackSlot *params, UmkaStackSlot *result) {
  void *umka = (void *)result;
  UmkaAPI *api = umkaGetAPI(umka);

  struct Umka *newumka = params[1].ptrVal;

  int offs = api->umkaGetFunc(newumka->ptr, NULL, params[0].ptrVal);
  api->umkaCall(newumka->ptr, offs, 0, NULL, result);
}