/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2012 Lina Kulakova

******************************************************************************/

#include <stdlib.h>
#include "fmpz_mod_poly.h"
#include "ulong_extras.h"

int
main(void)
{
    int iter;
    FLINT_TEST_INIT(state);
    

    flint_printf("factor_equal_deg_prob....");
    fflush(stdout);

    for (iter = 0; iter < 20 * flint_test_multiplier(); iter++)
    {
        fmpz_mod_poly_t poly1, poly2, q, r;
        fmpz_t modulus;
        slong length;
        int i, num;

        fmpz_init_set_ui(modulus, n_randtest_prime(state, 0));

        fmpz_mod_poly_init(q, modulus);
        fmpz_mod_poly_init(r, modulus);
        fmpz_mod_poly_init(poly1, modulus);
        fmpz_mod_poly_init(poly2, modulus);

        length = n_randint(state, 10) + 2;
        do
        {
            fmpz_mod_poly_randtest(poly1, state, length);
            if (poly1->length)
                fmpz_mod_poly_make_monic(poly1, poly1);
        }
        while ((poly1->length < 2) || (!fmpz_mod_poly_is_irreducible(poly1)));

        num = n_randint(state, 5) + 1;

        for (i = 0; i < num; i++)
        {
            do
            {
                fmpz_mod_poly_randtest(poly2, state, length);
                if (poly2->length)
                    fmpz_mod_poly_make_monic(poly2, poly2);
            }
            while ((poly2->length < 2)
                   || (!fmpz_mod_poly_is_irreducible(poly2)));

            fmpz_mod_poly_mul(poly1, poly1, poly2);
        }

        while (!fmpz_mod_poly_factor_equal_deg_prob
               (poly2, state, poly1, length - 1))
        {
        };
        fmpz_mod_poly_divrem(q, r, poly1, poly2);
        if (!fmpz_mod_poly_is_zero(r))
        {
            flint_printf("FAIL:\n");
            flint_printf("Error: factor does not divide original polynomial\n");
            flint_printf("factor:\n");
            fmpz_mod_poly_print(poly2);
            flint_printf("\n\n");
            flint_printf("polynomial:\n");
            fmpz_mod_poly_print(poly1);
            flint_printf("\n\n");
            abort();
        }

        fmpz_clear(modulus);
        fmpz_mod_poly_clear(q);
        fmpz_mod_poly_clear(r);
        fmpz_mod_poly_clear(poly1);
        fmpz_mod_poly_clear(poly2);
    }

    FLINT_TEST_CLEANUP(state);
    
    flint_printf("PASS\n");
    return 0;
}
