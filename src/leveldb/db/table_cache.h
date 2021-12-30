// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Thread-safe (provides internal synchronization)

#ifndef STORAGE_LEVELDB_DB_TABLE_CACHE_H_
#define STORAGE_LEVELDB_DB_TABLE_CACHE_H_

<<<<<<< HEAD
#include <string>
#include <stdint.h>
=======
#include <stdint.h>

#include <string>

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#include "db/dbformat.h"
#include "leveldb/cache.h"
#include "leveldb/table.h"
#include "port/port.h"

namespace leveldb {

class Env;

class TableCache {
 public:
<<<<<<< HEAD
  TableCache(const std::string& dbname, const Options* options, int entries);
=======
  TableCache(const std::string& dbname, const Options& options, int entries);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  ~TableCache();

  // Return an iterator for the specified file number (the corresponding
  // file length must be exactly "file_size" bytes).  If "tableptr" is
<<<<<<< HEAD
  // non-NULL, also sets "*tableptr" to point to the Table object
  // underlying the returned iterator, or NULL if no Table object underlies
  // the returned iterator.  The returned "*tableptr" object is owned by
  // the cache and should not be deleted, and is valid for as long as the
  // returned iterator is live.
  Iterator* NewIterator(const ReadOptions& options,
                        uint64_t file_number,
                        uint64_t file_size,
                        Table** tableptr = NULL);

  // If a seek to internal key "k" in specified file finds an entry,
  // call (*handle_result)(arg, found_key, found_value).
  Status Get(const ReadOptions& options,
             uint64_t file_number,
             uint64_t file_size,
             const Slice& k,
             void* arg,
=======
  // non-null, also sets "*tableptr" to point to the Table object
  // underlying the returned iterator, or to nullptr if no Table object
  // underlies the returned iterator.  The returned "*tableptr" object is owned
  // by the cache and should not be deleted, and is valid for as long as the
  // returned iterator is live.
  Iterator* NewIterator(const ReadOptions& options, uint64_t file_number,
                        uint64_t file_size, Table** tableptr = nullptr);

  // If a seek to internal key "k" in specified file finds an entry,
  // call (*handle_result)(arg, found_key, found_value).
  Status Get(const ReadOptions& options, uint64_t file_number,
             uint64_t file_size, const Slice& k, void* arg,
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
             void (*handle_result)(void*, const Slice&, const Slice&));

  // Evict any entry for the specified file number
  void Evict(uint64_t file_number);

 private:
<<<<<<< HEAD
  Env* const env_;
  const std::string dbname_;
  const Options* options_;
  Cache* cache_;

  Status FindTable(uint64_t file_number, uint64_t file_size, Cache::Handle**);
=======
  Status FindTable(uint64_t file_number, uint64_t file_size, Cache::Handle**);

  Env* const env_;
  const std::string dbname_;
  const Options& options_;
  Cache* cache_;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_DB_TABLE_CACHE_H_
