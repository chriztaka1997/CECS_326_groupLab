/*
Ryan Valdriz
CECS 326 TuTh
Group Project
sender 997
*/

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
#include <string>

using namespace std;

int randomNumberGenerator();
void killQueue();

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

void killQueue(int qid){
  msgctl (qid, IPC_RMID, NULL);
}

int main(){
  int qid = msgget(ftok(".",'u'), 0);
  int event = 0; //Random number
  int marker = 997;

  struct buf {
		long mtype; // required
		char greeting[50]; // mesg content
    int senderID = 997;
    int event;
    bool terminated = false;
	};
	buf msg;//used to send events
  buf rcv1; //used to receive acknowledgements from receiver 1
  buf rcv2; //used to receive acknowledgements from receiver 2

	int size = sizeof(msg)-sizeof(long);

  bool sending = true;
  int timer = 0;

while(sending){
  
  if(timer%100000 == 0){
  event = randomNumberGenerator(); //generate random number
  if(event < 100){ //terminates if event is smaller than 100
    strcpy(msg.greeting, "Sender 997 Terminating");
    msg.mtype = 100; //for angel
    msgsnd(qid, (struct msgbuf *)&msg, size, 0);
    msg.mtype = 200; //for chris main
    msgsnd(qid, (struct msgbuf *)&msg, size, 0);
    //killQueue(qid);//kill id
    sending = false;
  }
  else{ // else send event to receivers
    cout << "event: " << event << endl;
    msg.event = event;
    cout << "msg.event: " << msg.event << endl;
    msg.mtype = 100;
    strcpy(msg.greeting, "I am sender 997");
    msgsnd(qid, (struct msgbuf *)&msg, size, 0);
    msg.mtype = 90;
    msgsnd(qid, (struct msgbuf *)&msg, size, 0);
    cout << getpid() << ": sent messages" << endl;
  }



  cout << "looking for acknowledgements" << endl;


  msgrcv(qid, (struct msgbuf *)&rcv1, size, 997, 0); // read mesg
  if(rcv1.senderID == 2){
              // don't read "fake" mesg
  cout << getpid() << ": gets message" << endl;
  cout << "message: " << rcv1.greeting << endl;
  cout << "sender id: " << rcv1.senderID << endl;
  cout << getpid() << ": " << endl;

  }
  else{
      cout << "Received wrong message" << endl;
  }

  }//end of first if statement
}



  msgctl (qid, IPC_RMID, NULL);

  exit(0);
}
