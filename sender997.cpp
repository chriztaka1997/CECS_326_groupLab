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
  buf rcv; //used for receiving messages

	int size = sizeof(msg)-sizeof(long);

  bool sending = true;
  bool rcv1_terminated = false;
  bool rcv2_terminated = false;
  bool waitFor2Messages = true;
  bool received1 == false;
  bool received2 == false;
  bool stay;
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
      strcpy(msg.greeting, "I am sender 997");

      if(rcv1_terminated == false){ //sending to rcv 1
      msg.mtype = 100;
      msgsnd(qid, (struct msgbuf *)&msg, size, 0);
      }

      if(rcv2rcv2_terminated == false){ //sending to rcv 2
      msg.mtype = 90;
      msgsnd(qid, (struct msgbuf *)&msg, size, 0);
      }

      cout << getpid() << ": sent messages" << endl;
    }



    cout << "looking for acknowledgements" << endl;

    if(waitFor2Messages == true){ //have to wait for 2 messages
      stay = true;
      while(stay){
        msgrcv(qid, (struct msgbuf *)&rcv, size, 997, 0); // read mesg
        if(rcv.senderID == 1){
          cout << getpid() << ": gets message" << endl;
          cout << "message: " << rcv.greeting << endl;
          cout << "sender id: " << rcv.senderID << endl;
          received1 == true
        }
        if(rcv.senderID == 2){
          cout << getpid() << ": gets message" << endl;
          cout << "message: " << rcv.greeting << endl;
          cout << "sender id: " << rcv.senderID << endl;
          received2== true
        }
        if(received1 == true && received2 == true){
          stay = false;
        }
      }
    }
    else{ //dont have to wait for both messages
      msgrcv(qid, (struct msgbuf *)&rcv, size, 997, 0); // read mesg
      if(rcv.senderID == 1 || rcv.senderID ==2){
      // don't read "fake" mesg
      cout << getpid() << ": gets message" << endl;
      cout << "message: " << rcv.greeting << endl;
      cout << "sender id: " << rcv.senderID << endl;

      if(rcv.terminated ==true){
        if(rcv.senderID == 1){
        rcv1_terminated = true;
        }
        else if(rcv.senderID == 2){
        rcv2_terminated == true;
        }
        waitFor2Messages = false;
      }
      cout << getpid() << ": " << endl;

      }
    }



  }//end of first if statement
}



  //msgctl (qid, IPC_RMID, NULL);

  exit(0);
}
