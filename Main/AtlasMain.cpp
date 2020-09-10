#include "../Architecture/Communication_Manager.h"
#include "../Architecture/Objects_Engine.h"
#include <iostream>
#include <thread>

int main(int argc, char *argv[]) {

	cout<<"Starting Atlas thing architecture ................"<<endl;

        Communication_Manager CM;
	//std::thread t1(&Communication_Manager::Communication_Manager,&CM); 	

	//IP socket communication	
	std::thread t1(&Communication_Manager::BroadcastTweets_IPSocket,&CM); 
	std::thread t2(&Communication_Manager::CaptureMatesTweets_IPSocket,&CM);
	std::thread t3(&Communication_Manager::CaptureAPIcalls_IPSocket,&CM);

	//MQTT socket communication

	//std::thread t4(&Communication_Manager::MQTT_Publish,&CM);
	//std::thread t5(&Communication_Manager::MQTT_Subscribe,&CM);

	//Objects_Engine OE;	
	//std::thread t6(&Objects_Engine::Start_DeviceManagement,&OE); 
	
	t1.join();
	t2.join();
	t3.join();
	//t4.join();
	//t5.join();
	//t6.join();

	return 0;
}
