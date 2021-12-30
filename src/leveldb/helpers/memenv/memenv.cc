// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "helpers/memenv/memenv.h"

<<<<<<< HEAD
#include "leveldb/env.h"
#include "leveldb/status.h"
#include "port/port.h"
#include "util/mutexlock.h"
#include <map>
#include <string.h>
#include <string>
#include <vector>
=======
#include <string.h>

#include <limits>
#include <map>
#include <string>
#include <vector>

#include "leveldb/env.h"
#include "leveldb/status.h"
#include "port/port.h"
#include "port/thread_annotations.h"
#include "util/mutexlock.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

namespace leveldb {

namespace {

class FileState {
 public:
  // FileStates are reference counted. The initial reference count is zero
  // and the caller must call Ref() at least once.
  FileState() : refs_(0), size_(0) {}

<<<<<<< HEAD
=======
  // No copying allowed.
  FileState(const FileState&) = delete;
  FileState& operator=(const FileState&) = delete;

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  // Increase the reference count.
  void Ref() {
    MutexLock lock(&refs_mutex_);
    ++refs_;
  }

  // Decrease the reference count. Delete if this is the last reference.
  void Unref() {
    bool do_delete = false;

    {
      MutexLock lock(&refs_mutex_);
      --refs_;
      assert(refs_ >= 0);
      if (refs_ <= 0) {
        do_delete = true;
      }
    }

    if (do_delete) {
      delete this;
    }
  }

<<<<<<< HEAD
  uint64_t Size() const { return size_; }

  Status Read(uint64_t offset, size_t n, Slice* result, char* scratch) const {
=======
  uint64_t Size() const {
    MutexLock lock(&blocks_mutex_);
    return size_;
  }

  void Truncate() {
    MutexLock lock(&blocks_mutex_);
    for (char*& block : blocks_) {
      delete[] block;
    }
    blocks_.clear();
    size_ = 0;
  }

  Status Read(uint64_t offset, size_t n, Slice* result, char* scratch) const {
    MutexLock lock(&blocks_mutex_);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (offset > size_) {
      return Status::IOError("Offset greater than file size.");
    }
    const uint64_t available = size_ - offset;
    if (n > available) {
      n = static_cast<size_t>(available);
    }
    if (n == 0) {
      *result = Slice();
      return Status::OK();
    }

<<<<<<< HEAD
    assert(offset / kBlockSize <= SIZE_MAX);
    size_t block = static_cast<size_t>(offset / kBlockSize);
    size_t block_offset = offset % kBlockSize;

    if (n <= kBlockSize - block_offset) {
      // The requested bytes are all in the first block.
      *result = Slice(blocks_[block] + block_offset, n);
      return Status::OK();
    }

=======
    assert(offset / kBlockSize <= std::numeric_limits<size_t>::max());
    size_t block = static_cast<size_t>(offset / kBlockSize);
    size_t block_offset = offset % kBlockSize;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    size_t bytes_to_copy = n;
    char* dst = scratch;

    while (bytes_to_copy > 0) {
      size_t avail = kBlockSize - block_offset;
      if (avail > bytes_to_copy) {
        avail = bytes_to_copy;
      }
      memcpy(dst, blocks_[block] + block_offset, avail);

      bytes_to_copy -= avail;
      dst += avail;
      block++;
      block_offset = 0;
    }

    *result = Slice(scratch, n);
    return Status::OK();
  }

  Status Append(const Slice& data) {
    const char* src = data.data();
    size_t src_len = data.size();

<<<<<<< HEAD
=======
    MutexLock lock(&blocks_mutex_);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    while (src_len > 0) {
      size_t avail;
      size_t offset = size_ % kBlockSize;

      if (offset != 0) {
        // There is some room in the last block.
        avail = kBlockSize - offset;
      } else {
        // No room in the last block; push new one.
        blocks_.push_back(new char[kBlockSize]);
        avail = kBlockSize;
      }

      if (avail > src_len) {
        avail = src_len;
      }
      memcpy(blocks_.back() + offset, src, avail);
      src_len -= avail;
      src += avail;
      size_ += avail;
    }

    return Status::OK();
  }

 private:
<<<<<<< HEAD
  // Private since only Unref() should be used to delete it.
  ~FileState() {
    for (std::vector<char*>::iterator i = blocks_.begin(); i != blocks_.end();
         ++i) {
      delete [] *i;
    }
  }

  // No copying allowed.
  FileState(const FileState&);
  void operator=(const FileState&);

  port::Mutex refs_mutex_;
  int refs_;  // Protected by refs_mutex_;

  // The following fields are not protected by any mutex. They are only mutable
  // while the file is being written, and concurrent access is not allowed
  // to writable files.
  std::vector<char*> blocks_;
  uint64_t size_;

  enum { kBlockSize = 8 * 1024 };
=======
  enum { kBlockSize = 8 * 1024 };

  // Private since only Unref() should be used to delete it.
  ~FileState() { Truncate(); }

  port::Mutex refs_mutex_;
  int refs_ GUARDED_BY(refs_mutex_);

  mutable port::Mutex blocks_mutex_;
  std::vector<char*> blocks_ GUARDED_BY(blocks_mutex_);
  uint64_t size_ GUARDED_BY(blocks_mutex_);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};

class SequentialFileImpl : public SequentialFile {
 public:
  explicit SequentialFileImpl(FileState* file) : file_(file), pos_(0) {
    file_->Ref();
  }

<<<<<<< HEAD
  ~SequentialFileImpl() {
    file_->Unref();
  }

  virtual Status Read(size_t n, Slice* result, char* scratch) {
=======
  ~SequentialFileImpl() override { file_->Unref(); }

  Status Read(size_t n, Slice* result, char* scratch) override {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    Status s = file_->Read(pos_, n, result, scratch);
    if (s.ok()) {
      pos_ += result->size();
    }
    return s;
  }

<<<<<<< HEAD
  virtual Status Skip(uint64_t n) {
=======
  Status Skip(uint64_t n) override {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (pos_ > file_->Size()) {
      return Status::IOError("pos_ > file_->Size()");
    }
    const uint64_t available = file_->Size() - pos_;
    if (n > available) {
      n = available;
    }
    pos_ += n;
    return Status::OK();
  }

<<<<<<< HEAD
  virtual std::string GetName() const { return "[memenv]"; }
=======
  virtual std::string GetName() const override { return "[memenv]"; }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
 private:
  FileState* file_;
  uint64_t pos_;
};

class RandomAccessFileImpl : public RandomAccessFile {
 public:
<<<<<<< HEAD
  explicit RandomAccessFileImpl(FileState* file) : file_(file) {
    file_->Ref();
  }

  ~RandomAccessFileImpl() {
    file_->Unref();
  }

  virtual Status Read(uint64_t offset, size_t n, Slice* result,
                      char* scratch) const {
    return file_->Read(offset, n, result, scratch);
  }

  virtual std::string GetName() const { return "[memenv]"; }
=======
  explicit RandomAccessFileImpl(FileState* file) : file_(file) { file_->Ref(); }

  ~RandomAccessFileImpl() override { file_->Unref(); }

  Status Read(uint64_t offset, size_t n, Slice* result,
              char* scratch) const override {
    return file_->Read(offset, n, result, scratch);
  }

  virtual std::string GetName() const override { return "[memenv]"; }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
 private:
  FileState* file_;
};

class WritableFileImpl : public WritableFile {
 public:
<<<<<<< HEAD
  WritableFileImpl(FileState* file) : file_(file) {
    file_->Ref();
  }

  ~WritableFileImpl() {
    file_->Unref();
  }

  virtual Status Append(const Slice& data) {
    return file_->Append(data);
  }

  virtual Status Close() { return Status::OK(); }
  virtual Status Flush() { return Status::OK(); }
  virtual Status Sync() { return Status::OK(); }

  virtual std::string GetName() const { return "[memenv]"; }
=======
  WritableFileImpl(FileState* file) : file_(file) { file_->Ref(); }

  ~WritableFileImpl() override { file_->Unref(); }

  Status Append(const Slice& data) override { return file_->Append(data); }

  Status Close() override { return Status::OK(); }
  Status Flush() override { return Status::OK(); }
  Status Sync() override { return Status::OK(); }

  virtual std::string GetName() const override { return "[memenv]"; }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
 private:
  FileState* file_;
};

class NoOpLogger : public Logger {
 public:
<<<<<<< HEAD
  virtual void Logv(const char* format, va_list ap) { }
=======
  void Logv(const char* format, va_list ap) override {}
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};

class InMemoryEnv : public EnvWrapper {
 public:
<<<<<<< HEAD
  explicit InMemoryEnv(Env* base_env) : EnvWrapper(base_env) { }

  virtual ~InMemoryEnv() {
    for (FileSystem::iterator i = file_map_.begin(); i != file_map_.end(); ++i){
      i->second->Unref();
=======
  explicit InMemoryEnv(Env* base_env) : EnvWrapper(base_env) {}

  ~InMemoryEnv() override {
    for (const auto& kvp : file_map_) {
      kvp.second->Unref();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }
  }

  // Partial implementation of the Env interface.
<<<<<<< HEAD
  virtual Status NewSequentialFile(const std::string& fname,
                                   SequentialFile** result) {
    MutexLock lock(&mutex_);
    if (file_map_.find(fname) == file_map_.end()) {
      *result = NULL;
=======
  Status NewSequentialFile(const std::string& fname,
                           SequentialFile** result) override {
    MutexLock lock(&mutex_);
    if (file_map_.find(fname) == file_map_.end()) {
      *result = nullptr;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
      return Status::IOError(fname, "File not found");
    }

    *result = new SequentialFileImpl(file_map_[fname]);
    return Status::OK();
  }

<<<<<<< HEAD
  virtual Status NewRandomAccessFile(const std::string& fname,
                                     RandomAccessFile** result) {
    MutexLock lock(&mutex_);
    if (file_map_.find(fname) == file_map_.end()) {
      *result = NULL;
=======
  Status NewRandomAccessFile(const std::string& fname,
                             RandomAccessFile** result) override {
    MutexLock lock(&mutex_);
    if (file_map_.find(fname) == file_map_.end()) {
      *result = nullptr;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
      return Status::IOError(fname, "File not found");
    }

    *result = new RandomAccessFileImpl(file_map_[fname]);
    return Status::OK();
  }

<<<<<<< HEAD
  virtual Status NewWritableFile(const std::string& fname,
                                 WritableFile** result) {
    MutexLock lock(&mutex_);
    if (file_map_.find(fname) != file_map_.end()) {
      DeleteFileInternal(fname);
    }

    FileState* file = new FileState();
    file->Ref();
    file_map_[fname] = file;
=======
  Status NewWritableFile(const std::string& fname,
                         WritableFile** result) override {
    MutexLock lock(&mutex_);
    FileSystem::iterator it = file_map_.find(fname);

    FileState* file;
    if (it == file_map_.end()) {
      // File is not currently open.
      file = new FileState();
      file->Ref();
      file_map_[fname] = file;
    } else {
      file = it->second;
      file->Truncate();
    }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    *result = new WritableFileImpl(file);
    return Status::OK();
  }

<<<<<<< HEAD
  virtual Status NewAppendableFile(const std::string& fname,
                                   WritableFile** result) {
    MutexLock lock(&mutex_);
    FileState** sptr = &file_map_[fname];
    FileState* file = *sptr;
    if (file == NULL) {
=======
  Status NewAppendableFile(const std::string& fname,
                           WritableFile** result) override {
    MutexLock lock(&mutex_);
    FileState** sptr = &file_map_[fname];
    FileState* file = *sptr;
    if (file == nullptr) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
      file = new FileState();
      file->Ref();
    }
    *result = new WritableFileImpl(file);
    return Status::OK();
  }

<<<<<<< HEAD
  virtual bool FileExists(const std::string& fname) {
=======
  bool FileExists(const std::string& fname) override {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    MutexLock lock(&mutex_);
    return file_map_.find(fname) != file_map_.end();
  }

<<<<<<< HEAD
  virtual Status GetChildren(const std::string& dir,
                             std::vector<std::string>* result) {
    MutexLock lock(&mutex_);
    result->clear();

    for (FileSystem::iterator i = file_map_.begin(); i != file_map_.end(); ++i){
      const std::string& filename = i->first;
=======
  Status GetChildren(const std::string& dir,
                     std::vector<std::string>* result) override {
    MutexLock lock(&mutex_);
    result->clear();

    for (const auto& kvp : file_map_) {
      const std::string& filename = kvp.first;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

      if (filename.size() >= dir.size() + 1 && filename[dir.size()] == '/' &&
          Slice(filename).starts_with(Slice(dir))) {
        result->push_back(filename.substr(dir.size() + 1));
      }
    }

    return Status::OK();
  }

<<<<<<< HEAD
  void DeleteFileInternal(const std::string& fname) {
=======
  void DeleteFileInternal(const std::string& fname)
      EXCLUSIVE_LOCKS_REQUIRED(mutex_) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (file_map_.find(fname) == file_map_.end()) {
      return;
    }

    file_map_[fname]->Unref();
    file_map_.erase(fname);
  }

<<<<<<< HEAD
  virtual Status DeleteFile(const std::string& fname) {
=======
  Status DeleteFile(const std::string& fname) override {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    MutexLock lock(&mutex_);
    if (file_map_.find(fname) == file_map_.end()) {
      return Status::IOError(fname, "File not found");
    }

    DeleteFileInternal(fname);
    return Status::OK();
  }

<<<<<<< HEAD
  virtual Status CreateDir(const std::string& dirname) {
    return Status::OK();
  }

  virtual Status DeleteDir(const std::string& dirname) {
    return Status::OK();
  }

  virtual Status GetFileSize(const std::string& fname, uint64_t* file_size) {
=======
  Status CreateDir(const std::string& dirname) override { return Status::OK(); }

  Status DeleteDir(const std::string& dirname) override { return Status::OK(); }

  Status GetFileSize(const std::string& fname, uint64_t* file_size) override {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    MutexLock lock(&mutex_);
    if (file_map_.find(fname) == file_map_.end()) {
      return Status::IOError(fname, "File not found");
    }

    *file_size = file_map_[fname]->Size();
    return Status::OK();
  }

<<<<<<< HEAD
  virtual Status RenameFile(const std::string& src,
                            const std::string& target) {
=======
  Status RenameFile(const std::string& src,
                    const std::string& target) override {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    MutexLock lock(&mutex_);
    if (file_map_.find(src) == file_map_.end()) {
      return Status::IOError(src, "File not found");
    }

    DeleteFileInternal(target);
    file_map_[target] = file_map_[src];
    file_map_.erase(src);
    return Status::OK();
  }

<<<<<<< HEAD
  virtual Status LockFile(const std::string& fname, FileLock** lock) {
=======
  Status LockFile(const std::string& fname, FileLock** lock) override {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    *lock = new FileLock;
    return Status::OK();
  }

<<<<<<< HEAD
  virtual Status UnlockFile(FileLock* lock) {
=======
  Status UnlockFile(FileLock* lock) override {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    delete lock;
    return Status::OK();
  }

<<<<<<< HEAD
  virtual Status GetTestDirectory(std::string* path) {
=======
  Status GetTestDirectory(std::string* path) override {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    *path = "/test";
    return Status::OK();
  }

<<<<<<< HEAD
  virtual Status NewLogger(const std::string& fname, Logger** result) {
=======
  Status NewLogger(const std::string& fname, Logger** result) override {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    *result = new NoOpLogger;
    return Status::OK();
  }

 private:
  // Map from filenames to FileState objects, representing a simple file system.
  typedef std::map<std::string, FileState*> FileSystem;
<<<<<<< HEAD
  port::Mutex mutex_;
  FileSystem file_map_;  // Protected by mutex_.
=======

  port::Mutex mutex_;
  FileSystem file_map_ GUARDED_BY(mutex_);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};

}  // namespace

<<<<<<< HEAD
Env* NewMemEnv(Env* base_env) {
  return new InMemoryEnv(base_env);
}
=======
Env* NewMemEnv(Env* base_env) { return new InMemoryEnv(base_env); }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

}  // namespace leveldb
