#ifndef TRANSITION_H
#define TRANSITION_H

#include <string>

namespace app {

class Transition {
public:
  int actor_id    = 0;
  unsigned int id = 0;

  bool executed    = false;
  int read_write   = 0;  // default value is read (= 0), write =1, if mutex lock = 2, unlock = 3
  int access_var   = 0;  // default this transition access variable 0
  int mailbox_id   = -1; // used to present mailbox id for send and receive transition
  int commId       = -1; // the communication that i concern (used for only send, receive, wait and test operations)
  std::string type = ""; // Isend, Ireceive, test, wait
  int mutexID      = -1; // the mutex i touch
  int lockId       = -1; // used for Mtest and Mwait

  bool isDependent(const Transition& other) const;

  bool operator<(const Transition& other) const;

  Transition() = default;
  Transition(int read_write, int access_var);
  Transition(int mbId, int commId, const std::string& type);
  Transition(const Transition&)            = default;
  Transition& operator=(Transition const&) = default;
  Transition(Transition&&)                 = default;
  ~Transition()                            = default;

  inline std::string get_trans_tag() const { return trans_tag; }
  inline void set_trans_tag(const int actor_id, const unsigned int trans_id)
  {
    trans_tag = std::to_string(actor_id) + std::to_string(trans_id);
  }

private:
  std::string trans_tag = "";
};

} // namespace app

#endif // TRANSITION_H
