#include "Mailbox.h"

namespace app {

bool Mailbox::operator<(const Mailbox& other) const
{
  return (this->id < other.id);
}

void Mailbox::update(const Transition& t)
{
  Communication comm;

  comm.actorId = t.actor_id;
  comm.commId  = t.commId;

  if (t.type == "Isend") {
    for (unsigned long i = 0; i < this->nbReceive; i++)
      if (receiveList[i].status == "pending") {
        receiveList[i].status = "ready";
        comm.status           = "ready";

        break;
      }
    sendList[nbSend] = comm;
    this->nbSend++;
  }

  if (t.type == "Ireceive") {
    for (unsigned long i = 0; i < this->nbSend; i++)
      if (sendList[i].status == "pending") {

        sendList[i].status = "ready";
        comm.status        = "ready";

        break;
      }
    receiveList[nbReceive] = comm;
    nbReceive++;
  }
};

bool Mailbox::checkComm(const Transition& t)
{

  for (unsigned long i = 0; i < nbSend; i++)
    if (t.actor_id == sendList[i].actorId && t.commId == sendList[i].commId && sendList[i].status == "ready")
      return true;

  for (unsigned long i = 0; i < nbReceive; i++)
    if (t.actor_id == receiveList[i].actorId && t.commId == receiveList[i].commId && receiveList[i].status == "ready")
      return true;

  return false;
}

} // namespace app
