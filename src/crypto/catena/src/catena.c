#include <string.h>
#include <stdio.h>
#if defined(__APPLE__)
#include <libkern/OSByteOrder.h>
#define le32toh OSSwapLittleToHostInt32
#define htole32 OSSwapHostToLittleInt32
#define bswap_32 OSSwapInt32
#else
#include <byteswap.h>
#endif

#include <stdlib.h>
#include <sys/param.h>
#define __STDC_CONSTANT_MACROS
#include <stdint.h>

#include "catena.h"
#include "catena-helpers.h"
#include "cathash.h"

#if __BYTE_ORDER == __LITTLE_ENDIAN
  #define TO_LITTLE_ENDIAN_64(n) (n)
  #define TO_LITTLE_ENDIAN_32(n) (n)
#elif __BYTE_ORDER == __BIG_ENDIAN
  #define TO_LITTLE_ENDIAN_64(n) bswap_64(n)
  #define TO_LITTLE_ENDIAN_32(n) bswap_32(n)
#else
  #warning "byte order couldn't be detected. This affects key generation and keyed hashing"
  #define TO_LITTLE_ENDIAN_64(n) (n)
  #define TO_LITTLE_ENDIAN_32(n) (n)
#endif

/* Ensure that a pointer passed to the PHS interface stays const
 */
#ifdef OVERWRITE
  #define MAYBECONST
#else
  #define MAYBECONST const
#endif

/***************************************************/

void print_hlex(uint8_t *key, int len)
{
  int i;
  for(i=0; i< len; i++) printf("%02x",key[i]);  puts("");
}

int __Catena(const uint8_t *pwd,   const uint32_t pwdlen,
	     const uint8_t *salt,  const uint8_t  saltlen,
	     const uint8_t *data,  const uint32_t datalen,
	     const uint8_t lambda, const uint8_t  min_garlic,
	     const uint8_t garlic, const uint8_t  hashlen,
	     const uint8_t client, const uint8_t  tweak_id, uint8_t *hash)
{
  uint8_t x[H_LEN];
  uint8_t hv[H_LEN];
  uint8_t t[4];
  uint8_t c;

  if((hashlen > H_LEN) || (garlic > 63) || (min_garlic > garlic) ||
    (lambda == 0) || (min_garlic == 0)){
     return -1;
  }

  /*Compute H(V)*/
  __Hash1(VERSION_ID, strlen((char*)VERSION_ID), hv);

  /* Compute Tweak */
  t[0] = tweak_id;
  t[1] = lambda;
  t[2] = hashlen;
  t[3] = saltlen;

  /* Compute H(AD) */
  __Hash1((uint8_t *) data, datalen,x);

  /* Compute the initial value to hash  */
  __Hash5(hv, H_LEN, t, 4, x, H_LEN, pwd,  pwdlen, salt, saltlen, x);


  Flap(x, lambda, (min_garlic+1)/2, salt, saltlen, x);
  //print_hlex(x, 32);

  for(c=min_garlic; c <= garlic; c++)
  {
      Flap(x, lambda, c, salt, saltlen, x);
      if( (c==garlic) && (client == CLIENT))
      {
        memcpy(hash, x, H_LEN);
        return 0;
      }
      __Hash2(&c,1, x,H_LEN, x);
      memset(x+hashlen, 0, H_LEN-hashlen);
  }
  memcpy(hash, x, hashlen);

  return 0;
}


/***************************************************/

int Catena(uint8_t *pwd,   const uint32_t pwdlen,
	   const uint8_t *salt,  const uint8_t  saltlen,
	   const uint8_t *data,  const uint32_t datalen,
	   const uint8_t lambda, const uint8_t  min_garlic,
	   const uint8_t garlic, const uint8_t  hashlen,  uint8_t *hash)
{
  return __Catena(pwd, pwdlen, salt, saltlen, data, datalen,
		  lambda, min_garlic, garlic,
		  hashlen,  REGULAR, PASSWORD_HASHING_MODE, hash);
}

/***************************************************/

void RetHash(const char *input, char *output)
{
  __Catena(input, strlen(input), (const uint8_t *) "", 0, (const uint8_t *) "", 0, 1, 16, 16, 32,
     REGULAR, PASSWORD_HASHING_MODE, output);
  return;
}
