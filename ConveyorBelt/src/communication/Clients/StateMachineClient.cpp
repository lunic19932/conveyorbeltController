/*
 * StateMachineClient.cpp
 *
 *  Created on: 14.05.2018
 *      Author: Johannes Kruber
 */

#include "StateMachineClient.h"
#include "../../automaton/StateMachine2.h"
#include "../../automaton/StateMachine3.h"
#include "../../automaton/StateMachineCalibration.h"
#include "../Message.h"
#include "../../serial/SerialMsg.h"

#include <iostream>

#include "../../automaton/StateMachine1.h"
#define TIKCOUNTERCALIBRATION 300 //Tiks(3s)
#define NORM_ROUND_TOLERANCE 500
#define TIMETOSENDDATA 50

namespace communication {

//false = StateMachine1, true = Statemachine2
StateMachineClient::StateMachineClient(int channelId, bool stateMachine,
		int pulseChannelId)
		: Client(channelId), hightCalValue(1), calibrateCounter(0),sendDataCounter(0), saveStopButton(
				false), rampFull(false), beltFree(false), metallexp(true), eStop(
				false),toggleBoolTest(false) 
				
{
	sendPuk.id = -1;
	calData.reset();
	if (stateMachine)
	{
		machine = new automaton::StateMachine2(&calData);
	} else
	{
		std::cout << "Starte Statemachin1" << std::endl;
		machine = new automaton::StateMachine1(&calData);
	}
	tmpMachine = machine;
	calibrateMachine = new automaton::StateMachineCalibration(&calData);
	serialController = new SerialController(pulseChannelId);
	serialThread = new std::thread(std::ref(*serialController));
}

StateMachineClient::~StateMachineClient() {
}

void StateMachineClient::work() {
	if (msg.senderId == HARDWARE_CONTR) {
		saveStopButton = msg.sensorData[MSG_BTN_STOP];
		if (machine->getType() == STATEMACHINECALIBRATION) {
			machine->updateSensorData(msg.sensorData, msg.hightSensor,
					msg.pulseType);
		} else {
			machine->updateSensorData(msg.sensorData, normalizeHightValue(),
					msg.pulseType);
		}

		for (int i = 0; i < ACTOR_ARRAY_SIZE; i++) {
			msg.actorStatus[i] = machine->actorState[i];
		}

		if (machine->checkError()||!serialController->isConnected()) {
			msg.actorStatus[MSG_MOTOR_STOP] = true;
			machine->errorState(true);
		} else {
			machine->errorState(false);
			//machine->automatonStep();
			//updateActordata();
		}
		machine->automatonStep();
		updateActordata();
		if(msg.sensorData[MSG_BTN_RESET]){
			rampFull = !machine->getRampFull();
			beltFree = !machine->getBeltFree();
			metallexp = ! machine->getMetallExp();
			eStop = !machine->getEstop();
			serialController->reset();
		}
		if(machine->getType()==STATEMACHINE1 && !msg.sensorData[MSG_SN_LB_END]){
			SerialMsg serialMsg;
			serialMsg.header.type = BELTSTATUSREQ;
			serialMsg.header.version = 0;
			serialController->sendMsg(serialMsg);
		}
		msg.receiverId = HARDWARE_CONTR;
	} else if (msg.senderId == PULSE_RECIEVER
			&& msg.pulseType == MSG_PULS_TIKTIMER) {

		for (int i = 0; i < ACTOR_ARRAY_SIZE; i++) {
			msg.actorStatus[i] = machine->actorState[i];
		}
		if (machine->checkError() ||!serialController->isConnected()) {
			machine->errorState(true);
			machine->automatonStep();
			updateActordata();
			msg.actorStatus[MSG_MOTOR_STOP] = true;
			msg.receiverId = HARDWARE_CONTR;
		} else {
			machine->makeTikStep();
			machine->errorState(false);
			updateActordata();
			chooseStateMachine();
			if (checkSendNewActor()) {
				msg.receiverId = HARDWARE_CONTR;
			} else {
				msg.receiverId = EMPTY;
			}
		}

		sendDataCounter++;
	} else if (msg.senderId == PULSE_RECIEVER
			&& msg.pulseType == MSG_PULS_SERIAL) {

		if (serialController->isConnected()
				&& !serialController->receiveEmpty()) {

			SerialMsg serialMsg = serialController->getNextMsg();
			switch (serialMsg.header.type) {
			case PUKMSG:
				machine->setPuk(serialMsg.puk);
//				std::cout << "pop PukMsg: " << std::endl;
				break;
			case EMGYSTOP:
				machine->setOtherEmergency(true);
//				std::cout << "pop Emgy Stop: " << std::endl;
				break;
			case EMGYSTOPCLEAR:
				machine->setOtherEmergency(false);
//				std::cout << "pop Emgy Clear: " << std::endl;
				break;
			case RAMPFULL:
				machine->setOtherRampFull(true);
//				std::cout << "pop FullRamp: " << std::endl;
				break;
			case RAMPFREE:
//				std::cout << "pop FreeRamp: " << std::endl;
				machine->setOtherRampFull(false);
				break;
			case BELTFREE:
//				std::cout << "pop Belt Free: " << std::endl;
				machine->setOtherBeltFree(true);
				break;
			case BELTFULL:
//				std::cout << "pop Belt Full: " << std::endl;
				machine->setOtherBeltFree(false);
				break;
			case METALREJECTED:
				machine->setOtherMetalExpected(false);
				break;
			case METALACCEPTED:
				machine->setOtherMetalExpected(true);
				break;
			case BELTSTATUSREQ:
				if(machine->getBeltFree()){
					serialMsg.header.type = BELTFREE;
				}else{
					serialMsg.header.type = BELTFULL;
				}
				serialMsg.header.version = 0;
				serialController->sendMsg(serialMsg);
				break;
			default:
//				std::cout << "default: " << std::endl;
				break;
			}
			msg.receiverId = EMPTY;
		}
		if (!serialController->isConnected()) {
//			std::cout << "SerialDisconected" << std::endl;
		}
		//std::cout << "Got something: "<< msg.senderId << std::endl;

	} else {
		//std::cout << "Got something: "<< msg.senderId << std::endl;
		msg.receiverId = EMPTY;
	}
	send();

	msg.senderId = MAIN_CNTR;

}

void StateMachineClient::send() {
	if(serialController->isConnected()){
		SerialMsg serialMsg;
			if (rampFull != machine->getRampFull()) {
				serialMsg.header.version = 0;
				if (machine->getRampFull()) {
//					std::cout << "add To Queue: rampFull" << std::endl;
					serialMsg.header.type = RAMPFULL;

				} else {
//					std::cout << "add To Queue: rampFree" << std::endl;
					serialMsg.header.type = RAMPFREE;
				}
				serialController->sendMsg(serialMsg);
			}
			if ((beltFree != machine->getBeltFree())
					&& machine->getBeltFree()) {
//				std::cout << "add To Queue: BeltFree" << std::endl;
				serialMsg.header.type = BELTFREE;
				serialMsg.header.version = 0;
				serialController->sendMsg(serialMsg);
			}
			if ((beltFree != machine->getBeltFree())
					&& !machine->getBeltFree()) {
//				std::cout << "add To Queue: BeltFull" << std::endl;
				serialMsg.header.type = BELTFULL;
				serialMsg.header.version = 0;
				serialController->sendMsg(serialMsg);
			}

			if (metallexp != machine->getMetallExp()) {

				serialMsg.header.version = 0;
				if (machine->getMetallExp())// && machine->getType() == STATEMACHINE2)
				{
//					std::cout << "add To Queue: MetalAccept" << std::endl;
					serialMsg.header.type = METALACCEPTED;
				} else {
//					std::cout << "add To Queue: MetalRejected" << std::endl;
					serialMsg.header.type = METALREJECTED;
				}
				serialController->sendMsg(serialMsg);
			}
			if (eStop != machine->getEstop()) {
				if(machine->getEstop()){
					serialMsg.header.type=EMGYSTOP;
				}else{
					serialMsg.header.type=EMGYSTOPCLEAR;
				}
				serialController->sendMsg(serialMsg);
			}
			if (sendPuk.id
					!= machine->getPuk().id&& machine->getType()==STATEMACHINE1) {
			//	std::cout << "add To Queue: Puk" << std::endl;
				serialMsg.header.type = PUKMSG;
				serialMsg.header.version = 0;
				serialMsg.puk = machine->getPuk();
				serialController->sendMsg(serialMsg);
			}

			rampFull = machine->getRampFull();
			beltFree = machine->getBeltFree();
			metallexp = machine->getMetallExp();
			eStop = machine->getEstop();
			sendPuk = machine->getPuk();
	}

}

void StateMachineClient::chooseStateMachine() {
	if (saveStopButton) {
		calibrateCounter = 0;
	} else {
		if (calibrateCounter > TIKCOUNTERCALIBRATION) {
			saveStopButton = true;

			if (machine->getType() == STATEMACHINE1
					|| machine->getType() == STATEMACHINE2) {
				std::cout << "Change to CalibrateMachine" << std::endl;
				machine = calibrateMachine;
				tmpMachine->reset();
			} else if (machine->getType() == STATEMACHINECALIBRATION) {
				std::cout << "Change to StateMachine" << std::endl;
				machine = tmpMachine;
				calibrateMachine->reset();
			}
		}
		calibrateCounter++;
	}

	if (machine->getType() == STATEMACHINECALIBRATION
			&& machine->machineComplete) {
		std::cout << "Change to StateMachine" << std::endl;
		calibrateMachine->reset();
		machine = tmpMachine;
	}
}
bool StateMachineClient::checkSendNewActor() {
	for (int i = 0; i < ACTOR_ARRAY_SIZE; i++) {
		if (actorsave[i] != machine->actorState[i]) {
			return true;
		}
	}
	return false;
}
void StateMachineClient::updateActordata() {
	for (int i = 0; i < ACTOR_ARRAY_SIZE; i++) {
		msg.actorStatus[i] = machine->actorState[i];
	}

	for (int i = 0; i < BLINK_ARRAY_SIZE; i++) {
		msg.blink[i] = machine->blink[i];
	}

	msg.adcEnable = machine->adcEnable;
}

int StateMachineClient::normalizeHightValue() {
	if (msg.hightSensor == -1) {
		msg.hightSensor = calData.hight_noPukRaw;
	}
	int result = (calData.hight_noPukRaw - msg.hightSensor)
			/ calData.hight_normQuotient;

//	int result = msg.hightSensor/calData.hight_normQuotient;

	//std::cout << "Hightvalue: " << msg.hightSensor << std::endl;

	int temp = result / 1000;
	temp *= 1000;

	if ((result - temp) >= NORM_ROUND_TOLERANCE) {
		result = temp + 1000;
	} else {
		result = temp;
	}

	//std::cout << "Normalized and rounded Hightvalue: " << result << std::endl;

	return result;
}
void StateMachineClient::sendMsg(int i) {
	SerialMsg serialMsg;
	serialMsg.header.type = i;
	serialMsg.header.version = 0;

	serialController->sendMsg(serialMsg);
}
void StateMachineClient::setBelt2exp(bool metall) {
	//machine->setOtherMetalExpected(metall);
	//machine->setOtherBeltFree(true);
	toggleBoolTest=!toggleBoolTest;
	machine->setOtherEmergency(toggleBoolTest);
}

} /* namespace communication */
