/*
* FileName: System_Functions.h
* Description: Holds the implementation of the different services offered by the thing to thing mates, and list of
* such services that holds the signatures of these functions.
* Note: Supported datatypes for the current version of Atlas-IoT_thing are void, int, bool. This can be extended to 
* include other types
*
* Authors: Ahmed E. Khaled     Date: July 2017
*/

#ifndef SYSTEM_FUNCTIONS
#define SYSTEM_FUNCTIONS
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <map>
#include <vector>
#include <curl/curl.h>
#include <cstring>
#include <unistd.h>
using namespace std;

//====================

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
//------------------------------



//====================



class System_Functions {
     public:
    	//typedef for each type of function according to the inputs and outputs, and a map for each type
	//First type includes user-defined functions with no input parameters
    	typedef int   (System_Functions::*TYPE1_1)();                              std::map<string, TYPE1_1> MapType1_1; 
    	typedef bool  (System_Functions::*TYPE1_2)();                              std::map<string, TYPE1_2> MapType1_2;
    	typedef void  (System_Functions::*TYPE1_3)();                              std::map<string, TYPE1_3> MapType1_3;
	
	//Second type includes user-defined functions with one integer input parameters
    	typedef int   (System_Functions::*TYPE2_1)(int value);                     std::map<string, TYPE2_1> MapType2_1; 
    	typedef bool  (System_Functions::*TYPE2_2)(int value);                     std::map<string, TYPE2_2> MapType2_2;
    	typedef void  (System_Functions::*TYPE2_3)(int value);                     std::map<string, TYPE2_3> MapType2_3;  

	//Third type includes user-defined functions with two integer input parameters
    	typedef int   (System_Functions::*TYPE3_1)(int value1, int value2);        std::map<string, TYPE3_1> MapType3_1; 
    	typedef bool  (System_Functions::*TYPE3_2)(int value1, int value2);        std::map<string, TYPE3_2> MapType3_2;
    	typedef void  (System_Functions::*TYPE3_3)(int value1, int value2);        std::map<string, TYPE3_3> MapType3_3;   


	/************************************Add user-defined in the following maps******************************
	On a new line within the System_Functions() method after adding your method in the class, add the following line:
	       MapType.insert(std::make_pair("The_Name_of_Your_Method",  &System_Functions::The_Name_of_Your_Method));
	replace The_Name_of_Your_Method with the name of your function that you provided implementation and want to add
        replace MapType with:
		MapType1_1 is you method has the following signature:  int  Name_of_Your_Method ()
		MapType1_2 is you method has the following signature:  bool Name_of_Your_Method ()
		MapType1_3 is you method has the following signature:  void Name_of_Your_Method ()
		MapType2_1 is you method has the following signature:  int  Name_of_Your_Method (int)
		MapType2_2 is you method has the following signature:  bool Name_of_Your_Method (int)
		MapType2_3 is you method has the following signature:  void Name_of_Your_Method (int)
		MapType3_1 is you method has the following signature:  int  Name_of_Your_Method (int, int)
		MapType3_2 is you method has the following signature:  bool Name_of_Your_Method (int, int)
		MapType3_3 is you method has the following signature:  void Name_of_Your_Method (int, int)
	*********************************************************************************************************/
    	System_Functions(){
  		MapType1_1.insert(std::make_pair("UserDefinedFunction_1",  &System_Functions::UserDefinedFunction_1));
		MapType2_1.insert(std::make_pair("UserDefinedFunction_2",  &System_Functions::UserDefinedFunction_2));
		MapType3_3.insert(std::make_pair("UserDefinedFunction_3",  &System_Functions::UserDefinedFunction_3));
	        MapType2_3.insert(std::make_pair("Recording_Scheduled",  &System_Functions::Recording_Scheduled));
		
        }



	/**************************************************************************************************************/
	/*************************************User defined functions **************************************************/
	/**************************************************************************************************************/
    	int UserDefinedFunction_1();
    	int UserDefinedFunction_2(int value);
    	void UserDefinedFunction_3(int value1, int value2);


	/****DVR-Functions for Mobicom17 Demo****/
	void Recording_Scheduled(int duration);


	/****Coffee prototype for Atlas archi****/
	void PrepareCoffee();
};
#endif