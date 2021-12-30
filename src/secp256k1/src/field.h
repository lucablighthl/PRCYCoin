/**********************************************************************
 * Copyright (c) 2013, 2014 Pieter Wuille                             *
 * Distributed under the MIT software license, see the accompanying   *
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.*
 **********************************************************************/

<<<<<<< HEAD
#ifndef _SECP256K1_FIELD_
#define _SECP256K1_FIELD_
=======
#ifndef SECP256K1_FIELD_H
#define SECP256K1_FIELD_H
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

/** Field element module.
 *
 *  Field elements can be represented in several ways, but code accessing
<<<<<<< HEAD
 *  it (and implementations) need to take certain properaties into account:
=======
 *  it (and implementations) need to take certain properties into account:
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
 *  - Each field element can be normalized or not.
 *  - Each field element has a magnitude, which represents how far away
 *    its representation is away from normalization. Normalized elements
 *    always have a magnitude of 1, but a magnitude of 1 doesn't imply
 *    normality.
 */

#if defined HAVE_CONFIG_H
#include "libsecp256k1-config.h"
#endif

<<<<<<< HEAD
#if defined(USE_FIELD_GMP)
#include "field_gmp.h"
#elif defined(USE_FIELD_10X26)
=======
#if defined(USE_FIELD_10X26)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#include "field_10x26.h"
#elif defined(USE_FIELD_5X52)
#include "field_5x52.h"
#else
#error "Please select field implementation"
#endif

<<<<<<< HEAD
typedef struct {
#ifndef USE_NUM_NONE
    secp256k1_num_t p;
#endif
    secp256k1_fe_t order;
} secp256k1_fe_consts_t;

static const secp256k1_fe_consts_t *secp256k1_fe_consts = NULL;

/** Initialize field element precomputation data. */
static void secp256k1_fe_start(void);

/** Unload field element precomputation data. */
static void secp256k1_fe_stop(void);

/** Normalize a field element. */
static void secp256k1_fe_normalize(secp256k1_fe_t *r);

/** Set a field element equal to a small integer. Resulting field element is normalized. */
static void secp256k1_fe_set_int(secp256k1_fe_t *r, int a);

/** Verify whether a field element is zero. Requires the input to be normalized. */
static int secp256k1_fe_is_zero(const secp256k1_fe_t *a);

/** Check the "oddness" of a field element. Requires the input to be normalized. */
static int secp256k1_fe_is_odd(const secp256k1_fe_t *a);

/** Compare two field elements. Requires both inputs to be normalized */
static int secp256k1_fe_equal(const secp256k1_fe_t *a, const secp256k1_fe_t *b);

/** Compare two field elements. Requires both inputs to be normalized */
static int secp256k1_fe_cmp_var(const secp256k1_fe_t *a, const secp256k1_fe_t *b);

/** Set a field element equal to 32-byte big endian value. If succesful, the resulting field element is normalized. */
static int secp256k1_fe_set_b32(secp256k1_fe_t *r, const unsigned char *a);

/** Convert a field element to a 32-byte big endian value. Requires the input to be normalized */
static void secp256k1_fe_get_b32(unsigned char *r, const secp256k1_fe_t *a);

/** Set a field element equal to the additive inverse of another. Takes a maximum magnitude of the input
 *  as an argument. The magnitude of the output is one higher. */
static void secp256k1_fe_negate(secp256k1_fe_t *r, const secp256k1_fe_t *a, int m);

/** Multiplies the passed field element with a small integer constant. Multiplies the magnitude by that
 *  small integer. */
static void secp256k1_fe_mul_int(secp256k1_fe_t *r, int a);

/** Adds a field element to another. The result has the sum of the inputs' magnitudes as magnitude. */
static void secp256k1_fe_add(secp256k1_fe_t *r, const secp256k1_fe_t *a);

/** Sets a field element to be the product of two others. Requires the inputs' magnitudes to be at most 8.
 *  The output magnitude is 1 (but not guaranteed to be normalized). */
static void secp256k1_fe_mul(secp256k1_fe_t *r, const secp256k1_fe_t *a, const secp256k1_fe_t * SECP256K1_RESTRICT b);

/** Sets a field element to be the square of another. Requires the input's magnitude to be at most 8.
 *  The output magnitude is 1 (but not guaranteed to be normalized). */
static void secp256k1_fe_sqr(secp256k1_fe_t *r, const secp256k1_fe_t *a);

/** Sets a field element to be the (modular) square root (if any exist) of another. Requires the
 *  input's magnitude to be at most 8. The output magnitude is 1 (but not guaranteed to be
 *  normalized). Return value indicates whether a square root was found. */
static int secp256k1_fe_sqrt(secp256k1_fe_t *r, const secp256k1_fe_t *a);

/** Sets a field element to be the (modular) inverse of another. Requires the input's magnitude to be
 *  at most 8. The output magnitude is 1 (but not guaranteed to be normalized). */
static void secp256k1_fe_inv(secp256k1_fe_t *r, const secp256k1_fe_t *a);

/** Potentially faster version of secp256k1_fe_inv, without constant-time guarantee. */
static void secp256k1_fe_inv_var(secp256k1_fe_t *r, const secp256k1_fe_t *a);
=======
#include "util.h"

/** Normalize a field element. */
static void secp256k1_fe_normalize(secp256k1_fe *r);

/** Weakly normalize a field element: reduce it magnitude to 1, but don't fully normalize. */
static void secp256k1_fe_normalize_weak(secp256k1_fe *r);

/** Normalize a field element, without constant-time guarantee. */
static void secp256k1_fe_normalize_var(secp256k1_fe *r);

/** Verify whether a field element represents zero i.e. would normalize to a zero value. The field
 *  implementation may optionally normalize the input, but this should not be relied upon. */
static int secp256k1_fe_normalizes_to_zero(secp256k1_fe *r);

/** Verify whether a field element represents zero i.e. would normalize to a zero value. The field
 *  implementation may optionally normalize the input, but this should not be relied upon. */
static int secp256k1_fe_normalizes_to_zero_var(secp256k1_fe *r);

/** Set a field element equal to a small integer. Resulting field element is normalized. */
static void secp256k1_fe_set_int(secp256k1_fe *r, int a);

/** Sets a field element equal to zero, initializing all fields. */
static void secp256k1_fe_clear(secp256k1_fe *a);

/** Verify whether a field element is zero. Requires the input to be normalized. */
static int secp256k1_fe_is_zero(const secp256k1_fe *a);

/** Check the "oddness" of a field element. Requires the input to be normalized. */
static int secp256k1_fe_is_odd(const secp256k1_fe *a);

/** Compare two field elements. Requires magnitude-1 inputs. */
static int secp256k1_fe_equal(const secp256k1_fe *a, const secp256k1_fe *b);

/** Same as secp256k1_fe_equal, but may be variable time. */
static int secp256k1_fe_equal_var(const secp256k1_fe *a, const secp256k1_fe *b);

/** Compare two field elements. Requires both inputs to be normalized */
static int secp256k1_fe_cmp_var(const secp256k1_fe *a, const secp256k1_fe *b);

/** Set a field element equal to 32-byte big endian value. If successful, the resulting field element is normalized. */
static int secp256k1_fe_set_b32(secp256k1_fe *r, const unsigned char *a);

/** Convert a field element to a 32-byte big endian value. Requires the input to be normalized */
static void secp256k1_fe_get_b32(unsigned char *r, const secp256k1_fe *a);

/** Set a field element equal to the additive inverse of another. Takes a maximum magnitude of the input
 *  as an argument. The magnitude of the output is one higher. */
static void secp256k1_fe_negate(secp256k1_fe *r, const secp256k1_fe *a, int m);

/** Multiplies the passed field element with a small integer constant. Multiplies the magnitude by that
 *  small integer. */
static void secp256k1_fe_mul_int(secp256k1_fe *r, int a);

/** Adds a field element to another. The result has the sum of the inputs' magnitudes as magnitude. */
static void secp256k1_fe_add(secp256k1_fe *r, const secp256k1_fe *a);

/** Sets a field element to be the product of two others. Requires the inputs' magnitudes to be at most 8.
 *  The output magnitude is 1 (but not guaranteed to be normalized). */
static void secp256k1_fe_mul(secp256k1_fe *r, const secp256k1_fe *a, const secp256k1_fe * SECP256K1_RESTRICT b);

/** Sets a field element to be the square of another. Requires the input's magnitude to be at most 8.
 *  The output magnitude is 1 (but not guaranteed to be normalized). */
static void secp256k1_fe_sqr(secp256k1_fe *r, const secp256k1_fe *a);

/** If a has a square root, it is computed in r and 1 is returned. If a does not
 *  have a square root, the root of its negation is computed and 0 is returned.
 *  The input's magnitude can be at most 8. The output magnitude is 1 (but not
 *  guaranteed to be normalized). The result in r will always be a square
 *  itself. */
static int secp256k1_fe_sqrt(secp256k1_fe *r, const secp256k1_fe *a);

/** Checks whether a field element is a quadratic residue. */
static int secp256k1_fe_is_quad_var(const secp256k1_fe *a);

/** Sets a field element to be the (modular) inverse of another. Requires the input's magnitude to be
 *  at most 8. The output magnitude is 1 (but not guaranteed to be normalized). */
static void secp256k1_fe_inv(secp256k1_fe *r, const secp256k1_fe *a);

/** Potentially faster version of secp256k1_fe_inv, without constant-time guarantee. */
static void secp256k1_fe_inv_var(secp256k1_fe *r, const secp256k1_fe *a);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

/** Calculate the (modular) inverses of a batch of field elements. Requires the inputs' magnitudes to be
 *  at most 8. The output magnitudes are 1 (but not guaranteed to be normalized). The inputs and
 *  outputs must not overlap in memory. */
<<<<<<< HEAD
static void secp256k1_fe_inv_all(size_t len, secp256k1_fe_t r[len], const secp256k1_fe_t a[len]);

/** Potentially faster version of secp256k1_fe_inv_all, without constant-time guarantee. */
static void secp256k1_fe_inv_all_var(size_t len, secp256k1_fe_t r[len], const secp256k1_fe_t a[len]);

/** Convert a field element to a hexadecimal string. */
static void secp256k1_fe_get_hex(char *r, int *rlen, const secp256k1_fe_t *a);

/** Convert a hexadecimal string to a field element. */
static int secp256k1_fe_set_hex(secp256k1_fe_t *r, const char *a, int alen);

/** If flag is true, set *r equal to *a; otherwise leave it. Constant-time. */
static void secp256k1_fe_cmov(secp256k1_fe_t *r, const secp256k1_fe_t *a, int flag);

#endif
=======
static void secp256k1_fe_inv_all_var(secp256k1_fe *r, const secp256k1_fe *a, size_t len);

/** Convert a field element to the storage type. */
static void secp256k1_fe_to_storage(secp256k1_fe_storage *r, const secp256k1_fe *a);

/** Convert a field element back from the storage type. */
static void secp256k1_fe_from_storage(secp256k1_fe *r, const secp256k1_fe_storage *a);

/** If flag is true, set *r equal to *a; otherwise leave it. Constant-time. */
static void secp256k1_fe_storage_cmov(secp256k1_fe_storage *r, const secp256k1_fe_storage *a, int flag);

/** If flag is true, set *r equal to *a; otherwise leave it. Constant-time. */
static void secp256k1_fe_cmov(secp256k1_fe *r, const secp256k1_fe *a, int flag);

#endif /* SECP256K1_FIELD_H */
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
