/*
    Copyright (C) 2013 Fredrik Johansson
    Copyright (C) 2013 William Hart
    Copyright (C) 2011 Sebastian Pancratz

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

#include "fmpz_poly_q.h"
#include "ulong_extras.h"

int
main(void)
{
    int i, result;
    FLINT_TEST_INIT(state);

    flint_printf("add... ");
    fflush(stdout);

    

    /* Check aliasing of a and c */
    for (i = 0; i < 10 * flint_test_multiplier(); i++)
    {
        fmpz_poly_q_t a, b, c;

        fmpz_poly_q_init(a);
        fmpz_poly_q_init(b);
        fmpz_poly_q_init(c);
        fmpz_poly_q_randtest(a, state, n_randint(state, 50), 50, n_randint(state, 50), 50);
        fmpz_poly_q_randtest(b, state, n_randint(state, 50), 50, n_randint(state, 50), 50);

        fmpz_poly_q_add(c, a, b);
        fmpz_poly_q_add(a, a, b);

        result = (fmpz_poly_q_equal(a, c));
        if (!result)
        {
            flint_printf("FAIL:\n");
            fmpz_poly_q_print(a), flint_printf("\n\n");
            fmpz_poly_q_print(b), flint_printf("\n\n");
            fmpz_poly_q_print(c), flint_printf("\n\n");
            fflush(stdout);
            flint_abort();
        }

        fmpz_poly_q_clear(a);
        fmpz_poly_q_clear(b);
        fmpz_poly_q_clear(c);
    }

    /* Check aliasing of b and c */
    for (i = 0; i < 10 * flint_test_multiplier(); i++)
    {
        fmpz_poly_q_t a, b, c;

        fmpz_poly_q_init(a);
        fmpz_poly_q_init(b);
        fmpz_poly_q_init(c);
        fmpz_poly_q_randtest(a, state, n_randint(state, 50), 50, n_randint(state, 50), 50);
        fmpz_poly_q_randtest(b, state, n_randint(state, 50), 50, n_randint(state, 50), 50);

        fmpz_poly_q_add(c, a, b);
        fmpz_poly_q_add(b, a, b);

        result = fmpz_poly_q_equal(b, c);
        if (!result)
        {
            flint_printf("FAIL:\n");
            fmpz_poly_q_print(a), flint_printf("\n\n");
            fmpz_poly_q_print(b), flint_printf("\n\n");
            fmpz_poly_q_print(c), flint_printf("\n\n");
            fflush(stdout);
            flint_abort();
        }

        fmpz_poly_q_clear(a);
        fmpz_poly_q_clear(b);
        fmpz_poly_q_clear(c);
    }

    /* Check commutativity */
    for (i = 0; i < 10 * flint_test_multiplier(); i++)
    {
        fmpz_poly_q_t a, b, c, d;

        fmpz_poly_q_init(a);
        fmpz_poly_q_init(b);
        fmpz_poly_q_init(c);
        fmpz_poly_q_init(d);
        fmpz_poly_q_randtest(a, state, n_randint(state, 50), 50, n_randint(state, 50), 50);
        fmpz_poly_q_randtest(b, state, n_randint(state, 50), 50, n_randint(state, 50), 50);

        fmpz_poly_q_add(c, a, b);
        fmpz_poly_q_add(d, b, a);

        result = fmpz_poly_q_equal(c, d) && fmpz_poly_q_is_canonical(c) 
                                         && fmpz_poly_q_is_canonical(d);
        if (!result)
        {
            flint_printf("FAIL:\n");
            fmpz_poly_q_print(a), flint_printf("\n\n");
            fmpz_poly_q_print(b), flint_printf("\n\n");
            fmpz_poly_q_print(c), flint_printf("\n\n");
            fmpz_poly_q_print(d), flint_printf("\n\n");
            fflush(stdout);
            flint_abort();
        }

        fmpz_poly_q_clear(a);
        fmpz_poly_q_clear(b);
        fmpz_poly_q_clear(c);
        fmpz_poly_q_clear(d);
    }

    FLINT_TEST_CLEANUP(state);
    
    flint_printf("PASS\n");
    return EXIT_SUCCESS;
}
