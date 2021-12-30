<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef PRCYcoin_BLOCKSIGNATURE_H
#define PRCYcoin_BLOCKSIGNATURE_H
=======
// Copyright (c) 2017-2019 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef PIVX_BLOCKSIGNATURE_H
#define PIVX_BLOCKSIGNATURE_H
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

#include "key.h"
#include "primitives/block.h"
#include "keystore.h"

bool SignBlockWithKey(CBlock& block, const CKey& key);
bool SignBlock(CBlock& block, const CKeyStore& keystore);
bool CheckBlockSignature(const CBlock& block);

<<<<<<< HEAD
#endif //PRCYcoin_BLOCKSIGNATURE_H
=======
#endif //PIVX_BLOCKSIGNATURE_H
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
