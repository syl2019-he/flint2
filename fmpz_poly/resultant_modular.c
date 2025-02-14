/*
    Copyright (C) 2014 William Hart

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include <gmp.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_vec.h"
#include "fmpz_poly.h"
#include "mpn_extras.h"


void _fmpz_poly_resultant_modular(fmpz_t res, const fmpz * poly1, slong len1, 
                                        const fmpz * poly2, slong len2)
{
    flint_bitcnt_t bound, pbits, curr_bits = 0; 
    slong i, num_primes;
    fmpz_comb_t comb;
    fmpz_comb_temp_t comb_temp;
    fmpz_t ac, bc, l, modulus;
    fmpz * A, * B, * lead_A, * lead_B;
    mp_ptr a, b, rarr, parr;
    mp_limb_t p;
    nmod_t mod;
    
    /* special case, one of the polys is a constant */
    if (len2 == 1) /* if len1 == 1 then so does len2 */
    {
        fmpz_pow_ui(res, poly2, len1 - 1);

        return;
    }
    
    fmpz_init(ac);
    fmpz_init(bc);
    
    /* compute content of poly1 and poly2 */
    _fmpz_vec_content(ac, poly1, len1);
    _fmpz_vec_content(bc, poly2, len2);
    
    /* divide poly1 and poly2 by their content */
    A = _fmpz_vec_init(len1);
    B = _fmpz_vec_init(len2);
    _fmpz_vec_scalar_divexact_fmpz(A, poly1, len1, ac);
    _fmpz_vec_scalar_divexact_fmpz(B, poly2, len2, bc);
    
    /* get product of leading coefficients */
    fmpz_init(l);
    
    lead_A = A + len1 - 1;
    lead_B = B + len2 - 1;
    fmpz_mul(l, lead_A, lead_B);

    /* set size of first prime */
    pbits = FLINT_BITS - 1;
    p = (UWORD(1)<<pbits);

    /* Theorem 7, https://doi.org/10.1016/j.laa.2009.08.012 */
    /* |res(f,g)| <= (|f|_2)^deg(g) (|g|_2)^deg(f) */
    {
        fmpz_t b1, b2;
        fmpz_init(b1);
        fmpz_init(b2);

        for (i = 0; i < len1; i++)
            fmpz_addmul(b1, A + i, A + i);
        for (i = 0; i < len2; i++)
            fmpz_addmul(b2, B + i, B + i);

        fmpz_pow_ui(b1, b1, len2 - 1);
        fmpz_pow_ui(b2, b2, len1 - 1);
        fmpz_mul(b1, b1, b2);

        fmpz_sqrt(b1, b1);
        fmpz_add_ui(b1, b1, 1);

        bound = fmpz_bits(b1) + 2;

        fmpz_clear(b1);
        fmpz_clear(b2);
    }

    num_primes = (bound + pbits - 1)/pbits;

    parr = _nmod_vec_init(num_primes);
    rarr = _nmod_vec_init(num_primes);

    fmpz_init(modulus);
    fmpz_set_ui(modulus, 1);
    fmpz_zero(res);

    /* make space for polynomials mod p */
    a = _nmod_vec_init(len1);
    b = _nmod_vec_init(len2);

    for (i = 0; curr_bits < bound; )
    {
        /* get new prime and initialise modulus */
        p = n_nextprime(p, 0);
        if (fmpz_fdiv_ui(l, p) == 0)
            continue;
        
        curr_bits += pbits;

        nmod_init(&mod, p);

        /* reduce polynomials modulo p */
        _fmpz_vec_get_nmod_vec(a, A, len1, mod);
        _fmpz_vec_get_nmod_vec(b, B, len2, mod);

        /* compute resultant over Z/pZ */
        parr[i] = p;
        rarr[i++] = _nmod_poly_resultant(a, len1, b, len2, mod);
    }

    fmpz_comb_init(comb, parr, num_primes);
    fmpz_comb_temp_init(comb_temp, comb);
    
    fmpz_multi_CRT_ui(res, rarr, comb, comb_temp, 1);
        
    fmpz_clear(modulus);
    fmpz_comb_temp_clear(comb_temp);
    fmpz_comb_clear(comb);
        
    _nmod_vec_clear(a);
    _nmod_vec_clear(b);

    _nmod_vec_clear(parr);
    _nmod_vec_clear(rarr);
    
    /* finally multiply by powers of content */
    if (!fmpz_is_one(ac))
    {
       fmpz_pow_ui(l, ac, len2 - 1);
       fmpz_mul(res, res, l);
    }
    
    if (!fmpz_is_one(bc))
    {
       fmpz_pow_ui(l, bc, len1 - 1);
       fmpz_mul(res, res, l);
    }

    fmpz_clear(l); 
    
    _fmpz_vec_clear(A, len1);
    _fmpz_vec_clear(B, len2);

    fmpz_clear(ac);
    fmpz_clear(bc);
}

void
fmpz_poly_resultant_modular(fmpz_t res, const fmpz_poly_t poly1,
              const fmpz_poly_t poly2)
{
   slong len1 = poly1->length;
   slong len2 = poly2->length;
   
   if (len1 == 0 || len2 == 0)
     fmpz_zero(res);
   else if (len1 >= len2)
        _fmpz_poly_resultant_modular(res, poly1->coeffs, len1, poly2->coeffs, len2);
   else
   {
        _fmpz_poly_resultant_modular(res, poly2->coeffs, len2, poly1->coeffs, len1);  
        if ((len1 > 1) && (!(len1 & WORD(1)) & !(len2 & WORD(1))))
            fmpz_neg(res, res);
   }
}

