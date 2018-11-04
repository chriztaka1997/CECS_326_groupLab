#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

using namespace std;

void sendDeathMessage(int);

int main() {
  int qid = msgget(ftok(".",'j'), IPC_EXCL|IPC_CREAT|0600);

  struct buf {
		long mtype; // required
		char greeting[50]; // mesg content
    int senderID;
    bool terminated = false;
    int event;
	};
	buf msg;
  buf msg2;
	int size = sizeof(msg)-sizeof(long);

  msg2.mtype = 997;
  msg2.senderID = 1;
  strcpy(msg2.greeting, "Hi i am reciever one acknowledgement");

  msg.terminated = false;
  bool t251 = false;
  bool t997 = false;
  msg.mtype = 251;
  strcpy(msg.greeting, "Hello my name is reciever one");
  bool sending = true;
  int count = 0;

while(sending)
  {
    msgrcv (qid, (struct msgbuf *)&msg, size, 110, 0);
    if(msg.senderID > 0)
    {
      cout << msg.greeting << " from: " << msg.senderID << " event: " << msg.event << " M[0]" << (msg.greeting[0] == 'T')<< endl;
      if(msg.senderID == 997 && msg.terminated == true)
      {
        t997 = true;
        cout << "Reached here 1" << endl;
      }
      else if(msg.senderID == 251 && msg.greeting[0] == 'T')
      {
        t251 = true;
        cout << "Reached here 2" << endl;
      }
      if(msg.senderID == 997)
        msgsnd(qid, (struct msgbuf *)&msg2, size, 0);
    }
    if(t997 == true && t251 == true)
    {
      sendDeathMessage(qid);
      break;
    }
    msg.senderID = 0;
  }
  cout << "Finished running" <<endl;
  exit(0);
}

void sendDeathMessage(int qid)
{
  msgctl (qid, IPC_RMID, NULL);
}
