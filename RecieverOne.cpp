#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

using namespace std;

int main() {
  int qid = msgget(ftok(".",'g'), IPC_EXCL|IPC_CREAT|0600);

  struct buf {
		long mtype; // required
		char greeting[50]; // mesg content
    int senderID;
    bool terminated;
	};
	buf msg;
	int size = sizeof(msg)-sizeof(long);

  bool t251 = false;
  bool t997 = false;
  msg.mtype = 251;
  strcpy(msg.greeting, "Hello my name is reciever one");
  bool sending = true;
  int count = 0;

while(sending)
  {
    msgrcv (qid, (struct msgbuf *)&msg, size, 251, 0);
    if(msg.senderID > 0)
    {
      cout << msg.greeting << " from: " << msg.senderID << endl;
      if(msg.senderID == 251 && msg.terminated == true)
        t251 = true;
      else if(msg.senderID == 251 && msg.terminated == true)
        t997 = true;
    }
    if(t997 == true && t251 == true)
    {
      sendDeathMessage(qid);
      break;
    }
    msg.senderID = 0;
  }
}

void sendDeathMessage(int qid)
{
  msgctl (qid, IPC_RMID, NULL);
}
