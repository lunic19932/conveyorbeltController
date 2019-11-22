/*
 * SerialController.cpp
 *
 *  Created on: 29.04.2018
 *      Author: LuisNickel
 */

#include "SerialController.h"
#include <fcntl.h>
#include <termios.h>
#include "../hardware/Bib/Util.h"
#include <iostream>
#include <sys/neutrino.h>
#include "../communication/PulseId.h"

#define QUEUETEST
#define TAKT 10 // in ms

SerialController::SerialController(int channelId) {
	int ret=0;
	SerialMsg buf;
	fdesc_ = open("/dev/ser4", O_RDWR);
	if (this->fdesc_ == -1) {
		std::cout << "error opening serial " << std::endl;
		exit(EXIT_FAILURE);
	}

	do{
		ret=readcond(fdesc_, &buf, sizeof(SerialMsg),
				sizeof(SerialMsg), 0, 1);
	}while(ret>0);

	connectionId_ = ConnectAttach_r(0, 0, channelId, 0, 0);
	if (connectionId_ < 0) {
		DBG_ERROR("Fail Channel create:Timer" + connectionId_);
		exit(-1);
	}
	init();

	connected_ = false;
	oldConnected_ = false;
	running_ = true;
	ackNmb_ = -1;
	version =0;

}

SerialController::~SerialController() {
	if (close(this->fdesc_) < 0) {
		std::cout << "error closing serial " << std::endl;
		exit(EXIT_FAILURE);
	}


}

void SerialController::init() {
	struct termios ts;
	int baud = B19200;
	tcflush(this->fdesc_, TCIOFLUSH);
	tcgetattr(this->fdesc_, &ts);
	cfsetispeed(&ts, baud);
	cfsetospeed(&ts, baud);
	ts.c_cflag &= ~CSIZE;
	ts.c_cflag &= ~CSTOPB;
	ts.c_cflag &= ~PARENB;
	ts.c_cflag |= CS8;
	ts.c_cflag |= CREAD;
	ts.c_cflag |= CLOCAL;
	tcsetattr(this->fdesc_, TCSANOW, &ts);

}

void SerialController::operator()() {
	while (running_) {
		send();
		receive();
		//std::this_thread::sleep_for(std::chrono::nanoseconds(100000));
	}
}

SerialMsg SerialController::getNextMsg() {
	SerialMsg msg;
	if (!receivedQueue_.empty()) {
		msg = receivedQueue_.front();
		receivedQueue_.pop();
	}

	return msg;
}
void SerialController::sendMsg(SerialMsg msg) {
	sendQueue_.push(msg);
}

bool SerialController::receiveEmpty() {
	return receivedQueue_.empty();
}

void SerialController::send() {
	SerialMsg sendMsg;
	if (!sendQueue_.empty()) {
		sendMsg = sendQueue_.front();
		sendMsg.header.msgNumber = msgNmb_;


	} else {
		sendMsg.header.type = PING;
		sendMsg.header.version=0;
	}
	sendMsg.header.ackNumber = ackNmb_;
	sendMsg.header.checkSum=buildCheckSum(sendMsg);
//	if (sendMsg.header.type != PING) {
//		std::cout << "Send: AckNumber: " << sendMsg.header.ackNumber << " checkSum: "
//				<< sendMsg.header.checkSum << " MsgNumber: "
//				<< sendMsg.header.msgNumber << " Type: "
//				<< (int) sendMsg.header.type << " Version: "
//				<< (int) sendMsg.header.version << std::endl;
//	}

	write(fdesc_, &sendMsg, sizeof(SerialMsg));
}

void SerialController::receive() {
	SerialMsg receivedMsg;
	receivedMsg.header.ackNumber=-1;
	receivedMsg.header.checkSum=-1;
	receivedMsg.header.checkSum=-1;
	receivedMsg.header.version=-1;
	int ret = readcond(fdesc_, &receivedMsg, sizeof(SerialMsg),
			sizeof(SerialMsg), 0, TAKT);
	bool checkSum=receivedMsg.header.checkSum == buildCheckSum(receivedMsg);
//	if (ret>0 && receivedMsg.header.type!=PING) {
//			std::cout << "Receive: AckNumber: " << receivedMsg.header.ackNumber << " checkSum: "
//					<< receivedMsg.header.checkSum << " MsgNumber: "
//					<< receivedMsg.header.msgNumber << " Type: "
//					<< (int) receivedMsg.header.type << " Version: "
//					<< (int) receivedMsg.header.version << std::endl;
//		}
	if (ret <= 0) {
		serialMutex_.lock();
		connected_ = false;
		serialMutex_.unlock();

	} else if (ret != sizeof(SerialMsg)) {
		serialMutex_.lock();
		connected_ = true;
		serialMutex_.unlock();
	} else if (checkSum) {
		serialMutex_.lock();
		connected_ = true;
		serialMutex_.unlock();
		if (receivedMsg.header.type == PING) {
			if (msgNmb_ == receivedMsg.header.ackNumber) {
				sendQueue_.pop();
				msgNmb_++;
			}
		} else if (receivedMsg.header.type > PING
				&& receivedMsg.header.type <= METALREJECTED) {
			if (msgNmb_ == receivedMsg.header.ackNumber) {
				sendQueue_.pop();
				msgNmb_++;

			}
			ackNmb_ = receivedMsg.header.msgNumber;
//			std::cout << "receive: " << (int) receivedMsg.header.type
//					<< std::endl;
			receivedQueue_.push(receivedMsg);
			MsgSendPulse_r(connectionId_, sched_get_priority_max(0), SERIAL,
					SERIALCONNECTED);

		} else {
//			std::cout << "Invalid Header: " << (int) receivedMsg.header.type
//					<< std::endl;
		}
	}

//	}
}

void SerialController::reset(){
	sendQueue_.clear();
	receivedQueue_.clear();
}
void SerialController::stop() {
	running_ = false;
}

long SerialController::buildCheckSum(SerialMsg msg) {
	long crc = 15;
	return crc;
}

bool SerialController::isConnected() {
	std::lock_guard<std::mutex> lock(serialMutex_);
	return connected_;
}
//void test(void) {
//
//	SerialController serialController(1);
//	Puk puck;
//	SerialMsg receivedMsg;
//	puck.i = 10;
//	puck.k = false;
//	MsgHeader header;
//	header.type = PUKMSG;
//
//	SerialMsg smsg;
//	smsg.header = header;
//	smsg.puck = puck;
//
//	std::thread th_sc(std::ref(serialController));
//
//	char c = 'r';
//	while (c != 'q') {
//		std::cin >> c;
//		if (c == 's') {
//			std::cout << "Eingabe(1 oder 2)" << std::endl;
//			std::cin >> c;
//			if (c == '1') {
//				smsg.puck.i = 15;
//				smsg.puck.k = true;
//			}
//			if (c == '2') {
//				smsg.puck.i = 47;
//				smsg.puck.k = false;
//			}
//			serialController.sendMsg(smsg);
//		}
//		if (c == 'r') {
//			if (!serialController.receiveEmpty()) {
//				receivedMsg = serialController.getNextMsg();
//				std::cout << "i=" << receivedMsg.puck.i << " \n k="
//						<< receivedMsg.puck.k << "\n HeaderType: "
//						<< (int) (receivedMsg.header.type) << std::endl;
//			}
//		}
//
//	}
//	serialController.stop();
//	th_sc.join();
//
//}
