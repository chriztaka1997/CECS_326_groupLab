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

//This will be reandom generator that will be assign to msg.event
int randomGenerator(){
	int randomNumber = 0;
	randomNumber = rand()%4294967296 + 1; //random number from 1 to 2^32
	return randomNumber;
}

//Main function
int main() {

  int qid = msgget(ftok(".",'z'), 0);

  struct buf {
		long mtype; // required
		char greeting[50]; // mesg content
		int senderID;
		int event;
		bool terminated;
  };

  buf msg;
  int size = sizeof(msg)-sizeof(long);

  msg.mtype = 100;				//assigning mtype to 100
	msg.senderID = 257;
	msg.terminated = false;	//Assign to folse because it is not going to terminate
  strcpy(msg.greeting,"This is 257");

  bool send = true;


  int count = 0;				//This will be the counter to check terminationof receiver 2
  int end_count = 5000; //Check termination of receiver 2 after 5000 message send

	int timer = 0;

  while (send){

		if (timer%1000000 == 0){	//This is will slow down the sending process
			cout<<"Send message to receiver 2"<<endl; //debug

			msg.event = randomGenerator();
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);

			if (count >= end_count){
				msgrcv(qid,(struct msgbuf *)&msg,size,99,0); //if receiver 2 terminated


				if(msg.mtype == 99){

					send = false;	//Get out of the loop
				}
				count = 0;
			}
			count++;
			timer = 0;
		}
		timer++;
  }

	//Send termination message to the message queue
	strcpy(msg.greeting,"257 terminated");
	msg.terminated = true;
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

}
