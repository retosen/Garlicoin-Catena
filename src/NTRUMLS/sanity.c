/*
 * CPQREF/sanity.c
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
#include "pol.h"

#define TIMES 100

static int
testPack(PQ_PARAM_SET_ID id)
{
  int i;
  int T;
  int rc;
  PQ_PARAM_SET *P;
  if(!(P = pq_get_param_set_by_id(id)))
  {
    return -1;
  }
  fprintf(stderr, "PACK: testing parameter set %s\n", P->name);

  unsigned char *scratch;
  uint16_t *iF;
  uint16_t *oF;
  uint16_t *ig;
  uint16_t *og;
  int64_t *iginv;
  int64_t *oginv;
  int64_t *ih;
  int64_t *oh;
  int64_t *isig;
  int64_t *psig;
  int64_t *osig;
  unsigned char *priv_blob;
  unsigned char *pub_blob;
  unsigned char *sig_blob;
  int8_t *sp;

  size_t prod = 2*(P->d1 + P->d2 + P->d3)*sizeof(uint16_t);
  size_t full = P->N*sizeof(int64_t);
  size_t priv_blob_len = PRIVKEY_PACKED_BYTES(P);
  size_t pub_blob_len = PUBKEY_PACKED_BYTES(P);
  size_t sig_len = SIGNATURE_BYTES(P);
  size_t offset;

  scratch = malloc(4*prod + 7*full + priv_blob_len + pub_blob_len + sig_len + P->N);

  offset = 0;
  iF = (uint16_t*)(scratch + offset); offset += prod;
  oF = (uint16_t*)(scratch + offset); offset += prod;
  ig = (uint16_t*)(scratch + offset); offset += prod;
  og = (uint16_t*)(scratch + offset); offset += prod;
  iginv = (int64_t*)(scratch + offset); offset += full;
  oginv = (int64_t*)(scratch + offset); offset += full;
  isig = (int64_t*)(scratch + offset); offset += full;
  psig = (int64_t*)(scratch + offset); offset += full;
  osig = (int64_t*)(scratch + offset); offset += full;
  ih = (int64_t*)(scratch + offset); offset += full;
  oh = (int64_t*)(scratch + offset); offset += full;
  priv_blob = (unsigned char*)(scratch + offset); offset += priv_blob_len;
  pub_blob = (unsigned char*)(scratch + offset); offset += pub_blob_len;
  sig_blob = (unsigned char*)(scratch + offset); offset += sig_len;
  sp = (int8_t *)(scratch + offset); offset += P->N;

  for(T=0; T<TIMES; T++)
  {
    fastrandombytes(scratch, 4*prod + 7*full + priv_blob_len + pub_blob_len + sig_len);
    for(i = 0; i < prod; i++)
    {
      iF[i] = iF[i] % P->N;
      ig[i] = ig[i] % P->N;
    }

    for(i=0; i < P->N; i++)
    {
      iginv[i] = cmod(iginv[i], P->p);
      ih[i] = cmod(ih[i], P->q);
      isig[i] = cmod(isig[i],P->q - (P->p/2));
      sp[i] = cmod(isig[i],P->p);

      /* Prepare isig for packing as psig */
      psig[i] = (isig[i] - sp[i])/P->p + (P->q / (2*P->p));
    }

    rc = pack_private_key(P, iF, ig, iginv, priv_blob_len, priv_blob);
    if(PQNTRU_ERROR == rc) { printf("Private key pack error\n"); return -1; }

    rc = unpack_private_key(P, oF, og, oginv, priv_blob_len, priv_blob);
    if(PQNTRU_ERROR == rc) { printf("Private key unpack error\n"); return -1; }

    rc = pack_public_key(P, ih, pub_blob_len, pub_blob);
    if(PQNTRU_ERROR == rc) { printf("Public key pack error\n"); return -1; }

    rc = unpack_public_key(P, oh, pub_blob_len, pub_blob);
    if(PQNTRU_ERROR == rc) { printf("Public key unpack error\n"); return -1; }

    rc = pack_signature(P, psig, sig_len, sig_blob);
    if(PQNTRU_ERROR == rc) { printf("Signature pack error\n"); return -1; }

    rc = unpack_signature(P, osig, sp, sig_len, sig_blob);
    if(PQNTRU_ERROR == rc) { printf("Signature unpack error\n"); return -1; }

    for(i=0; i<2*(P->d1 + P->d2 + P->d3); i++)
    {
      if(iF[i] != oF[i] || ig[i] != og[i])
      {
        printf("product form keys not equal\n");
        break;
      }
    }

    for(i=0; i<P->N; i++)
    {
      if(cmod(oh[i] - ih[i], P->q) != 0)
      {
        printf("in/out public key not equal @ indx %d :: ih %ld oh %ld \n", i, ih[i], oh[i]);
        printf("\n");
        return -1;
      }

      if(cmod(oginv[i] - iginv[i], P->p) != 0)
      {
        printf("in/out ginv not equal @ indx %d :: iginv %ld oginv %ld\n", i, iginv[i], oginv[i]);
        printf("\n");
        return -1;
      }

      if(cmod(isig[i] - osig[i], P->q) != 0)
      {
        printf("signatures not equal @ indx %d :: isig %ld osig %ld sp %d\n", i, isig[i], osig[i], sp[i]);
        return -1;
      }
    }
  }
  return 0;
}

static int
testKeyGen(PQ_PARAM_SET_ID id)
{
  uint16_t i;
  uint16_t j;

  PQ_PARAM_SET *P;
  size_t privkey_blob_len;
  size_t pubkey_blob_len;

  unsigned char *privkey_blob;
  unsigned char *pubkey_blob;

  unsigned char *scratch;
  size_t         scratch_len;

  int rc;

  if(!(P = pq_get_param_set_by_id(id)))
  {
    return -1;
  }
  fprintf(stderr, "KEYGEN: testing parameter set %s\n", P->name);

  size_t prod = 2*(P->d1 + P->d2 + P->d3)*sizeof(uint16_t);
  size_t full = POLYNOMIAL_BYTES(P);
  scratch_len = 2*prod + 6*full;
  scratch = malloc(scratch_len);
  size_t offset = 0;
  uint16_t *f = (uint16_t*)(scratch); offset += prod;
  uint16_t *g = (uint16_t*)(scratch+offset); offset += prod;
  int64_t *ginv = (int64_t*)(scratch+offset); offset += full;
  int64_t *h = (int64_t*)(scratch+offset); offset += full;
  int64_t *a1 = (int64_t*)(scratch+offset); offset += full;
  int64_t *a2 = (int64_t*)(scratch+offset); offset += 3*full;

  for(i=0; i<TIMES; i++)
  {
    memset(scratch, 0, scratch_len);

    /* Generate a key */
    pq_gen_key(P, &privkey_blob_len, NULL, &pubkey_blob_len, NULL);

    privkey_blob = malloc(privkey_blob_len);
    pubkey_blob = malloc(pubkey_blob_len);

    if(PQNTRU_ERROR == pq_gen_key(P,
               &privkey_blob_len, privkey_blob,
               &pubkey_blob_len, pubkey_blob))
    {
      fprintf(stderr, "\t fail in keygen\n");
    }

    /* Unpack the key */
    rc = unpack_private_key(P, f, g, ginv, privkey_blob_len, privkey_blob);
    if(PQNTRU_ERROR == rc) { printf("Private key unpack error\n"); return -1; }

    rc = unpack_public_key(P, h, pubkey_blob_len, pubkey_blob);
    if(PQNTRU_ERROR == rc) { printf("Public key unpack error\n"); return -1; }

    /* Multiply h by f mod q, should have g in a1 */
    pol_mul_product(a1, h, P->d1, P->d2, P->d3, f, P->N, a2);
    for(j=0; j<P->N; j++)
    {
      a1[j] = cmod(P->p * (h[j] + a1[j]), P->q);
    }

    /* Multiply a1 by g inverse mod p, should have 1 in a2 */
    pol_mul_coefficients(a2, a1, ginv, P->N, P->padded_N, P->p, a2);
    for(j=1; j<P->N; j++)
    {
      if(a2[0] != 1 || a2[j] != 0)
      {
        fprintf(stderr, "\t bad key");
        free(privkey_blob);
        free(pubkey_blob);
        free(scratch);
        return -1;
      }
    }

    free(privkey_blob);
    free(pubkey_blob);
  }
  free(scratch);

  return 0;
}


static int
testSet(PQ_PARAM_SET_ID id)
{
  uint16_t i;

  PQ_PARAM_SET *P;
  size_t privkey_blob_len;
  size_t pubkey_blob_len;
  unsigned char *privkey_blob;
  unsigned char *pubkey_blob;

  unsigned char *sigs;

  uint16_t msg_len = 256;
  unsigned char *msg;

  int result = 0;

  if(!(P = pq_get_param_set_by_id(id)))
  {
    return -1;
  }
  fprintf(stderr, "SUITE: Testing parameter set %s\n", P->name);
  fflush(stderr);

  pq_gen_key(P, &privkey_blob_len, NULL, &pubkey_blob_len, NULL);

  privkey_blob = malloc(TIMES * privkey_blob_len);
  pubkey_blob = malloc(TIMES * pubkey_blob_len);

  msg = malloc(TIMES * msg_len * sizeof(int64_t));
  memset(msg, 0, TIMES*msg_len*sizeof(int64_t));


  for(i=0; i<TIMES; i++)
  {
    if(PQNTRU_ERROR == pq_gen_key(P,
               &privkey_blob_len, privkey_blob + (i*privkey_blob_len),
               &pubkey_blob_len, pubkey_blob + (i*pubkey_blob_len)))
    {
      result = -1;
      fprintf(stderr, "\t fail in keygen\n");
      goto exit_kg;
    }
  }

  size_t packed_sig_len;
  pq_sign(&packed_sig_len, NULL, privkey_blob_len, privkey_blob, pubkey_blob_len, pubkey_blob, 0, NULL);
  sigs = malloc(TIMES * packed_sig_len);

  for(i=0; i<TIMES; i++)
  {
    fastrandombytes(msg+(i*msg_len), msg_len);
    if(PQNTRU_ERROR == pq_sign( &packed_sig_len, sigs + (i*packed_sig_len),
                               privkey_blob_len, privkey_blob + (i*privkey_blob_len),
                                pubkey_blob_len, pubkey_blob + (i*pubkey_blob_len),
                                        msg_len, msg + (i*msg_len)))
    {
      result = -1;
      fprintf(stderr, "\t fail in sign\n");
      goto exit;
    }
  }

  for(i=0; i<TIMES; i++)
  {
    if(PQNTRU_ERROR == pq_verify( packed_sig_len, sigs + (i*packed_sig_len),
                                 pubkey_blob_len, pubkey_blob + (i*pubkey_blob_len),
                                         msg_len, msg + (i*msg_len)))
    {
      result = -1;
      fprintf(stderr, "\t fail in verify\n");
      goto exit;
    }
  }

exit:
  free(sigs);
exit_kg:
  free(msg);
  free(privkey_blob);
  free(pubkey_blob);
  return result;
}


int
main(int argc, char **argv)
{
  uint16_t i;
  int result = 0;
  PQ_PARAM_SET_ID plist[] =
  {
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
    result = 0;
    result |= testPack(plist[i]);
    result |= testKeyGen(plist[i]);
    result |= testSet(plist[i]);

    if(result == 0) fprintf(stderr, "\t good\n");
    else fprintf(stderr, "\t fail\n");

  }

  rng_cleanup();

  exit(EXIT_SUCCESS);
}
