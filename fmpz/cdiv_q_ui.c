/*
    Copyright (C) 2009 William Hart
    Copyright (C) 2010 Sebastian Pancratz

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "flint.h"
#include "fmpz.h"

void
fmpz_cdiv_q_ui(fmpz_t f, const fmpz_t g, ulong h)
{
    fmpz c1 = *g;
    ulong c2 = h;

    if (h == 0)
    {
        flint_printf("Exception: division by zero in fmpz_cdiv_q_ui\n");
        flint_abort();
    }

    if (!COEFF_IS_MPZ(c1))      /* g is small */
    {
        if (c1 > 0)
        {
            ulong q = c1 / c2;
            ulong r = c1 - c2 * q;

            if (r)
                ++q;

            fmpz_set_ui(f, q);
        }
        else
        {
            fmpz_set_si(f, - (((ulong) -c1) / c2));
        }
    }
    else                        /* g is large */
    {
        __mpz_struct * mf = _fmpz_promote(f);

        flint_mpz_cdiv_q_ui(mf, COEFF_TO_PTR(c1), c2);
        _fmpz_demote_val(f);    /* division by h may result in small value */
    }
}
