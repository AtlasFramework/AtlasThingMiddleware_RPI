/*
* FileName: Attachment_Engine.h
* Description: Generates tweets for the different attachments on both levels (the thing, the entity) 
* and offers different functions to access such attachments (Atlas thing multi-lang translator, Device manager OMA, web-based Dashboard, NodeJS server).
*
* Authors: Ahmed E. Khaled     Date: July 2017
*/

#ifndef ATTACHMENTENGINE
#define ATTACHMENTENGINE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <ctime>
using namespace std;

//------------------------------
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<signal.h>
#include<fcntl.h>
#define CONNMAX 1000
#define BYTES 1024
/*
#include <curl/curl.h>
#include <fstream>
#include <sstream>
#include <iostream>
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> //inet_addr
#include <vector>
#include <string>
#include <thread>
//using namespace std;
#include <iostream>
#include <exception>
#include <string>
#include <ctime>
#include <fstream>
#include <cstring>
#include <algorithm>
//------------------------------
#include <iostream>
#include <sstream>
#include <string>
//+++++++MQTT++++++++++++++
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTTClient.h"
#define TIMEOUT     10000L
//+++++++Unix Socket+++++++



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> //inet_addr
#include <vector>
#include <string>
#include <thread>
#include <iostream>
#include <exception>
#include <string>
#include <ctime>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <curl/curl.h>

using namespace std::chrono;




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <string>
#include <thread>


/***RESTful libraries**/
#include <iostream>
#include <exception>
#include <string>
#include <ctime>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <time.h>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <curl/curl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
using namespace std::chrono;



class Attachment_Engine {

  public:
    	vector<string> ThingAttachment_tweets;

    	Attachment_Engine();
	void Generate_AttachmentTweets();

    	void Parse_Attachments();
    	void DVR_Dashboard();
    	void Node_LogServer();
    	void Protocol_Translator();
    	void OffThing_Protocol_Translator();
    	void initiateBroker();
    	bool mqtt_client_detected();
    	void On_thing_translator();
};
#endif
