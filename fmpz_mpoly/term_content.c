/*
    Copyright (C) 2017 Daniel Schultz

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <gmp.h>
#include <stdlib.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_mpoly.h"
#include "assert.h"


void fmpz_mpoly_term_content(fmpz_mpoly_t poly1, const fmpz_mpoly_t poly2,
                                                    const fmpz_mpoly_ctx_t ctx)
{
    int deg, rev;
    slong bits, nfields;
    slong i, N;
    ulong * pmin, * exps, mask;
    fmpz_t igcd;
    TMP_INIT;

    if (poly2->length == 0)
    {
        fmpz_mpoly_zero(poly1, ctx);
        return;
    }

    bits = poly2->bits;
    nfields = ctx->n;
    degrev_from_ord(deg, rev, ctx->ord);

    TMP_START;

    mask = 0;
    for (i = 0; i < FLINT_BITS/bits; i++)
        mask = (mask << bits) + (UWORD(1) << (bits - 1));

    N = words_per_exp(nfields, bits);
    pmin = (ulong *) TMP_ALLOC(N*sizeof(ulong));
    exps = (ulong *) TMP_ALLOC(ctx->n*sizeof(ulong));

    fmpz_init(igcd);

    mpoly_monomial_set(pmin, poly2->exps + 0*N, N);
    fmpz_set(igcd, poly2->coeffs + 0);
    for (i = 1; i < poly2->length; i++)
    {
        mpoly_monomial_min(pmin, pmin, poly2->exps + N*i, bits, N, mask);
        fmpz_gcd(igcd, igcd, poly2->coeffs + i);
    }
    mpoly_get_monomial(exps, pmin, bits, ctx->minfo);

    fmpz_mpoly_fit_length(poly1, 1, ctx);
    fmpz_mpoly_fit_bits(poly1, bits, ctx);
    poly1->bits = bits;
    mpoly_set_monomial(poly1->exps + N*0, exps, bits, ctx->n, deg, rev);
    fmpz_set(poly1->coeffs + 0, igcd);
    _fmpz_mpoly_set_length(poly1, 1, ctx);

    fmpz_clear(igcd);
}

