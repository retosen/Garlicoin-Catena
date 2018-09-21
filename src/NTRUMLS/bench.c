/*
 * CPQREF/bench.c
 *
 *  Copyright 2014 John M. Schanck
 *
 *  This file is part of CPQREF.
 *
 *  CPQREF is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  CPQREF is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with CPQREF.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "fastrandombytes.h"
#include "params.h"
#include "pqerror.h"
#include "pqntrusign.h"
#include "pack.h"

#define TRIALS 100
#define VERIFY 1

int bench_param_set(PQ_PARAM_SET_ID id);

int
main(int argc, char **argv)
{
  uint16_t i;
  PQ_PARAM_SET_ID plist[] = {
    XXX_20140508_401,
    XXX_20140508_439,
    XXX_20140508_593,
    XXX_20140508_743,

    XXX_20151024_401,
    XXX_20151024_443,
    XXX_20151024_563,
    //XXX_20151024_509,
    XXX_20151024_743,
    XXX_20151024_907,
    };
  size_t numParams = sizeof(plist)/sizeof(PQ_PARAM_SET_ID);

  for(i = 0; i<numParams; i++)
  {
    bench_param_set(plist[i]);
  }

  rng_cleanup();

  exit(EXIT_SUCCESS);
}

int
bench_param_set(PQ_PARAM_SET_ID id)
{
  int i;

  int valid = 0;

  PQ_PARAM_SET *P;
  size_t privkey_blob_len;
  size_t pubkey_blob_len;

  unsigned char *privkey_blob;
  unsigned char *pubkey_blob;

  unsigned char msg[256] = {0};
  uint16_t msg_len = 256;

  unsigned char *sigs;
  size_t packed_sig_len;


  clock_t c0;
  clock_t c1;

  rng_init();
  if(!(P = pq_get_param_set_by_id(id)))
  {
    exit(EXIT_FAILURE);
  }

  fprintf(stderr, "Testing parameter set %s. %d trials.\n", P->name, TRIALS);

  pq_gen_key(P, &privkey_blob_len, NULL, &pubkey_blob_len, NULL);

  //printf("privkey_blob_len: %d\n", (int) privkey_blob_len);
  //printf("pubkey_blob_len: %d\n", (int) pubkey_blob_len);

  privkey_blob = malloc(privkey_blob_len);
  pubkey_blob = malloc(pubkey_blob_len);

  c0 = clock();
  for(i=0; i<TRIALS; i++) {
    msg[(i&0xff)]++; /* Hash a different message each time */
    pq_gen_key(P, &privkey_blob_len, privkey_blob,
               &pubkey_blob_len, pubkey_blob);
  }

  c1 = clock();
  printf("Time/key: %fs\n", (float) (c1 - c0)/(TRIALS*CLOCKS_PER_SEC));

  pq_sign(&packed_sig_len, NULL, privkey_blob_len, privkey_blob, pubkey_blob_len, pubkey_blob, 0, NULL);
  printf("packed_sig_len %d\n", (int) packed_sig_len);

  sigs = malloc(TRIALS * packed_sig_len);

  memset(msg, 0, 256);
  valid = 0;
  c0 = clock();
  for(i=0; i<TRIALS; i++) {
    msg[(i&0xff)]++; /* Hash a different message each time */
    valid += (PQNTRU_OK == pq_sign(&packed_sig_len, sigs + (i*packed_sig_len),
                                   privkey_blob_len, privkey_blob,
                                   pubkey_blob_len, pubkey_blob,
                                   msg_len, msg));
  }
  c1 = clock();
  printf("Time/signature: %fs\n", (float) (c1 - c0)/(TRIALS*CLOCKS_PER_SEC));
  printf("Good signatures %d/%d\n", valid, TRIALS);

  memset(msg, 0, 256);
  valid = 0;
  c0 = clock();
  for(i=0; i<TRIALS; i++) {
    msg[(i&0xff)]++;
    valid += (PQNTRU_OK == pq_verify(packed_sig_len, sigs + (i*packed_sig_len),
                                     pubkey_blob_len, pubkey_blob,
                                     msg_len, msg));
  }
  c1 = clock();
  printf("Time/verification: %fs\n", (float) (c1 - c0)/(TRIALS*CLOCKS_PER_SEC));
  printf("Verified %d/%d\n\n\n", (int)valid, (int)TRIALS);


  free(sigs);
  free(privkey_blob);
  free(pubkey_blob);

  rng_cleanup();

  return EXIT_SUCCESS;
}
