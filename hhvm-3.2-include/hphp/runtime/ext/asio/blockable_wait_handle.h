/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-2014 Facebook, Inc. (http://www.facebook.com)     |
   | Copyright (c) 1997-2010 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

#ifndef incl_HPHP_EXT_ASIO_BLOCKABLE_WAIT_HANDLE_H_
#define incl_HPHP_EXT_ASIO_BLOCKABLE_WAIT_HANDLE_H_

#include "hphp/runtime/base/base-includes.h"
#include "hphp/runtime/ext/asio/waitable_wait_handle.h"

namespace HPHP {
///////////////////////////////////////////////////////////////////////////////
// class BlockableWaitHandle

/**
 * A blockable wait handle is a wait handle that can be blocked by a waitable
 * wait handle it is waiting for. Once a wait handle blocking this wait handle
 * is finished, a notification is received and the operation can be resumed.
 */
FORWARD_DECLARE_CLASS(BlockableWaitHandle);
class c_BlockableWaitHandle : public c_WaitableWaitHandle {
 public:
  DECLARE_CLASS_NO_SWEEP(BlockableWaitHandle)

  explicit c_BlockableWaitHandle(Class* cls =
      c_BlockableWaitHandle::classof())
    : c_WaitableWaitHandle(cls)
  {}
  ~c_BlockableWaitHandle() {}

 public:
  static constexpr ptrdiff_t nextParentOff() {
    return offsetof(c_BlockableWaitHandle, m_nextParent);
  }

  static void UnblockChain(c_BlockableWaitHandle* parentChain);
  c_BlockableWaitHandle* unblock();
  c_BlockableWaitHandle* getNextParent() { return m_nextParent; }

  void exitContextBlocked(context_idx_t ctx_idx);

  static const int8_t STATE_BLOCKED = 2;

 protected:
  void blockOn(c_WaitableWaitHandle* child);
  void detectCycle(c_WaitableWaitHandle* child) const;
  ObjectData* createCycleException(c_WaitableWaitHandle* child) const;
};

///////////////////////////////////////////////////////////////////////////////
}

#include "hphp/runtime/ext/asio/blockable_wait_handle-inl.h"

#endif // incl_HPHP_EXT_ASIO_BLOCKABLE_WAIT_HANDLE_H_
