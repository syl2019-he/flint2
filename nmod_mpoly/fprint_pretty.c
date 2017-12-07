/*
    Copyright (C) 2017 Daniel Schultz

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
#include "nmod_mpoly.h"

int
_nmod_mpoly_fprint_pretty(FILE * file, const mp_limb_t * coeff, const ulong * exp,
                        slong len, const char ** x_in,  slong bits,
                                const mpoly_ctx_t mctx, const nmodf_ctx_t fctx)
{
    slong i, j, N;
    ulong * degs;
    int r = 0, first;
    char ** x = (char **) x_in;

    TMP_INIT;

    if (len == 0)
    {
        r = fputc('0', file);
        r = (r != EOF) ? 1 : EOF;
        return r;
    }

    N = mpoly_words_per_exp(bits, mctx);

    TMP_START;

    if (x == NULL)
    {
        x = (char **) TMP_ALLOC(mctx->nvars*sizeof(char *));
        for (i = 0; i < mctx->nvars; i++)
        {
            x[i] = (char *) TMP_ALLOC(((FLINT_BITS+4)/3)*sizeof(char));
            flint_sprintf(x[i], "x%wd", i + 1);
        }
    }

    degs = (ulong *) TMP_ALLOC(mctx->nvars*sizeof(ulong));
   
    for (i = 0; i < len; i++)
    {
        if (i > 0)
        {
            r = fputc('+', file);
            r = (r != EOF) ? 1 : EOF;
            if (r <= 0) goto done;
        }

        first = (coeff[i] == 1);
        if (!first)
        {
            r = flint_fprintf(file, "%wd", coeff[i]);
            if (r <= 0) goto done;
        }

        mpoly_get_monomial(degs, exp + N*i, bits, mctx);

        for (j = 0; j < mctx->nvars; j++)
        {
            if (degs[j] == 0)
                continue;

            if (!first)
            {
                r = fputc('*', file);
                r = (r != EOF) ? 1 : EOF;
                if (r <= 0) goto done;
            }
            if (degs[j] > 1)
                r = flint_fprintf(file, "%s^%wd", x[j], degs[j]);
            else
                r = flint_fprintf(file, "%s", x[j]);
            if (r <= 0) goto done;
            
            first = 0;
        }

        if (first)
        {
            r = flint_fprintf(file, "1");
            if (r <= 0) goto done;
        }
   }
   
done:
   TMP_END;
   return r;
}

int
nmod_mpoly_fprint_pretty(FILE * file, const nmod_mpoly_t poly,
                                   const char ** x, const nmod_mpoly_ctx_t ctx)
{
   return _nmod_mpoly_fprint_pretty(file, poly->coeffs, poly->exps,
                         poly->length, x, poly->bits, ctx->minfo, ctx->ffinfo);
}
