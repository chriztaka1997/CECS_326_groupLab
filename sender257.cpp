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

  msg.type = 257;
  //strcopy(msg.greeting,"This is 257");
  //msgsnd(qid,(struct msgbuf *)&msg,size, 0);

  //bool send = true;
  //msgsnd(qid, (struct msgbuf *)&msg, size, 0);
  
  while (send){
	cout<<"Send message"<<endl;
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	
	if(msgrcv(qid,(struct msgbuf *)&msg,size,99,0)>0){
		send = false;
	}

  }
	
  
}
