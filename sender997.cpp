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

int main(){
  int qid = msgget(ftok(".",'z'), 0);
  int event = 0; //Random number
  int marker = 997;

  struct buf {
		long mtype; // required
		char greeting[50]; // mesg content
    int senderID = 997;
    bool terminated = false;
    int event;

	};
	buf msg;//used to send events
  buf rcv; //used for receiving messages

	int size = sizeof(msg)-sizeof(long);

  bool sending = true;
  bool rcv1_terminated = false;
  bool rcv2_terminated = false;
  bool waitFor2Messages = true;
  bool received1 = false;
  bool received2 = false;
  bool stay;
  int timer = 1;

while(sending){

  if(timer%1000000 == 0){//if statement to slow down the rate of sent messages
    event = randomNumberGenerator(); //generate random number

    if(event < 100){ //terminates if event is smaller than 100
      strcpy(msg.greeting, "Sender 997 Terminating");
      msg.mtype = 100; //for angel
      msgsnd(qid, (struct msgbuf *)&msg, size, 0);
      msg.mtype = 90; //for chris main
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
      msg.mtype = 90;
      msgsnd(qid, (struct msgbuf *)&msg, size, 0);
      cout << getpid() << ": sent message to receiver 1" << endl;
      }

      if(rcv2_terminated == false){ //sending to rcv 2
      msg.mtype = 100;
      msgsnd(qid, (struct msgbuf *)&msg, size, 0);
      cout << getpid() << ": sent message to receiver 2" << endl;
      }

    }



    cout << "looking for acknowledgements" << endl;

    if(waitFor2Messages == true){ //have to wait for 2 messages
      stay = true;
      cout << "waiting for acknowledgement from both receivers. . ." << endl;
      while(stay){

        cout << "waitFor2Messages" << waitFor2Messages << endl; //tester
        msgrcv(qid, (struct msgbuf *)&rcv, size, 997, 0); // read mesg
        if(rcv.senderID == 1){
          cout << getpid() << ": gets message from receiver 1" << endl;
          cout << "message: " << rcv.greeting << endl;
          cout << "sender id: " << rcv.senderID << endl;
          received1 = true; //received message from receiver 1
        }
        if(rcv.senderID == 2){
          cout << getpid() << ": gets message from receiver 2" << endl;
          cout << "message: " << rcv.greeting << endl;
          cout << "sender id: " << rcv.senderID << endl;
          received2 = true; //received message from receiver 2
        }

        if(rcv.terminated ==true){
          if(rcv.senderID == 1){
          cout << "receiver 1 terminated" << endl;
          rcv1_terminated = true;
          stay = false;
          }
          else if(rcv.senderID == 2){
          cout << "receiver 2 terminated" << endl;
          rcv2_terminated = true;
          stay = false;
          }
          waitFor2Messages = false;
        }

        if(received1 == true && received2 == true){ //received both
          stay = false;
          received1 = false; //reset both received booleans for next round
          received2 = false;
        }

        //break if it receives message from rcv1 and rc2 terminated already
        if(received1 == true && rcv2_terminated ==true){
          stay = false;
        }
      }
    }
    else{ //dont have to wait for both messages
      cout << "waiting for one receiver. . . " << endl;
      msgrcv(qid, (struct msgbuf *)&rcv, size, 997, 0); // read mesg
      if(rcv.senderID == 1 || rcv.senderID ==2){
      // don't read "fake" mesg
      cout << getpid() << ": gets message" << endl;
      cout << "message: " << rcv.greeting << endl;
      cout << "terminated: " << rcv.terminated << endl;
      cout << "sender id: " << rcv.senderID << endl;

      //check both for termination still
      //so you can stop sending messages if both terminate
      if(rcv.terminated ==true){
        if(rcv.senderID == 1){
        cout<< "receiver 1 terminated" << endl;
        rcv1_terminated = true;
        }
        else if(rcv.senderID == 2){
          cout << "receiver 2 terminated" << endl;
        rcv2_terminated = true;
        }
        //waitFor2Messages = false;
      }
      cout << getpid() << ": " << endl;

      }
    }


    timer = 1;
  }//end of if statement used to control speed
  timer++;
}//end of while loop



  //msgctl (qid, IPC_RMID, NULL);

  exit(0);
}
