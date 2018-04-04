/*
 * CPQREF/params.c
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
#include <string.h>

#include "params.h"


static PQ_PARAM_SET pqParamSets[] = {

    {
      XXX_20140508_401,    /* parameter set id */
      "xxx-20140508-401",  /* human readable name */
      {0xff, 0xff, 0xff},  /* OID */
      9,                   /* bitlength of N */
      18,                  /* bitlength of q */
      401,                 /* ring degree */
      3,                   /* message space prime */
      1<<18,               /* ring modulus */
      240,                 /* max norm of f*a convolution */
      80,                  /* max norm of g*a convolution */
      (1<<17) - 240,       /* q/2 - B_s */
      (1<<17) - 80,        /* q/2 - B_t */
      8,                   /* Product form +1/-1 counts */
      8,
      6,
      416,                 /* # Polynomial coefficients for Karatsuba */
    },

    {
      XXX_20140508_439,    /* parameter set id */
      "xxx-20140508-439",  /* human readable name */
      {0xff, 0xff, 0xfe},  /* OID */
      9,                   /* bitlength of N */
      19,                  /* bitlength of q */
      439,                 /* ring degree */
      3,                   /* message space prime */
      1<<19,               /* ring modulus */
      264,                 /* max norm of f*a convolution */
      88,                  /* max norm of g*a convolution */
      (1<<18) - 264,       /* q/2 - B_s */
      (1<<18) - 88,        /* q/2 - B_t */
      9,                   /* Product form +1/-1 counts */
      8,
      5,
      448,                 /* # Polynomial coefficients for Karatsuba */
    },

    {
      XXX_20140508_593,    /* parameter set id */
      "xxx-20140508-593",  /* human readable name */
      {0xff, 0xff, 0xfd},  /* OID */
      10,                  /* bitlength of N */
      19,                  /* bitlength of q */
      593,                 /* ring degree */
      3,                   /* message space prime */
      1<<19,               /* ring modulus */
      300,                 /* max norm of f*a convolution */
      100,                 /* max norm of g*a convolution */
      (1<<18) - 300,       /* q/2 - B_s */
      (1<<18) - 100,       /* q/2 - B_t */
      10,                  /* Product form +1/-1 counts */
      10,
       8,
      608,                 /* # Polynomial coefficients for Karatsuba */
    },

    {
      XXX_20140508_743,    /* parameter set id */
      "xxx-20140508-743",  /* human readable name */
      {0xff, 0xff, 0xfc},  /* OID */
      10,                  /* bitlength of N */
      20,                  /* bitlength of q */
      743,                 /* ring degree */
      3,                   /* message space prime */
      1<<20,               /* ring modulus */
      336,                 /* max norm of f*a convolution */
      112,                 /* max norm of g*a convolution */
      (1<<19) - 336,       /* q/2 - B_s */
      (1<<19) - 112,       /* q/2 - B_t */
      11,                  /* Product form +1/-1 counts */
      11,
      15,
      768,                 /* # Polynomial coefficients for Karatsuba */
    },


    {
      XXX_20151024_401,    /* parameter set id */
      "xxx-20151024-401",  /* human readable name */
      {0xff, 0xff, 0xfb},  /* OID */
      9,                   /* bitlength of N */
      15,                  /* bitlength of q */
      401,                 /* ring degree */
      3,                   /* message space prime */
      1<<15,               /* ring modulus */
      138,                 /* max norm of f*a convolution */
      46,                  /* max norm of g*a convolution */
      (1<<14)-138,         /* q/2 - B_s */
      (1<<14)-46,          /* q/2 - B_t */
      8,                   /* Product form +1/-1 counts */
      8,
      6,
      416,                 /* # Polynomial coefficients for Karatsuba */
    },

    {
      XXX_20151024_443,    /* parameter set id */
      "xxx-20151024-443",  /* human readable name */
      {0xff, 0xff, 0xfa},  /* OID */
      9,                   /* bitlength of N */
      16,                  /* bitlength of q */
      443,                 /* ring degree */
      3,                   /* message space prime */
      1<<16,               /* ring modulus */
      138,                 /* max norm of f*a convolution */
      46,                  /* max norm of g*a convolution */
      (1<<15)-138,         /* q/2 - B_s */
      (1<<15)-46,          /* q/2 - B_t */
      9,                   /* Product form +1/-1 counts */
      8,
      5,
      448,                 /* # Polynomial coefficients for Karatsuba */
    },

    {
      XXX_20151024_563,    /* parameter set id */
      "xxx-20151024-563",  /* human readable name */
      {0xff, 0xff, 0xf9},  /* OID */
      10,                  /* bitlength of N */
      16,                  /* bitlength of q */
      563,                 /* ring degree */
      3,                   /* message space prime */
      1<<16,               /* ring modulus */
      174,                 /* max norm of f*a convolution */
      58,                  /* max norm of g*a convolution */
      (1<<15)-174,         /* q/2 - B_s */
      (1<<15)-58,          /* q/2 - B_t */
      10,                  /* Product form +1/-1 counts */
      9,
      8,
      592,                 /* # Polynomial coefficients for Karatsuba */
    },

#if 0
    /* Test parameter set that is not formally transcript secure */
    {
      XXX_20151024_509,    /* parameter set id */
      "xxx-20151024-509",  /* human readable name */
      {0xff, 0xff, 0xf8},  /* OID */
      9,                  /* bitlength of N */
      14,                  /* bitlength of q */
      509,                 /* ring degree */
      3,                   /* message space prime */
      1<<14,               /* ring modulus */
      10000,                 /* max norm of f*a convolution */
      10000,                  /* max norm of g*a convolution */
      (1<<13)-1,       /* q/2 - B_s */
      (1<<13)-1,        /* q/2 - B_t */
      9,                  /* Product form +1/-1 counts */
      9,
      8,
      512,                 /* # Polynomial coefficients for Karatsuba */
    },
#endif

    {
      XXX_20151024_743,    /* parameter set id */
      "xxx-20151024-743",  /* human readable name */
      {0xff, 0xff, 0xf7},  /* OID */
      10,                  /* bitlength of N */
      17,                  /* bitlength of q */
      743,                 /* ring degree */
      3,                   /* message space prime */
      1<<17,               /* ring modulus */
      186,                 /* max norm of f*a convolution */
      62,                  /* max norm of g*a convolution */
      (1<<16)-186,         /* q/2 - B_s */
      (1<<16)-62,          /* q/2 - B_t */
      11,                  /* Product form +1/-1 counts */
      11,
      6,
      752,                 /* # Polynomial coefficients for Karatsuba */
    },

    {
      XXX_20151024_907,    /* parameter set id */
      "xxx-20151024-907",  /* human readable name */
      {0xff, 0xff, 0xf6},  /* OID */
      10,                  /* bitlength of N */
      17,                  /* bitlength of q */
      907,                 /* ring degree */
      3,                   /* message space prime */
      1<<17,               /* ring modulus */
      225,                 /* max norm of f*a convolution */
      75,                  /* max norm of g*a convolution */
      (1<<16)-225,         /* q/2 - B_s */
      (1<<16)-75,          /* q/2 - B_t */
      13,                  /* Product form +1/-1 counts */
      12,
      7,
      912,                 /* # Polynomial coefficients for Karatsuba */
    },

/*
 * Orig:
 * [401, 2^18, 8, 8, 6, 80, 65, 89, 145] @ 65
 * [439, 2^19, 9, 8, 5, 88, 70, 95, 147] @ 70
 * [593, 2^19, 10, 10, 8, 100, 110, 147, 185] @ 110
 * [743, 2^20, 11, 11, 15, 112, 146, 193] @ 146
 *
 *
 * [N, q, d1, d2, d3, Bt, lambda, K, directMITM]
 * [401, 2^15, 8, 8, 6, 46, 82, 110, 145] @ 82
 * [443, 2^16, 9, 8, 5, 46, 88, 117, 147] @ 88
 * [563, 2^16, 10, 9, 8, 52, 126, 166, 185] @ 126
 * [587, 2^16, 10, 10, 6, 55, 134, 176, 180] @ 134
 * [743, 2^17, 11, 11, 6, 62, 179, 233, 201] @ 179
 * [907, 2^17, 13, 12, 7, 71, 235, 312, 235] @ 235
 * [1019, 2^18, 13, 13, 7, 75, 246, 347, 246] @ 269
 */
};



static int numParamSets = sizeof(pqParamSets)/sizeof(PQ_PARAM_SET);

PQ_PARAM_SET *
pq_get_param_set_by_id(PQ_PARAM_SET_ID id)
{
  int i;

  for(i=0; i<numParamSets; i++)
  {
    if(pqParamSets[i].id == id)
    {
      return (pqParamSets + i);
    }
  }
  return NULL;
}

PQ_PARAM_SET *
pq_get_param_set_by_oid(const uint8_t *oid)
{
  int i;

  for(i=0; i<numParamSets; i++)
  {
    if (0 == memcmp(pqParamSets[i].OID, oid, 3))
    {
      return (pqParamSets + i);
    }
  }
  return NULL;
}

