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

int randomNumberGenerator();

int main() {

  int qid = msgget(ftok(".",'z'), 0);

  struct buf {
		long mtype; // required
		char greeting[50]; // mesg content
    int senderID;
    bool terminated;
    int event;
	};
	buf msg;
  buf msg2;
	int size = sizeof(msg)-sizeof(long);

  msg.mtype = 110;
  strcpy(msg.greeting, "Hello my name is 251");
  bool sending = true;
  long count = 1;
  msg.senderID = 251;
  msg.terminated = false;
  msg.event = randomNumberGenerator();

  msg2.mtype = 110;
  strcpy(msg2.greeting, "Term");
  msg2.senderID = 251;
  msg2.terminated = true;
  get_info(qid, (struct msgbuf *)&msg2, size, msg2.mtype);

  while(sending)
  {
    if(count % 1000000 == 0)
    {
      cout << "Sender251: sending: " << msg.greeting << endl;
      msgsnd(qid, (struct msgbuf *)&msg, size, 0);
      count = 0;
    }
    count++;
  }
}

int randomNumberGenerator(){
  int randomNumber = 0;
  randomNumber = rand()%4294967296 + 1; //random number from 1 to 2^32
  if(randomNumber%997 == 0){
  return randomNumber;
  }
  else{
    randomNumberGenerator();
  }
}
