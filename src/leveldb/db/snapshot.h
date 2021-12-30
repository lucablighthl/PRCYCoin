// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_DB_SNAPSHOT_H_
#define STORAGE_LEVELDB_DB_SNAPSHOT_H_

#include "db/dbformat.h"
#include "leveldb/db.h"

namespace leveldb {

class SnapshotList;

// Snapshots are kept in a doubly-linked list in the DB.
// Each SnapshotImpl corresponds to a particular sequence number.
class SnapshotImpl : public Snapshot {
 public:
<<<<<<< HEAD
  SequenceNumber number_;  // const after creation
=======
  SnapshotImpl(SequenceNumber sequence_number)
      : sequence_number_(sequence_number) {}

  SequenceNumber sequence_number() const { return sequence_number_; }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

 private:
  friend class SnapshotList;

<<<<<<< HEAD
  // SnapshotImpl is kept in a doubly-linked circular list
  SnapshotImpl* prev_;
  SnapshotImpl* next_;

  SnapshotList* list_;                 // just for sanity checks
=======
  // SnapshotImpl is kept in a doubly-linked circular list. The SnapshotList
  // implementation operates on the next/previous fields direcly.
  SnapshotImpl* prev_;
  SnapshotImpl* next_;

  const SequenceNumber sequence_number_;

#if !defined(NDEBUG)
  SnapshotList* list_ = nullptr;
#endif  // !defined(NDEBUG)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};

class SnapshotList {
 public:
<<<<<<< HEAD
  SnapshotList() {
    list_.prev_ = &list_;
    list_.next_ = &list_;
  }

  bool empty() const { return list_.next_ == &list_; }
  SnapshotImpl* oldest() const { assert(!empty()); return list_.next_; }
  SnapshotImpl* newest() const { assert(!empty()); return list_.prev_; }

  const SnapshotImpl* New(SequenceNumber seq) {
    SnapshotImpl* s = new SnapshotImpl;
    s->number_ = seq;
    s->list_ = this;
    s->next_ = &list_;
    s->prev_ = list_.prev_;
    s->prev_->next_ = s;
    s->next_->prev_ = s;
    return s;
  }

  void Delete(const SnapshotImpl* s) {
    assert(s->list_ == this);
    s->prev_->next_ = s->next_;
    s->next_->prev_ = s->prev_;
    delete s;
=======
  SnapshotList() : head_(0) {
    head_.prev_ = &head_;
    head_.next_ = &head_;
  }

  bool empty() const { return head_.next_ == &head_; }
  SnapshotImpl* oldest() const {
    assert(!empty());
    return head_.next_;
  }
  SnapshotImpl* newest() const {
    assert(!empty());
    return head_.prev_;
  }

  // Creates a SnapshotImpl and appends it to the end of the list.
  SnapshotImpl* New(SequenceNumber sequence_number) {
    assert(empty() || newest()->sequence_number_ <= sequence_number);

    SnapshotImpl* snapshot = new SnapshotImpl(sequence_number);

#if !defined(NDEBUG)
    snapshot->list_ = this;
#endif  // !defined(NDEBUG)
    snapshot->next_ = &head_;
    snapshot->prev_ = head_.prev_;
    snapshot->prev_->next_ = snapshot;
    snapshot->next_->prev_ = snapshot;
    return snapshot;
  }

  // Removes a SnapshotImpl from this list.
  //
  // The snapshot must have been created by calling New() on this list.
  //
  // The snapshot pointer should not be const, because its memory is
  // deallocated. However, that would force us to change DB::ReleaseSnapshot(),
  // which is in the API, and currently takes a const Snapshot.
  void Delete(const SnapshotImpl* snapshot) {
#if !defined(NDEBUG)
    assert(snapshot->list_ == this);
#endif  // !defined(NDEBUG)
    snapshot->prev_->next_ = snapshot->next_;
    snapshot->next_->prev_ = snapshot->prev_;
    delete snapshot;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  }

 private:
  // Dummy head of doubly-linked list of snapshots
<<<<<<< HEAD
  SnapshotImpl list_;
=======
  SnapshotImpl head_;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_DB_SNAPSHOT_H_
