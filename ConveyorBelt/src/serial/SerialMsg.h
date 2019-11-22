/*
 * SerialMsg.h
 *
 *  Created on: 29.04.2018
 *      Author: LuisNickel
 */

#ifndef SRC_SERIALMSG_H_
#define SRC_SERIALMSG_H_
#include "../Puk.h"
#define PING			0
#define PUKMSG 			1
#define EMGYSTOP 		2
#define EMGYSTOPCLEAR	3
#define RAMPFULL		4
#define RAMPFREE		5
#define BELTFREE		6
#define BELTFULL		7
#define BELTSTATUSREQ	8
#define SENDWARNING		9
#define METALACCEPTED	10
#define METALREJECTED	11



struct MsgHeader{
	char type;
	char version;
	int msgNumber;
	int ackNumber;
	long checkSum;
}__attribute__ ((packed));


struct SerialMsg{
	MsgHeader header;
	Puk puk;
}__attribute__ ((packed));

#endif /* SRC_SERIALMSG_H_ */
