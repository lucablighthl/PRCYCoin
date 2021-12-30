// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_DB_DB_ITER_H_
#define STORAGE_LEVELDB_DB_DB_ITER_H_

#include <stdint.h>
<<<<<<< HEAD
#include "leveldb/db.h"
#include "db/dbformat.h"
=======

#include "db/dbformat.h"
#include "leveldb/db.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

namespace leveldb {

class DBImpl;

// Return a new iterator that converts internal keys (yielded by
// "*internal_iter") that were live at the specified "sequence" number
// into appropriate user keys.
<<<<<<< HEAD
extern Iterator* NewDBIterator(
    DBImpl* db,
    const Comparator* user_key_comparator,
    Iterator* internal_iter,
    SequenceNumber sequence,
    uint32_t seed);
=======
Iterator* NewDBIterator(DBImpl* db, const Comparator* user_key_comparator,
                        Iterator* internal_iter, SequenceNumber sequence,
                        uint32_t seed);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_DB_DB_ITER_H_
