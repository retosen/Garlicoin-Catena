#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include "catena.h"
#include <string>

#define SALT_LEN 8

void print_hex(uint8_t *key, int len)
{
  int i;
  for(i=0; i< len; i++) printf("%02x",key[i]);  puts("");
}



int main()
{
  const size_t hashlen = 32;
  const uint8_t salt[SALT_LEN]=
    {0x01, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};
  while (true)
  {
    std::cout << "INPUT PASSWORD";
    uint8_t* password = malloc(9);
    strncpy((char*)password, "WHATTTTH", 9);
    const char *data     = "This is a header";
    const uint8_t lambda = LAMBDA;
    const uint8_t min_garlic = GARLIC;
    const uint8_t garlic = GARLIC;
    uint8_t hash1[H_LEN];
    memset(hash1,0,H_LEN);

    Catena(password, strlen((char *)password) , salt, SALT_LEN,
  	 (uint8_t *) data, strlen(data), lambda, min_garlic, garlic,
  	 hashlen, hash1);
    print_hex(hash1, hashlen);
  }
  uint8_t* password = malloc(9);
  strncpy((char*)password, "WHATTTTH", 9);
  const char *data     = "This is a header";
  const uint8_t lambda = LAMBDA;
  const uint8_t min_garlic = GARLIC;
  const uint8_t garlic = GARLIC;
  uint8_t hash1[H_LEN];
  memset(hash1,0,H_LEN);

  Catena(password, strlen((char *)password) , salt, SALT_LEN,
	 (uint8_t *) data, strlen(data), lambda, min_garlic, garlic,
	 hashlen, hash1);
  print_hex(hash1, hashlen);

  return 0;
}
