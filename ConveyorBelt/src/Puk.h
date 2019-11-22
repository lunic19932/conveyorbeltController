/*
 * Puk.h
 *
 *  Created on: 18.06.2018
 *      Author: abt234
 */

#ifndef PUK_H_
#define PUK_H_

struct Puk{
	int startTime;
	int id;
	int codeFirst;
	int codeSecond;
	int hightFirst[3];
	int hightSecond[3];
	int hightTikFirst;
	int hightTikSecond;
	int tiksFirst;
	int tiksSecond;
}__attribute__ ((packed));



#endif /* PUK_H_ */
