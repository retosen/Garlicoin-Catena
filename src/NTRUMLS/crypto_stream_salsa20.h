#ifndef CRYPTO_STREAM_H
#define CRYPTO_STREAM_H

#ifdef __cplusplus
extern "C" {
#endif

#define crypto_stream_salsa20_KEYBYTES 32
#define crypto_stream_salsa20_NONCEBYTES 8

int crypto_stream(
        unsigned char *c,unsigned long long clen,
  const unsigned char *n,
  const unsigned char *k
);

//int crypto_stream_salsa20_amd64_xmm6(
//        unsigned char *c,unsigned long long clen,
//  const unsigned char *n,
//  const unsigned char *k
//);

#define crypto_stream_salsa20 crypto_stream

#ifdef __cplusplus
}
#endif

#endif
