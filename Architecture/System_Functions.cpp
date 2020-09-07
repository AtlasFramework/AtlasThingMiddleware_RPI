/*
* FileName: System_Functions.h
* Description: Holds the implementation of the different services offered by the thing to thing mates, and list of
* such services that holds the signatures of these functions.
* Authors: Ahmed E. Khaled     Date: July 2017
*/
#include "System_Functions.h"

int System_Functions::UserDefinedFunction_1(){
	int x = 100;
	return x;
}

int System_Functions::UserDefinedFunction_2(int value){
	int x = value * 2;
	return x;
}

void System_Functions::UserDefinedFunction_3(int value1, int value2){
	int x = value1 + value2;
	cout<<"value of x"<<x<<endl;
}


void System_Functions::Recording_Scheduled(int duration){

//added part
 			struct ifreq ifr;
 			int fd = socket(AF_INET, SOCK_DGRAM, 0);
 			ifr.ifr_addr.sa_family = AF_INET;
 			strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
 			ioctl(fd, SIOCGIFADDR, &ifr);
 			close(fd);
			char *listenAddressString = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
                        std::string requestIP = std::string("http:\/\/")+listenAddressString + std::string(":8080\/jsonrpc?request=");
        		

cout<<"start recording"<<endl;

	//start recording through the API:   http://192.168.0.6:8080/jsonrpc?request={"jsonrpc":"2.0","method":"PVR.Record","params":{},"id":1}
	{
  		CURL *curl;
  		CURLcode res;
  		struct curl_slist *headers = NULL;
  		curl_global_init(CURL_GLOBAL_ALL);
  		curl = curl_easy_init();
  		if(curl) {
    			const char *data = "{\"jsonrpc\":\"2.0\",\"method\":\"PVR.Record\",\"params\":{},\"id\":1}";
    			//const char *data = "";
    			headers = curl_slist_append(headers, "content-type: application/json;");
    			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    			//curl_easy_setopt(curl, CURLOPT_URL, "http:\/\/192.168.0.6:8080\/jsonrpc?request=");
    			curl_easy_setopt(curl, CURLOPT_URL, requestIP.c_str());

    			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long) strlen(data));
    			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    			res = curl_easy_perform(curl);
    			if(res != CURLE_OK)
      				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

    			curl_easy_cleanup(curl);
  		}
  		curl_global_cleanup();
	}

	// starting wait duration to stop recording
	clock_t endwait;
	endwait = clock () + duration * CLOCKS_PER_SEC ;
	while (clock() < endwait) {}

cout<<"Stop recording"<<endl;


	//stop recording through the API:   http://192.168.0.6:8080/jsonrpc?request={"jsonrpc":"2.0","method":"PVR.Record","params":{},"id":1}
	{
  		CURL *curl;
  		CURLcode res;
  		struct curl_slist *headers = NULL;
  		curl_global_init(CURL_GLOBAL_ALL);
  		curl = curl_easy_init();
  		if(curl) {
    			const char *data = "{\"jsonrpc\":\"2.0\",\"method\":\"PVR.Record\",\"params\":{},\"id\":1}";
    			
			headers = curl_slist_append(headers, "content-type: application/json;");
    			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    			//curl_easy_setopt(curl, CURLOPT_URL, "http:\/\/192.168.0.6:8080\/jsonrpc?request=");
    			curl_easy_setopt(curl, CURLOPT_URL, requestIP.c_str());
    			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long) strlen(data));
    			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    			res = curl_easy_perform(curl);
    			if(res != CURLE_OK)
      				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

    			curl_easy_cleanup(curl);
  		}
  		curl_global_cleanup();
	}

cout<<"Done recording"<<endl;
}


void System_Functions::PrepareCoffee(){
	printf("Preparing coffee ........ \n");
	usleep (1000000);
 	return;
	
	FILE *export_file = NULL; 
        FILE *IO_direction = NULL;
        char str1[] = "low";
        char str2[] = "high";
        char str[] = "23";                       //value to pass to export file
        export_file = fopen ("/sys/class/gpio/export", "w");
        fwrite (str, 1, sizeof(str), export_file);
        fclose (export_file);
	for (int i=0; i<3; i++){        //blink LED 2 times
		printf("Turning on the coffee maker \n");
		IO_direction = fopen ("/sys/class/gpio/gpio23/direction", "w");
		fwrite (str2, 1, sizeof(str1), IO_direction);   //set the pin to HIGH
		fclose (IO_direction);
		usleep (1000000);

		printf("Turning off the coffee maker \n");
		IO_direction = fopen ("/sys/class/gpio/gpio23/direction", "w");
		fwrite (str1, 1, sizeof(str1), IO_direction);   //set the pin to LOW
		fclose (IO_direction);
		usleep (1000000);
	}
	export_file = fopen ("/sys/class/gpio/unexport", "w");   //remove the mapping
	fwrite (str, 1, sizeof(str), export_file);
	fclose (export_file);
	printf("Done Preparing coffee !! \n");
}
