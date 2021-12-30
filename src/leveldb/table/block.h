// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_TABLE_BLOCK_H_
#define STORAGE_LEVELDB_TABLE_BLOCK_H_

#include <stddef.h>
#include <stdint.h>
<<<<<<< HEAD
=======

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#include "leveldb/iterator.h"

namespace leveldb {

struct BlockContents;
class Comparator;

class Block {
 public:
  // Initialize the block with the specified contents.
  explicit Block(const BlockContents& contents);

<<<<<<< HEAD
=======
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  ~Block();

  size_t size() const { return size_; }
  Iterator* NewIterator(const Comparator* comparator);

 private:
<<<<<<< HEAD
=======
  class Iter;

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  uint32_t NumRestarts() const;

  const char* data_;
  size_t size_;
<<<<<<< HEAD
  uint32_t restart_offset_;     // Offset in data_ of restart array
  bool owned_;                  // Block owns data_[]

  // No copying allowed
  Block(const Block&);
  void operator=(const Block&);

  class Iter;
=======
  uint32_t restart_offset_;  // Offset in data_ of restart array
  bool owned_;               // Block owns data_[]
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_TABLE_BLOCK_H_
