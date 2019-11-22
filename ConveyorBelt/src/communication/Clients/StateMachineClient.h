/*
 * StateMachineClient.h
 *
 *  Created on: 14.05.2018
 *      Author: Johannes Kruber
 */

#ifndef COMMUNICATION_CLIENTS_STATEMACHINECLIENT_H_
#define COMMUNICATION_CLIENTS_STATEMACHINECLIENT_H_
#include "../../Puk.h"
#include "../Client.h"
#include "../../automaton/StateMachine.h"
#include "../../serial/SerialController.h"
#include <thread>
namespace communication
{

class StateMachineClient: public Client
{
private:
	automaton::StateMachine *machine;
	automaton::StateMachine *tmpMachine;
	automaton::StateMachine *calibrateMachine;
	SerialController* serialController;
	std::thread *serialThread;
	automaton::CalibrationData calData;
	bool actorsave[ACTOR_ARRAY_SIZE];
	void updateActordata(void);
	int hightCalValue;
	int calibrateCounter;
	int sendDataCounter;
	bool saveStopButton;
	bool checkSendNewActor();
	int normalizeHightValue(void);
	void chooseStateMachine(void);
	bool rampFull;
	bool beltFree;
	bool metallexp;
	bool eStop;
	Puk sendPuk;
	bool toggleBoolTest;




public:
	void send(void);
	//false = StateMachine1, true = Statemachine2
	StateMachineClient(int channelId, bool stateMachine, int pulseChannel);
	virtual ~StateMachineClient();
	void setBelt2exp(bool metall);
	void sendMsg(int i);

protected:
	virtual void work();
};

} /* namespace communication */

#endif /* COMMUNICATION_CLIENTS_STATEMACHINECLIENT_H_ */
