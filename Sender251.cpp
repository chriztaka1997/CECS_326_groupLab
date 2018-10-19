#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <thread>
#include <chrono>

using namespace std;

int main() {

  int qid = msgget(ftok(".",'f'), 0);

  struct buf {
		long mtype; // required
		char greeting[50]; // mesg content
	};
	buf msg;
	int size = sizeof(msg)-sizeof(long);

  msg.mtype = 251;
  strcpy(msg.greeting, "Hello my name is 251");
  bool sending = true;

  while(sending)
  {
    this_thread::sleep_for (chrono::seconds(1));
    cout << "Sender251: sending" << endl;
    msgsnd(qid, (struct msgbuf *)&msg, size, 0);
  }
  msg.mtype = -1;
  msgsnd(qid, (struct msgbuf *)&msg, size, 0);
}
