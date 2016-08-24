/*
    Copyright (C) 2016 William Hart

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gmp.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_mpoly.h"

char *
_fmpz_mpoly_get_str_pretty1(fmpz * poly, ulong * exps, slong len,
                              char ** x, slong bits, slong n, int deg, int rev)
{
   char * str;
   slong i, j, bound, off;
   ulong * degs;
   ulong v;
   int first;

   if (len == 0)
   {
      str = flint_malloc(2);
      str[0] = '0';
      str[1] = '\0';
      return str;
   }

   bound = 1;
   for (i = 0; i < len; i++) /* for each term */
      bound += fmpz_sizeinbase(poly + i, 10) + 1;
      
   degs = _fmpz_mpoly_max_degrees1(exps, len, bits, n, deg, rev);

   if (deg) /* skip degree exponent */
      degs += 1;

   for (i = 0; i < n - deg; i++) /* for each max degree */
   {
      ulong d10 = 1;
      slong b = 0;

      while (d10 <= degs[i])
      {
         d10 *= 10;
         b++;
      }

      bound += (b + strlen(x[i]) + 3)*len;
   }

   str = flint_malloc(bound);
   off = 0;
   
   for (i = len - 1; i >= 0; i--)
   {
      if (fmpz_sgn(poly + i) > 0 && i != len - 1)
         str[off++] = '+';
      if (poly[i] == WORD(-1))
            str[off++] = '-';
      if (poly[i] != WORD(1) && poly[i] != WORD(-1))
      {
         if (!COEFF_IS_MPZ(poly[i]))
            off += flint_sprintf(str + off, "%wd", poly[i]);
         else
            off += gmp_sprintf(str + off, "%Zd", COEFF_TO_PTR(poly[i]));
      }

      v = exps[i];
      
      _exp_get_degrees1(degs, v, bits, n, deg, rev);

      first = 1;

      for (j = 0; j < n - deg; j++)
      {
          if (degs[j] > 1)
          {
             if (!first || (poly[i] != WORD(1) && poly[i] != WORD(-1)))
                off += flint_sprintf(str + off, "*");
             off += flint_sprintf(str + off, "%s^%wd", x[j], degs[j]);
             first = 0;
          }
          if (degs[j] == 1)
          {
             if (!first || (poly[i] != WORD(1) && poly[i] != WORD(-1)))
                off += flint_sprintf(str + off, "*");
             off += flint_sprintf(str + off, "%s", x[j]);
             first = 0;
          }
      }

      if (v == 0 && (poly[i] == WORD(1) || poly[i] == WORD(-1)))
         off += flint_sprintf(str + off, "1"); 
   }     
   
   if (deg)
      degs -= 1;

   flint_free(degs);

   return str;
}

char *
fmpz_mpoly_get_str_pretty(fmpz_mpoly_t poly, char ** x, fmpz_mpoly_ctx_t ctx)
{
   int deg, rev;

   if (ctx->N == 1)
   {
      degrev_from_ord(deg, rev, ctx->ord);

      return _fmpz_mpoly_get_str_pretty1(poly->coeffs, poly->exps,
                                 poly->length, x, ctx->bits, ctx->n, deg, rev);
   } else
      flint_throw(FLINT_ERROR, "Not implemented yet");

   return 0; 
}
