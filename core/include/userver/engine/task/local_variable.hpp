#pragma once

#include <userver/engine/task/local_storage.hpp>

namespace engine {

class TaskLocalVariableAny {
 public:
  TaskLocalVariableAny(const TaskLocalVariableAny&) = delete;
  TaskLocalVariableAny(TaskLocalVariableAny&&) = delete;

  TaskLocalVariableAny& operator=(const TaskLocalVariableAny&) = delete;
  TaskLocalVariableAny& operator=(TaskLocalVariableAny&&) = delete;

 protected:
  TaskLocalVariableAny();

  static impl::LocalStorage& GetCurrentLocalStorage();
  size_t CoroVariableIndex() const { return coro_variable_index_; }

 private:
  const size_t coro_variable_index_;
};

/** @brief TaskLocalVariable is a per-coroutine variable of arbitrary type.
 *
 * It is an alternative to thread_local, but per-coro instead of per-thread.
 * Every variable instance is allocated in heap.
 *
 * NOTE: currently T can be a non-engine type, it must not call any
 * coroutine-specific code from ~T as ~T is called outside of any coroutine.
 */
template <typename T>
class TaskLocalVariable final : public TaskLocalVariableAny {
 public:
  /* Get current coroutine's instance of variable.
   * Must be called from a coroutine, otherwise it is UB.
   */
  T& operator*();

  T* operator->();
};

template <typename T>
T& TaskLocalVariable<T>::operator*() {
  auto& local_storage = GetCurrentLocalStorage();
  return *local_storage.template Get<T>(CoroVariableIndex());
}

template <typename T>
T* TaskLocalVariable<T>::operator->() {
  return &(**this);
}

}  // namespace engine