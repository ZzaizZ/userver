#include <engine/task/task.hpp>

#include <cassert>

#include <logging/log.hpp>

#include <engine/coro/pool.hpp>
#include "task_context.hpp"
#include "task_processor.hpp"

namespace engine {
namespace {

class CancelDisabledScope {
 public:
  explicit CancelDisabledScope(impl::TaskContext* context)
      : context_(context), old_value_(context_->SetCancellable(false)) {}
  ~CancelDisabledScope() { context_->SetCancellable(old_value_); }

 private:
  impl::TaskContext* context_;
  bool old_value_;
};

}  // namespace

Task::Task() = default;

Task::Task(impl::TaskContextHolder&& context_holder)
    : context_(context_holder.Release()) {
  context_->Wakeup(impl::TaskContext::WakeupSource::kNone);
}

Task::~Task() {
  if (context_) {
    // TODO: it is currently not possible to Wait() from outside of coro
    // use std::promise + Detach() instead
    // do it with caution though as you may get a deadlock
    // e.g. between global event thread pool and task processor
    auto caller_ctx = current_task::GetCurrentTaskContext();
    assert(caller_ctx);
    if (!IsFinished()) {
      context_->RequestCancel(Task::CancellationReason::kAbandoned);
    }

    CancelDisabledScope cancel_disabled(caller_ctx);
    while (!IsFinished()) Wait();
  }
}

Task::Task(Task&&) noexcept = default;
Task& Task::operator=(Task&&) noexcept = default;

bool Task::IsValid() const { return !!context_; }

Task::State Task::GetState() const {
  return context_ ? context_->GetState() : State::kInvalid;
}

bool Task::IsFinished() const { return context_ && context_->IsFinished(); }

void Task::Wait() const {
  assert(context_);
  context_->Wait();
}

void Task::Detach() && {
  if (context_) {
    assert(context_->use_count() > 0);
    context_->GetTaskProcessor().Adopt(std::move(context_));
  }
}

void Task::RequestCancel() {
  assert(context_);
  context_->RequestCancel(CancellationReason::kUserRequest);
}

Task::CancellationReason Task::GetCancellationReason() const {
  assert(context_);
  return context_->GetCancellationReason();
}

void Task::DoWaitUntil(Deadline deadline) const {
  assert(context_);
  context_->WaitUntil(std::move(deadline));
}

namespace current_task {

void CancellationPoint() {
  auto context = GetCurrentTaskContext();
  if (context->IsCancelRequested()) {
    context->Sleep({});
  }
}

TaskProcessor& GetTaskProcessor() {
  return GetCurrentTaskContext()->GetTaskProcessor();
}

ev::ThreadControl& GetEventThread() {
  return GetTaskProcessor().EventThreadPool().NextThread();
}

}  // namespace current_task
}  // namespace engine
