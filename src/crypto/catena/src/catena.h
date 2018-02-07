#ifndef _CATENA_H_
#define _CATENA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#if defined(__APPLE__)
#include <libkern/OSByteOrder.h>
#include <machine/endian.h>
#define htole32 OSSwapHostToLittleInt32
#define le32toh OSSwapLittleToHostInt32
#else
#include <endian.h>
#endif

/* The default unit for all length values are bytes */

/* Recommended default values */
#define H_LEN      32
#define KEY_LEN    8
/* Default values depending on instance*/
extern const uint8_t LAMBDA;
extern const uint8_t GARLIC;
extern const uint8_t MIN_GARLIC;
extern const uint8_t VERSION_ID[];

/* Modes  */
#define PASSWORD_HASHING_MODE 1
#define KEY_DERIVATION_MODE   0
#define REGULAR 1
#define CLIENT 0

/*Flap function of Catena. Possible instantiations:
*	-Catena-BRG using a Bit-Reversal Graph
* 	-Catena-DBG using a Double-Butterfly Graph
*/
 void Flap(const uint8_t x[H_LEN], const uint8_t lambda, const uint8_t garlic,
	const uint8_t *salt, const uint8_t saltlen, uint8_t h[H_LEN]);

/* Returns -1 if an an error occurred, otherwise 0. */
 int Catena(uint8_t *pwd,   const uint32_t pwdlen,
	   const uint8_t *salt,  const uint8_t  saltlen,
	   const uint8_t *data,  const uint32_t datalen,
	   const uint8_t lambda, const uint8_t  min_garlic,
	   const uint8_t garlic, const uint8_t hashlen,  uint8_t *hash);

 void RetHash(const char *input, char *output);

#ifdef __cplusplus
}
#endif

#endif
