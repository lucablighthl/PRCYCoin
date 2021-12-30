<<<<<<< HEAD
// Copyright (c) 2009-2014 The Bitcoin developers
=======
// Copyright (c) 2009-2017 The Bitcoin Core developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
<<<<<<< HEAD
#include "config/prcycoin-config.h"
=======
#include "config/pivx-config.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#endif

#include <cstring>

#if HAVE_DECL_STRNLEN == 0
size_t strnlen( const char *start, size_t max_len)
{
    const char *end = (const char *)memchr(start, '\0', max_len);

    return end ? (size_t)(end - start) : max_len;
}
<<<<<<< HEAD
#endif // HAVE_DECL_STRNLEN
=======
#endif // HAVE_DECL_STRNLEN
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
