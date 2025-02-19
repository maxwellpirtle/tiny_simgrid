#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include "../App/AppSide.h"
#include <algorithm>
#include <iostream>
#include <queue>

namespace uc {

class UnfoldingEvent;

using EventSet = std::deque<UnfoldingEvent*>;
using AppSide  = app::AppSide;

class App {
public:
  static std::shared_ptr<AppSide> app_side_;
  static void set_app_side(std::shared_ptr<AppSide> app_side);
};

class Util {
public:
  static int INT_DEF;
  static void get_transition_detail(std::string trans_tag, std::string& type, int& actor_id = INT_DEF,
                                    int& mb_id = INT_DEF, int& trans_id = INT_DEF);
};

class EvtSetTools {
public:
  static bool contains(const EventSet events, const UnfoldingEvent* e);
  static UnfoldingEvent* find(const EventSet events, const UnfoldingEvent* e);
  static void subtract(EventSet& events, EventSet const& otherSet);
  static bool depends(EventSet const& events, EventSet const& otherSet);
  static bool isEmptyIntersection(EventSet evtS1, EventSet evtS2);
  static EventSet makeUnion(EventSet s1, EventSet s2);
  static void pushBack(EventSet& events, UnfoldingEvent* e);
  static void remove(EventSet& events, UnfoldingEvent* e);
  static EventSet minus(EventSet events, UnfoldingEvent* e);
  static EventSet plus(EventSet events, UnfoldingEvent* e);
};

typedef struct s_evset_in {
  EventSet causuality_events;
  EventSet cause;
  EventSet ancestorSet;
} s_evset_in_t;

class Configuration {
public:
  EventSet events_;
  EventSet maxEvent;         // Events recently added to events_
  EventSet actorMaxEvent;    // maximal events of the actors in current configuration
  UnfoldingEvent* lastEvent; // The last added event

  Configuration plus_config(UnfoldingEvent*) const;
  void createEvts(Configuration C, EventSet& result, const std::string& trans_tag, s_evset_in_t ev_sets, bool chk,
                  UnfoldingEvent* immPreEvt);
  void updateMaxEvent(UnfoldingEvent*);         // update maximal events of the configuration and actors
  UnfoldingEvent* findActorMaxEvt(int actorId); // find maximal event of a Actor whose id = actorId

  UnfoldingEvent* findTestedComm(const UnfoldingEvent* testEvt); // find comm tested by action testTrans

  Configuration()                                = default;
  Configuration(const Configuration&)            = default;
  Configuration& operator=(Configuration const&) = default;
  Configuration(Configuration&&) noexcept        = default;
  ~Configuration()                               = default;
};

class UnfoldingEvent {
public:
  int id = -1;
  EventSet causes; // used to store directed ancestors of event e
  UnfoldingEvent(unsigned int nb_events, std::string const& trans_tag, EventSet const& causes, int sid = -1);
  UnfoldingEvent(const UnfoldingEvent&)            = default;
  UnfoldingEvent& operator=(UnfoldingEvent const&) = default;
  UnfoldingEvent(UnfoldingEvent&&)                 = default;
  ~UnfoldingEvent()                                = default;

  EventSet getHistory() const;

  bool isConflict(UnfoldingEvent* event, UnfoldingEvent* otherEvent) const;
  bool concernSameComm(const UnfoldingEvent* event, const UnfoldingEvent* otherEvent) const;

  // check otherEvent is in my history ?
  bool inHistory(UnfoldingEvent* otherEvent) const;

  bool isImmediateConflict1(UnfoldingEvent* evt, UnfoldingEvent* otherEvt) const;

  bool conflictWithConfig(UnfoldingEvent* event, Configuration const& config) const;
  bool operator==(const UnfoldingEvent& other) const;
  void print() const;

  inline int get_state_id() const { return state_id; }
  inline void set_state_id(int sid) { state_id = sid; }

  inline std::string get_transition_tag() const { return transition_tag; }
  inline void set_transition_tag(std::string const& tr_tag) { transition_tag = tr_tag; }

private:
  int state_id{-1};
  std::string transition_tag{""}; // The tag of the last transition that lead to creating the event
  bool transition_is_IReceive(const UnfoldingEvent* testedEvt, const UnfoldingEvent* SdRcEvt) const;
  bool transition_is_ISend(const UnfoldingEvent* testedEvt, const UnfoldingEvent* SdRcEvt) const;
  bool check_tr_concern_same_comm(bool& chk1, bool& chk2, UnfoldingEvent* evt1, UnfoldingEvent* evt2) const;
};

} // namespace uc

#endif // GLOBAL_HPP
