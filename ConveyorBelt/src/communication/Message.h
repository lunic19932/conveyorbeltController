/*
 * Message.h
 *
 *  Created on: 28.04.2018
 *      Author: Johannes
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

namespace communication
{
// GPIO1
#define MSG_MOTOR_RIGHT		 0
#define MSG_MOTOR_LEFT 		1
#define MSG_MOTOR_SLOW 		2
#define MSG_MOTOR_STOP 		3
#define MSG_ACTLAMP_RED 	4
#define MSG_ACTLAMP_YELLOW 	5
#define MSG_ACTLAMP_GREEN 	6
#define MSG_SWITCH 			7

//GPIO2
#define MSG_LED_START 		8 //LED Taste Start
#define MSG_LED_RESET 		9 //LED Taste Reset
#define MSG_LED_Q1 			10 //Signalleuchte Q1
#define MSG_LED_Q2 			11//Signalleuchte Q2

#define MSG_SN_LB_START 		0
#define MSG_SN_LB_HIGHTSN 		1
#define MSG_SN_HIGHTCONTROLE 	2
#define MSG_SN_LB_SWITCH 		3
#define MSG_SN_METALL 			4
#define MSG_SN_SWITCHOPEN 		5
#define MSG_SN_LB_RAMP			6
#define MSG_SN_LB_END			7

#define MSG_BTN_START			8
#define MSG_BTN_STOP			9
#define MSG_BTN_RESET			10
#define MSG_BTN_EMERGENCY		11

#define BLINK_RED				0
#define BLINK_YELLOW			1
#define BLINK_GREEN				2

#define ACTOR_ARRAY_SIZE		12
#define SENSOR_ARRAY_SIZE		12
#define BLINK_ARRAY_SIZE		3

#define MSG_PULS_ALTIMER		0
#define MSG_PULS_TIKTIMER		1
#define MSG_PULS_SERIAL			2
#define MSG_PULS_GPIOISR		3
#define MSG_PULS_ADCISR			4
#define MSG_PULS_NOPULS			-1

struct Message
{
	int senderId;
	int receiverId;
	bool actorStatus[ACTOR_ARRAY_SIZE];
	bool blink[BLINK_ARRAY_SIZE];
	bool sensorData[SENSOR_ARRAY_SIZE];
	bool adcEnable;
	int hightSensor;
	int pulseType;
};

}
;

#endif /* MESSAGE_H_ */
