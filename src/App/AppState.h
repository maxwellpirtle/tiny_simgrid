#ifndef APPSTATE_H
#define APPSTATE_H

#include "Actor.h"
#include "Mailbox.h"
#include "Transition.h"
#include <deque>

namespace app {
class AppState {
public:
  explicit AppState(std::deque<app::Actor>&& actors, std::deque<app::Mailbox>&& mailboxes)
      : actors_(std::move(actors)), mailboxes_(std::move(mailboxes))
  {
  }
  AppState(const AppState&)            = default;
  AppState& operator=(AppState const&) = default;
  AppState(AppState&&) noexcept        = default;
  ~AppState()                          = default;

  std::deque<std::string> get_enabled_transitions();
  AppState execute_transition(std::string const& tr_tag);

private:
  std::deque<Actor> actors_;
  std::deque<Mailbox> mailboxes_;
};

} // namespace app

#endif // APPSTATE_H