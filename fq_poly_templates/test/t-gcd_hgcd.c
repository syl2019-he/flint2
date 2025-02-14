/*
    Copyright (C) 2011 William Hart
    Copyright (C) 2011 Sebastian Pancratz
    Copyright (C) 2013 Mike Hansen

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#ifdef T

#include "templates.h"

int
main(void)
{
    int i, result;
    FLINT_TEST_INIT(state);


    flint_printf("gcd_hgcd....");
    fflush(stdout);

    /* 
       Find coprime polys, multiply by another poly 
       and check the GCD is that poly 
     */
    for (i = 0; i < 10 * flint_test_multiplier(); i++)
    {
        TEMPLATE(T, poly_t) a, b, c, g;
        TEMPLATE(T, ctx_t) ctx;

        TEMPLATE(T, ctx_randtest) (ctx, state);

        TEMPLATE(T, poly_init) (a, ctx);
        TEMPLATE(T, poly_init) (b, ctx);
        TEMPLATE(T, poly_init) (c, ctx);
        TEMPLATE(T, poly_init) (g, ctx);

        do
        {
            TEMPLATE(T, poly_randtest) (a, state, n_randint(state, 300), ctx);
            TEMPLATE(T, poly_randtest) (b, state, n_randint(state, 300), ctx);
            TEMPLATE(T, poly_gcd_hgcd) (g, a, b, ctx);
        } while (g->length != 1);

        do
        {
            TEMPLATE(T, poly_randtest) (c, state, n_randint(state, 300), ctx);
        } while (c->length < 2);
        TEMPLATE(T, poly_make_monic) (c, c, ctx);

        TEMPLATE(T, poly_mul) (a, a, c, ctx);
        TEMPLATE(T, poly_mul) (b, b, c, ctx);

        TEMPLATE(T, poly_gcd_hgcd) (g, a, b, ctx);

        result = (TEMPLATE(T, poly_equal) (g, c, ctx));
        if (!result)
        {
            flint_printf("FAIL:\n");
            TEMPLATE(T, poly_print_pretty) (a, "x", ctx), flint_printf("\n\n");
            TEMPLATE(T, poly_print_pretty) (b, "x", ctx), flint_printf("\n\n");
            TEMPLATE(T, poly_print_pretty) (c, "x", ctx), flint_printf("\n\n");
            TEMPLATE(T, poly_print_pretty) (g, "x", ctx), flint_printf("\n\n");
            fflush(stdout);
            flint_abort();
        }

        TEMPLATE(T, poly_clear) (a, ctx);
        TEMPLATE(T, poly_clear) (b, ctx);
        TEMPLATE(T, poly_clear) (c, ctx);
        TEMPLATE(T, poly_clear) (g, ctx);

        TEMPLATE(T, ctx_clear) (ctx);
    }

    /* Check aliasing of a and g */
    for (i = 0; i < 10 * flint_test_multiplier(); i++)
    {
        TEMPLATE(T, ctx_t) ctx;
        TEMPLATE(T, poly_t) a, b, g;

        TEMPLATE(T, ctx_randtest) (ctx, state);

        TEMPLATE(T, poly_init) (a, ctx);
        TEMPLATE(T, poly_init) (b, ctx);
        TEMPLATE(T, poly_init) (g, ctx);
        TEMPLATE(T, poly_randtest) (a, state, n_randint(state, 300), ctx);
        TEMPLATE(T, poly_randtest) (b, state, n_randint(state, 300), ctx);

        TEMPLATE(T, poly_gcd_hgcd) (g, a, b, ctx);
        TEMPLATE(T, poly_gcd_hgcd) (a, a, b, ctx);

        result = (TEMPLATE(T, poly_equal) (a, g, ctx));
        if (!result)
        {
            flint_printf("FAIL:\n");
            TEMPLATE(T, poly_print_pretty) (a, "x", ctx), flint_printf("\n\n");
            TEMPLATE(T, poly_print_pretty) (b, "x", ctx), flint_printf("\n\n");
            TEMPLATE(T, poly_print_pretty) (g, "x", ctx), flint_printf("\n\n");
            fflush(stdout);
            flint_abort();
        }

        TEMPLATE(T, poly_clear) (a, ctx);
        TEMPLATE(T, poly_clear) (b, ctx);
        TEMPLATE(T, poly_clear) (g, ctx);

        TEMPLATE(T, ctx_clear) (ctx);
    }

    /* Check aliasing of b and g */
    for (i = 0; i < 10 * flint_test_multiplier(); i++)
    {
        TEMPLATE(T, ctx_t) ctx;
        TEMPLATE(T, poly_t) a, b, g;

        TEMPLATE(T, ctx_randtest) (ctx, state);

        TEMPLATE(T, poly_init) (a, ctx);
        TEMPLATE(T, poly_init) (b, ctx);
        TEMPLATE(T, poly_init) (g, ctx);
        TEMPLATE(T, poly_randtest) (a, state, n_randint(state, 300), ctx);
        TEMPLATE(T, poly_randtest) (b, state, n_randint(state, 300), ctx);

        TEMPLATE(T, poly_gcd_hgcd) (g, a, b, ctx);
        TEMPLATE(T, poly_gcd_hgcd) (b, a, b, ctx);

        result = (TEMPLATE(T, poly_equal) (b, g, ctx));
        if (!result)
        {
            flint_printf("FAIL:\n");
            TEMPLATE(T, poly_print_pretty) (a, "x", ctx), flint_printf("\n\n");
            TEMPLATE(T, poly_print_pretty) (b, "x", ctx), flint_printf("\n\n");
            TEMPLATE(T, poly_print_pretty) (g, "x", ctx), flint_printf("\n\n");
            fflush(stdout);
            flint_abort();
        }

        TEMPLATE(T, poly_clear) (a, ctx);
        TEMPLATE(T, poly_clear) (b, ctx);
        TEMPLATE(T, poly_clear) (g, ctx);

        TEMPLATE(T, ctx_clear) (ctx);
    }

    FLINT_TEST_CLEANUP(state);

    flint_printf("PASS\n");
    return 0;
}



#endif
