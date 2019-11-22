#include <iostream>
#include <thread>

#include "ConveyorBelt.h"
using namespace std;

int main()
{
	cout << "Start" << endl; // prints Hello World!!!

	communication::Dispatcher dispatcher;

	thread dispatcher_thread(ref(dispatcher));
	cout << "Dispatcher Started" << endl;

	while (dispatcher.getChannelId() < 0)
	{
		cout << "channel id: " << dispatcher.getChannelId() << endl;
	}

	cout << "Channel Created channel id: " << dispatcher.getChannelId() << endl;
	int channelId = dispatcher.getChannelId();
	communication::PulseReceiver pulseReceiverController(channelId);
	while (pulseReceiverController.getChannelId() < 0)
	{
		cout << "channel id: " << pulseReceiverController.getChannelId()
				<< endl;
	}

	cout << "PulseChannel Created channel id: "
			<< pulseReceiverController.getChannelId() << endl;

	communication::HardwareClient hardwareController(channelId);
	communication::ActivityLightClient acLightController(channelId);
	communication::Timer timerController(channelId,
			pulseReceiverController.getChannelId());
	communication::StateMachineClient mainController(channelId, true,pulseReceiverController.getChannelId());
	thread hardware_thread(ref(hardwareController));
	thread ac_thread(ref(acLightController));
	thread pulseReceiver_thread(ref(pulseReceiverController));
	thread timer_thread(ref(timerController));
	thread mainController_thread(ref(mainController));

	Isr isr(hardwareController.controller,
			pulseReceiverController.getChannelId());

	//timerController.setTimer(1,0,0,10,10);
	//timerController.setTimer(2,0,0,1,3);

	char c = 'k';
	while (c != 'q')
	{
		cin >> c;
		if (c == 'e')
		{
			hardwareController.controller.enableAdc();
		} else if (c == 'd')
		{
			hardwareController.controller.disableAdc();
		} else if (c == 's')
		{
			hardwareController.getController()->led_start->ledOn();

		} else if (c == 'r')
		{
			hardwareController.getController()->led_reset->ledOn();

		}else if (c == '1')
		{
			hardwareController.getController()->led_q1->ledOn();

		}else if (c == '2')
		{
			hardwareController.getController()->led_q2->ledOn();
		}
		else if(c== 'o')
		{
			hardwareController.getController()->led_start->ledOff();
			hardwareController.getController()->led_reset->ledOff();
			hardwareController.getController()->led_q1->ledOff();
			hardwareController.getController()->led_q2->ledOff();
		}
		else if(c == 'm')
		{
			mainController.setBelt2exp(true);
		}
		else if(c == 'n')
		{
			mainController.setBelt2exp(false);
		}
		else if('i')
		{

	//		mainController.sendMsg(10);
		}
	}
	return 0;
}
//
////Test Hight messurement
//#include "automaton/HightMeasureStateMachine.h"
//
//#define BIN0 23000
//#define BIN1 22000
//#define MAX_HEIGHT 25000
//
//int main()
//{
//	automaton::HightMeasureStateMachine machine;
//
//	int value = BIN1; // value 1
//
//	machine.updateStates(value);
//	machine.runStates();
//	machine.updateStates(value);
//	machine.runStates();
//	machine.updateStates(value);
//	machine.runStates();
//	machine.updateStates(value);
//	machine.runStates();
//
//	value = MAX_HEIGHT;
//	machine.updateStates(value);
//	machine.runStates();
//	machine.updateStates(value);
//	machine.runStates();
//	machine.updateStates(value);
//	machine.runStates();
//	machine.updateStates(value);
//	machine.runStates();
//	machine.updateStates(value);
//	machine.runStates();
//
//	value = BIN0;					// value 2
//	machine.updateStates(value);
//	machine.runStates();
//	machine.updateStates(value);
//	machine.runStates();
//	machine.updateStates(value);
//	machine.runStates();
//	machine.updateStates(value);
//	machine.runStates();
//
//	value = MAX_HEIGHT;
//	machine.updateStates(value);
//	machine.runStates();
//	machine.updateStates(value);
//	machine.runStates();
//	machine.updateStates(value);
//	machine.runStates();
//	machine.updateStates(value);
//	machine.runStates();
//
//	value=BIN1;					//value3
//	machine.updateStates(value);
//	machine.runStates();
//	machine.updateStates(value);
//	machine.runStates();
//	machine.updateStates(value);
//	machine.runStates();
//	machine.updateStates(value);
//	machine.runStates();
//
//	value = MAX_HEIGHT;
//	machine.updateStates(value);
//	machine.runStates();
//	machine.updateStates(value);
//	machine.runStates();
//	machine.updateStates(value);
//	machine.runStates();
//	machine.updateStates(value);
//	machine.runStates();
//
//	std::cout<< machine.getResult()<<std::endl;
//
//	//std::cout << "values: "<< machine.values[0]<< machine.values[1]<< machine.values[2]<<std::endl;
//
//	return 0;
//}
