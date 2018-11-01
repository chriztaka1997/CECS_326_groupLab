#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include "get_info.h"

using namespace std;

int main() {

  int qid = msgget(ftok(".",'k'), 0);

  struct buf {
		long mtype; // required
		char greeting[50]; // mesg content
    int senderID;
    bool terminated;
	};
	buf msg;
  buf msg2;
	int size = sizeof(msg)-sizeof(long);

  msg.mtype = 251;
  strcpy(msg.greeting, "Hello my name is 251");
  bool sending = true;
  long count = 1;
  msg.senderID = 251;
  msg.terminated = false;

  msg2.mtype = 251;
  strcpy(msg2.greeting, "Hello my name is 251");
  msg2.senderID = 251;
  msg2.terminated = true;
  get_info(qid, (struct msgbuf *)&msg2, size, msg2.mtype);

  while(sending)
  {
    if(count % 1000000000 == 0)
    {
      cout << "Sender251: sending: " << msg.greeting << endl;
      msgsnd(qid, (struct msgbuf *)&msg, size, 0);
      count = 0;
    }
    count++;
  }
}
