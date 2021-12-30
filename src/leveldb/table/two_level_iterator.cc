// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "table/two_level_iterator.h"

#include "leveldb/table.h"
#include "table/block.h"
#include "table/format.h"
#include "table/iterator_wrapper.h"

namespace leveldb {

namespace {

typedef Iterator* (*BlockFunction)(void*, const ReadOptions&, const Slice&);

<<<<<<< HEAD
class TwoLevelIterator: public Iterator {
 public:
  TwoLevelIterator(
    Iterator* index_iter,
    BlockFunction block_function,
    void* arg,
    const ReadOptions& options);

  virtual ~TwoLevelIterator();

  virtual void Seek(const Slice& target);
  virtual void SeekToFirst();
  virtual void SeekToLast();
  virtual void Next();
  virtual void Prev();

  virtual bool Valid() const {
    return data_iter_.Valid();
  }
  virtual Slice key() const {
    assert(Valid());
    return data_iter_.key();
  }
  virtual Slice value() const {
    assert(Valid());
    return data_iter_.value();
  }
  virtual Status status() const {
    // It'd be nice if status() returned a const Status& instead of a Status
    if (!index_iter_.status().ok()) {
      return index_iter_.status();
    } else if (data_iter_.iter() != NULL && !data_iter_.status().ok()) {
=======
class TwoLevelIterator : public Iterator {
 public:
  TwoLevelIterator(Iterator* index_iter, BlockFunction block_function,
                   void* arg, const ReadOptions& options);

  ~TwoLevelIterator() override;

  void Seek(const Slice& target) override;
  void SeekToFirst() override;
  void SeekToLast() override;
  void Next() override;
  void Prev() override;

  bool Valid() const override { return data_iter_.Valid(); }
  Slice key() const override {
    assert(Valid());
    return data_iter_.key();
  }
  Slice value() const override {
    assert(Valid());
    return data_iter_.value();
  }
  Status status() const override {
    // It'd be nice if status() returned a const Status& instead of a Status
    if (!index_iter_.status().ok()) {
      return index_iter_.status();
    } else if (data_iter_.iter() != nullptr && !data_iter_.status().ok()) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
      return data_iter_.status();
    } else {
      return status_;
    }
  }

 private:
  void SaveError(const Status& s) {
    if (status_.ok() && !s.ok()) status_ = s;
  }
  void SkipEmptyDataBlocksForward();
  void SkipEmptyDataBlocksBackward();
  void SetDataIterator(Iterator* data_iter);
  void InitDataBlock();

  BlockFunction block_function_;
  void* arg_;
  const ReadOptions options_;
  Status status_;
  IteratorWrapper index_iter_;
<<<<<<< HEAD
  IteratorWrapper data_iter_; // May be NULL
  // If data_iter_ is non-NULL, then "data_block_handle_" holds the
=======
  IteratorWrapper data_iter_;  // May be nullptr
  // If data_iter_ is non-null, then "data_block_handle_" holds the
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  // "index_value" passed to block_function_ to create the data_iter_.
  std::string data_block_handle_;
};

<<<<<<< HEAD
TwoLevelIterator::TwoLevelIterator(
    Iterator* index_iter,
    BlockFunction block_function,
    void* arg,
    const ReadOptions& options)
=======
TwoLevelIterator::TwoLevelIterator(Iterator* index_iter,
                                   BlockFunction block_function, void* arg,
                                   const ReadOptions& options)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    : block_function_(block_function),
      arg_(arg),
      options_(options),
      index_iter_(index_iter),
<<<<<<< HEAD
      data_iter_(NULL) {
}

TwoLevelIterator::~TwoLevelIterator() {
}
=======
      data_iter_(nullptr) {}

TwoLevelIterator::~TwoLevelIterator() = default;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

void TwoLevelIterator::Seek(const Slice& target) {
  index_iter_.Seek(target);
  InitDataBlock();
<<<<<<< HEAD
  if (data_iter_.iter() != NULL) data_iter_.Seek(target);
=======
  if (data_iter_.iter() != nullptr) data_iter_.Seek(target);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  SkipEmptyDataBlocksForward();
}

void TwoLevelIterator::SeekToFirst() {
  index_iter_.SeekToFirst();
  InitDataBlock();
<<<<<<< HEAD
  if (data_iter_.iter() != NULL) data_iter_.SeekToFirst();
=======
  if (data_iter_.iter() != nullptr) data_iter_.SeekToFirst();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  SkipEmptyDataBlocksForward();
}

void TwoLevelIterator::SeekToLast() {
  index_iter_.SeekToLast();
  InitDataBlock();
<<<<<<< HEAD
  if (data_iter_.iter() != NULL) data_iter_.SeekToLast();
=======
  if (data_iter_.iter() != nullptr) data_iter_.SeekToLast();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  SkipEmptyDataBlocksBackward();
}

void TwoLevelIterator::Next() {
  assert(Valid());
  data_iter_.Next();
  SkipEmptyDataBlocksForward();
}

void TwoLevelIterator::Prev() {
  assert(Valid());
  data_iter_.Prev();
  SkipEmptyDataBlocksBackward();
}

<<<<<<< HEAD

void TwoLevelIterator::SkipEmptyDataBlocksForward() {
  while (data_iter_.iter() == NULL || !data_iter_.Valid()) {
    // Move to next block
    if (!index_iter_.Valid()) {
      SetDataIterator(NULL);
=======
void TwoLevelIterator::SkipEmptyDataBlocksForward() {
  while (data_iter_.iter() == nullptr || !data_iter_.Valid()) {
    // Move to next block
    if (!index_iter_.Valid()) {
      SetDataIterator(nullptr);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
      return;
    }
    index_iter_.Next();
    InitDataBlock();
<<<<<<< HEAD
    if (data_iter_.iter() != NULL) data_iter_.SeekToFirst();
=======
    if (data_iter_.iter() != nullptr) data_iter_.SeekToFirst();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  }
}

void TwoLevelIterator::SkipEmptyDataBlocksBackward() {
<<<<<<< HEAD
  while (data_iter_.iter() == NULL || !data_iter_.Valid()) {
    // Move to next block
    if (!index_iter_.Valid()) {
      SetDataIterator(NULL);
=======
  while (data_iter_.iter() == nullptr || !data_iter_.Valid()) {
    // Move to next block
    if (!index_iter_.Valid()) {
      SetDataIterator(nullptr);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
      return;
    }
    index_iter_.Prev();
    InitDataBlock();
<<<<<<< HEAD
    if (data_iter_.iter() != NULL) data_iter_.SeekToLast();
=======
    if (data_iter_.iter() != nullptr) data_iter_.SeekToLast();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  }
}

void TwoLevelIterator::SetDataIterator(Iterator* data_iter) {
<<<<<<< HEAD
  if (data_iter_.iter() != NULL) SaveError(data_iter_.status());
=======
  if (data_iter_.iter() != nullptr) SaveError(data_iter_.status());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  data_iter_.Set(data_iter);
}

void TwoLevelIterator::InitDataBlock() {
  if (!index_iter_.Valid()) {
<<<<<<< HEAD
    SetDataIterator(NULL);
  } else {
    Slice handle = index_iter_.value();
    if (data_iter_.iter() != NULL && handle.compare(data_block_handle_) == 0) {
=======
    SetDataIterator(nullptr);
  } else {
    Slice handle = index_iter_.value();
    if (data_iter_.iter() != nullptr &&
        handle.compare(data_block_handle_) == 0) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
      // data_iter_ is already constructed with this iterator, so
      // no need to change anything
    } else {
      Iterator* iter = (*block_function_)(arg_, options_, handle);
      data_block_handle_.assign(handle.data(), handle.size());
      SetDataIterator(iter);
    }
  }
}

}  // namespace

<<<<<<< HEAD
Iterator* NewTwoLevelIterator(
    Iterator* index_iter,
    BlockFunction block_function,
    void* arg,
    const ReadOptions& options) {
=======
Iterator* NewTwoLevelIterator(Iterator* index_iter,
                              BlockFunction block_function, void* arg,
                              const ReadOptions& options) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  return new TwoLevelIterator(index_iter, block_function, arg, options);
}

}  // namespace leveldb
