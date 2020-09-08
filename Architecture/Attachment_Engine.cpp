/*
* FileName: Attachment_Engine.cpp
* Description: Generates tweets for the different attachments on both levels (the thing, the entity) 
* and offers different functions to access such attachments (Atlas thing multi-lang translator, Device manager OMA, web-based Dashboard, NodeJS server).
*/

#include "Attachment_Engine.h"
#include "IoTDDL_Parser.h"



void _wait ( int seconds ){
	clock_t endwait;
	endwait = clock () + seconds * CLOCKS_PER_SEC ;
	while (clock() < endwait) {}
}

Attachment_Engine::Attachment_Engine(){
    	ThingAttachment_tweets.clear();
}



void Attachment_Engine::Generate_AttachmentTweets(){
    	IoTDDL_Parser DDLM;
    	string ThingATID = DDLM.retriveXMLTag("Atlas_Thing","Descriptive_Metadata","Thing_ATID");
    	int num_Attachments = 1;
    	string num_of_services = DDLM.retriveXMLTag("Atlas_Thing","Thing_Attachment","Attachments_Number");
    	num_Attachments =  atoi(num_of_services.c_str());
    
    	for(int i=1;i<=num_Attachments;i++){
		string AttType = DDLM.retriveXMLTag("Thing_Attachment","Attachment_"+std::to_string(i),"Type");
        	string AttName = DDLM.retriveXMLTag("Thing_Attachment","Attachment_"+std::to_string(i),"Name");
        	string AttDesc = DDLM.retriveXMLTag("Thing_Attachment","Attachment_"+std::to_string(i),"Description");
        	string AttModl = DDLM.retriveXMLTag("Thing_Attachment","Attachment_"+std::to_string(i),"Model");
        	string AttVers = DDLM.retriveXMLTag("Thing_Attachment","Attachment_"+std::to_string(i),"Version");
		string AttAccessType = DDLM.retriveXMLTag("Thing_Attachment","Attachment_"+std::to_string(i),"Service_Access_Type");
		string AttAccessURL = DDLM.retriveXMLTag("Thing_Attachment","Attachment_"+std::to_string(i),"Service_Access_URL");
        	string AttAccessPORT = DDLM.retriveXMLTag("Thing_Attachment","Attachment_"+std::to_string(i),"Service_Access_PORT");
        
		string URL;
     		if (strstr((char *) AttAccessURL.c_str(), "on-thing")!=NULL){
 			struct ifreq ifr;
 			int fd = socket(AF_INET, SOCK_DGRAM, 0);
 			ifr.ifr_addr.sa_family = AF_INET;
 			strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
 			ioctl(fd, SIOCGIFADDR, &ifr);
 			close(fd);
			char *listenAddressString = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
        		URL = AttAccessType+":\/\/"+listenAddressString + ":" +AttAccessPORT;
     		}else{
        		URL = AttAccessType+":\/\/"+AttAccessURL + ":" +AttAccessPORT;
     		}
		string tweet = "AttachmentTweet ATID:" + ThingATID +" Name:" +AttName+"_" +AttModl + "_" + AttVers +"  " + URL;
		ThingAttachment_tweets.push_back(tweet);
    	}
}




void Attachment_Engine::Parse_Attachments(){
    	//IoTDDL_Parser DDLM;
	//if(DDLM.checkXMLTag("Atlas_Thing","Thing_Attachment","Log_Server")){
   	//	printf("Found \n");
		//Create_WebPage();
		//Create_LogServer();
	//}else{
   	//	printf("Attachments not found \n");
	//}
}

//-------------------------------------------------------------------------------------------------------
//----------------------------------broker thing service-------------------------------------------------
//-------------------------------------------------------------------------------------------------------

void Attachment_Engine::initiateBroker(){
    	system("chmod +x ~/Desktop/Atlas-Thing_Architecture/OnThingService/Atlas-Thing_Broker/Broker.sh");
	system("sh ~/Desktop/Atlas-Thing_Architecture/OnThingService/Atlas-Thing_Broker/Broker.sh");	
}

//-------------------------------------------------------------------------------------------------------
//---------------------------------------off thing translator--------------------------------------------
//-------------------------------------------------------------------------------------------------------

bool OP_mqtt_client_detected = false;
char tabx[1024];
struct sockaddr_in peeraddr;
int  res, s0, peeraddr_len;
char bufferc[1024];

int brokerlisten=0;
int brokerwrite=0;

void listenFromBroker(){
	printf("listening for any updates on from broker \n");
	res = read(s0, bufferc, 1024);
	if (res < 0) { perror("cannot read server reply");  close(s0); return;  }
	printf("Received from the translator: %s  \n", bufferc);
        OP_mqtt_client_detected = true;
	while(1){
		printf("listening for any updates on from broker \n");
		res = read(s0, bufferc, 1024);
		if (res < 0) { perror("cannot read server reply");  close(s0); return;  }
		printf("Received from the translator: %s  \n", bufferc);

    		cout<<"   listenning number PDUs received from the broker "<<brokerlisten<<endl;
		brokerlisten++;
	}
}

void WriteToBroker(){
	while(!OP_mqtt_client_detected);

    	int xRan=0;
    	while(1){
    		printf("writing to the offthing translator to publish\n");
		std::string thingTweets = "multicast";
		strcpy(tabx, thingTweets.c_str());
		write(s0,tabx, thingTweets.length());


    		xRan = rand()%5 + 1; //randomizing the number between 1-10
	        cout<<"CoAP thing uses off-thing translator to multicast and wait for "<<xRan<<"sec to send new publish"<<endl;
    		//wait(xRan);
    		cout<<"   writing number PDUs received from the broker "<<brokerwrite<<endl;
		brokerwrite++;
    	}
}

void Attachment_Engine::OffThing_Protocol_Translator(){
    	IoTDDL_Parser DDLM;
   	printf("Configuring and starting Off Thing Atlas protocol service\n");
	string BrokerURL = DDLM.retriveXMLTag("Atlas_Thing","Thing_Attachment","Protocol_Translator","Translator_URL");
	string BrokerPORT = DDLM.retriveXMLTag("Atlas_Thing","Thing_Attachment","Protocol_Translator","Translator_PORT");
	char * peerHost = (char *) BrokerURL.c_str();
	short peerPort = atoi(BrokerPORT.c_str());
        printf("OffThing Translator URl                : %s \n", peerHost);

	//while(1){
		s0 = socket(AF_INET, SOCK_STREAM, 0);
		if (s0 < 0) { perror("Cannot create a socket"); close(s0); }//continue; }

		memset(&peeraddr, 0, sizeof(peeraddr));
		struct hostent *host = gethostbyname(peerHost);
		if (host == NULL) {  perror("Cannot define host address"); close(s0); }//continue; }
		peeraddr.sin_family = AF_INET;
		peeraddr.sin_port = htons(peerPort);

		memmove(&(peeraddr.sin_addr.s_addr), host->h_addr_list[0], 4);
		res = connect(s0, (struct sockaddr*) &peeraddr, sizeof(peeraddr));
		if (res < 0) { perror("cannot connect to the remote Thing server"); close(s0); }//continue; }
		
   		printf("Instructing translator for broker connection and subscribtion .. \n");
		std::string thingTweets = "initiate connection:ExampleClientPutrans11, broker.hivemq.com:1883,ATLASMQTT/#";
		strcpy(tabx, thingTweets.c_str());
		write(s0,tabx, thingTweets.length());
		
		res = read(s0, bufferc, 1024);
		if (res < 0) { perror("cannot read server reply");  close(s0); }//continue;  }
		printf("Received reply from the translator: %s  \n", bufferc);
		
		printf("Keep listening for any updates on MQTT_Client topic from the translator .. \n");
		res = read(s0, bufferc, 1024);
		if (res < 0) { perror("cannot read server reply");  close(s0); }//continue;  }
		printf("Received reply from the translator: %s  \n", bufferc);

		printf("MQTT client(s) exist in the smart space \n");
		printf("Swtich CoAP Mutlicast through broker \n");
		std::thread t1(listenFromBroker);
		std::thread t2(WriteToBroker);
		t1.join();
		t2.join();
	//}
}

//-------------------------------------------------------------------------------------------------------
//----------------------------------On thing translator--------------------------------------------------
//-------------------------------------------------------------------------------------------------------


char* OP_ADDRESS;
char* OP_Private_ADDRESS;
char* OP_CLIENTID;
char* OP_RootTOPIC;
char* OP_TOPIC;
char* OP_MQTTClientTOPIC;
char* OP_PrivateBrokerTOPIC;
char* OP_MulticastThingIdTOPIC;
char* OP_MulticastEntityIdTOPIC;
char* OP_MulticastAPITOPIC;
char* OP_UnicastTOPIC;
char* OP_PAYLOAD;
short OP_QOS =1;
bool OP_privateBroker=false;
int OP_rc, OP_ch;
MQTTClient OP_client,OP_client2;
MQTTClient_connectOptions OP_conn_opts = MQTTClient_connectOptions_initializer;
volatile MQTTClient_deliveryToken OP_deliveredtoken;


void OP_Publish_DefaultBroker(){
    while(!OP_mqtt_client_detected);
    int packets_sent=0;
    int xRan=0;
    printf("the on thing translator initiates mutlicast publishing MQTT on the default broker \n");
    while(1){
    	MQTTClient_message pubmsg = MQTTClient_message_initializer;
    	MQTTClient_deliveryToken token;
    	OP_PAYLOAD = (char *) "multicasting_CoAP_ontranslator";
    	pubmsg.payload = (void *)OP_PAYLOAD;
    	pubmsg.payloadlen = strlen(OP_PAYLOAD);
    	pubmsg.qos = OP_QOS;
    	//pubmsg.retained = 1;
    	MQTTClient_publishMessage(OP_client, OP_MulticastThingIdTOPIC, &pubmsg, &token);
    	OP_rc = MQTTClient_waitForCompletion(OP_client, token, TIMEOUT);

	cout<<"number of packets sent from on-thing translator: "<<packets_sent<<endl;
	packets_sent++;
    	xRan = rand()%5 + 1; //randomizing the number between 1-10
	//cout<<"CoAP thing uses on-thing translator to multicast and wait for "<<xRan<<"sec to send new publish"<<endl;
    	_wait(xRan);
    }
}

void OP_Publish_PrivateBroker(){
    while(!OP_mqtt_client_detected);
    int packets_sent=0;
    int xRan=0;
    printf("the on thing translator initiates mutlicast publishing MQTT on the default broker \n");
    while(1){
    	MQTTClient_message pubmsg = MQTTClient_message_initializer;
    	MQTTClient_deliveryToken token;
    	OP_PAYLOAD = (char *) "multicasting_CoAP_ontranslator";
    	pubmsg.payload = (void *)OP_PAYLOAD;
    	pubmsg.payloadlen = strlen(OP_PAYLOAD);
    	pubmsg.qos = OP_QOS;
    	//pubmsg.retained = 1;
    	MQTTClient_publishMessage(OP_client2, OP_MulticastThingIdTOPIC, &pubmsg, &token);
    	OP_rc = MQTTClient_waitForCompletion(OP_client2, token, TIMEOUT);
	cout<<"number of packets sent from on-thing translator: "<<packets_sent<<endl;
	packets_sent++;

    	xRan = rand()%5 + 1; //randomizing the number between 1-10
	//cout<<"CoAP thing uses on-thing translator to multicast and wait for "<<xRan<<"sec to send new publish"<<endl;
    	_wait(xRan);
    }
}

void OP_delivered(void *context, MQTTClient_deliveryToken dt){
    printf("Message with token value %d delivery confirmed\n", dt);
    OP_deliveredtoken = dt;
}

void OP_connlost(void *context, char *cause){ printf("\nConnection lost  cause: %s\n", cause); }

int OP_subscribe_iterations = 0;
int OP_msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message){
    char payload[message->payloadlen + 1];
    strncpy(payload,(char *)message->payload, message->payloadlen);
    payload[message->payloadlen] = '\0';
    printf("   Received payload    : %s\n",payload);
    cout<<"   number of subscribe iterations "<<OP_subscribe_iterations<<endl;

    if (strstr(topicName, "/MQTT_Client")!=NULL){
        printf("the on thing translator received updates on /MQTT_Client \n");
        OP_mqtt_client_detected = true;
    }else if (strstr(topicName, "/Private_Broker")!=NULL){
	if(OP_privateBroker){
		printf("received updates on /Private_Broker \n");
        	printf("already migrated to the private broker %s \n", OP_Private_ADDRESS);
	}else {  //update the private broker if first time to get it.
        	printf("received updates on /Private_Broker \n");
        	printf("disconnect from %s \n\n\n",OP_ADDRESS);
    		MQTTClient_disconnect(OP_client, 100);
    		MQTTClient_destroy(&OP_client);
		usleep(100000);
        	OP_Private_ADDRESS = (char *)malloc(message->payloadlen + 1);
		int index=0;
		while(index<=strlen(payload)){
			OP_Private_ADDRESS[index] = payload[index]; index++;
		}
		OP_privateBroker=true;
	}
    }else if (strstr(topicName, OP_UnicastTOPIC)!=NULL){
        printf("Unicast interaction received \n");
	OP_subscribe_iterations++;
    }else if (strstr(topicName, OP_MulticastEntityIdTOPIC)!=NULL){
        printf("MulticastEntityIdTOPIC interaction received \n");
	OP_subscribe_iterations++;
    }else if (strstr(topicName, OP_MulticastAPITOPIC)!=NULL){
        printf("MulticastAPITOPIC interaction received \n");
	OP_subscribe_iterations++;
    }else if (strstr(topicName, OP_MulticastThingIdTOPIC)!=NULL){
        printf("MulticastThingIdTOPIC interaction received \n");
	OP_subscribe_iterations++;
    }

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void OP_Subscribe(){
	if(OP_privateBroker){
		printf("Suscribe on %s topic to %s broker \n", OP_RootTOPIC, OP_Private_ADDRESS);
		MQTTClient_subscribe(OP_client2, OP_RootTOPIC, OP_QOS);
		while(OP_privateBroker);
	}else {
		printf("Suscribe on %s topic to %s broker \n", OP_RootTOPIC, OP_ADDRESS);
		MQTTClient_subscribe(OP_client, OP_RootTOPIC, OP_QOS);
		while(!OP_privateBroker);
	}
}

void OP_DefaultBroker_Connect(){
	printf("Connect to default broker %s \n", OP_ADDRESS);
    	MQTTClient_create(&OP_client, OP_ADDRESS, OP_CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    	OP_conn_opts.keepAliveInterval = 20;
    	OP_conn_opts.cleansession = 1;
    	MQTTClient_setCallbacks(OP_client, NULL, OP_connlost, OP_msgarrvd, OP_delivered);
    	if ((OP_rc = MQTTClient_connect(OP_client, &OP_conn_opts)) != MQTTCLIENT_SUCCESS){
        	printf("Failed to connect, return code %d\n", OP_rc);  return;
    	}
	std::thread t1(OP_Subscribe);
	std::thread t2(OP_Publish_DefaultBroker);
	t1.join();
	t2.join();
}

void OP_NewBroker_Connect(){
	while(!OP_privateBroker);
        OP_Private_ADDRESS = (char *) "192.168.0.4:1883";
	printf("Connect to new broker %s \n", OP_Private_ADDRESS);
    	MQTTClient_create(&OP_client2, OP_Private_ADDRESS, OP_CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    	OP_conn_opts.keepAliveInterval = 20;
    	OP_conn_opts.cleansession = 1;
    	MQTTClient_setCallbacks(OP_client2, NULL, OP_connlost, OP_msgarrvd, OP_delivered);
    	if ((OP_rc = MQTTClient_connect(OP_client2, &OP_conn_opts)) != MQTTCLIENT_SUCCESS){
        	printf("Failed to connect, return code %d\n", OP_rc); 
		return;
    	}
	std::thread t1(OP_Subscribe);
	std::thread t2(OP_Publish_PrivateBroker);
	t1.join();
	t2.join();
}


void Attachment_Engine::On_thing_translator(){

    	IoTDDL_Parser DDLM;
   	printf("Configuring and starting On Thing Atlas protocol service\n");
        string ThingATID = DDLM.retriveXMLTag("Atlas_Thing","Descriptive_Metadata","Thing_ATID");
        string ThingSSID = DDLM.retriveXMLTag("Atlas_Thing","Descriptive_Metadata","Thing_SSID");
        string NetworkSSB = DDLM.retriveXMLTag("Atlas_Thing","Administrative_Metadata","Network_Manager","SSB");
        string Protocol = DDLM.retriveXMLTag("Atlas_Thing","Administrative_Metadata","Network_Manager","Communication_Protocol");
        string tempTopicRoot = DDLM.retriveXMLTag("Atlas_Thing","Administrative_Metadata","Network_Manager","Topic_Root");
        string tempTopicMQTTClient = DDLM.retriveXMLTag("Atlas_Thing","Administrative_Metadata","Network_Manager","Topic_Network_MQTTClient");
        string tempTopicPrivateBroker = DDLM.retriveXMLTag("Atlas_Thing","Administrative_Metadata","Network_Manager","Topic_Network_PrivateBroker");
	
	string tempTopicMulticastThingId = DDLM.retriveXMLTag("Atlas_Thing","Administrative_Metadata","Network_Manager","Topic_Multicast_ThingIdentity");
        string tempTopicMulticastEntityId = DDLM.retriveXMLTag("Atlas_Thing","Administrative_Metadata","Network_Manager","Topic_Multicast_EntityIdentity");
        string tempTopicMulticastAPI = DDLM.retriveXMLTag("Atlas_Thing","Administrative_Metadata","Network_Manager","Topic_Multicast_API");
        string tempTopicUnicast = DDLM.retriveXMLTag("Atlas_Thing","Administrative_Metadata","Network_Manager","Topic_Unicast");
	string TopicRoot = tempTopicRoot + "_" +ThingSSID;
	string MQTTClientTopic = TopicRoot +tempTopicMQTTClient;
	string PrivateBrokerTopic = TopicRoot  +tempTopicPrivateBroker;
	string MulticastThingIdTopic = TopicRoot +tempTopicMulticastThingId;
	string MulticastEntityIdTopic = TopicRoot  +tempTopicMulticastEntityId;
	string MulticastAPITopic = TopicRoot +tempTopicMulticastAPI;
	string UnicastTopic = TopicRoot  + tempTopicUnicast + "_" + ThingATID;
	string SubscriptionTopicRoot = tempTopicRoot + "_" +ThingSSID + "/#";

	OP_CLIENTID = (char *) ThingATID.c_str();
	OP_ADDRESS = (char *) NetworkSSB.c_str();
	OP_RootTOPIC = (char *) SubscriptionTopicRoot.c_str();
	OP_MQTTClientTOPIC = (char *) MQTTClientTopic.c_str();
	OP_PrivateBrokerTOPIC = (char *) PrivateBrokerTopic.c_str();
    	OP_MulticastThingIdTOPIC = (char *) MulticastThingIdTopic.c_str();
    	OP_MulticastEntityIdTOPIC = (char *) MulticastEntityIdTopic.c_str();
    	OP_MulticastAPITOPIC = (char *) MulticastAPITopic.c_str();
    	OP_UnicastTOPIC = (char *) UnicastTopic.c_str();
	OP_PAYLOAD = (char*)"payload from translator";
	OP_QOS=1;

	std::thread t1(OP_DefaultBroker_Connect);
	std::thread t2(OP_NewBroker_Connect);
	t1.join();
	t2.join();
}


bool Attachment_Engine::mqtt_client_detected(){
	return OP_mqtt_client_detected;
}




//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------

void Attachment_Engine::Node_LogServer(){

   	printf("Thing attachment Log-server\n");
    	IoTDDL_Parser DDLM;
        string Description = DDLM.retriveXMLTag("Thing_Attachment","Log_Server","Description");
	string URI = DDLM.retriveXMLTag("Thing_Attachment","Log_Server","URI");
	string Port = DDLM.retriveXMLTag("Thing_Attachment","Log_Server","Port");
	string RequestType = DDLM.retriveXMLTag("Thing_Attachment","Log_Server","HTTP_RequestType");
	string Name = DDLM.retriveXMLTag("Thing_Attachment","Log_Server","Login_Name");
	string Passcode = DDLM.retriveXMLTag("Thing_Attachment","Log_Server","Login_Passcode");

	char* peerHost = (char *) URI.c_str();
	short peerPort = atoi(Port.c_str());

	int s0 = socket(AF_INET, SOCK_STREAM, 0);
	if (s0 < 0) { perror("Cannot create a socket"); close(s0); return; }

	struct sockaddr_in peeraddr;
	int peeraddr_len;
	memset(&peeraddr, 0, sizeof(peeraddr));
	struct hostent *host = gethostbyname(peerHost);
	if (host == NULL) {  perror("Cannot define host address"); close(s0); return;}
	peeraddr.sin_family = AF_INET;
	peeraddr.sin_port = htons(peerPort);

	memmove(&(peeraddr.sin_addr.s_addr), host->h_addr_list[0], 4);
	int res = connect(s0, (struct sockaddr*) &peeraddr, sizeof(peeraddr));
	if (res < 0) { perror("cannot connect to the remote Thing server"); close(s0); return; }

	string http_cmd1 = "PUT /RasbperryPi/Status/Alive HTTP/1.1\n";
    	string http_cmd2 = "Host: 192.168.0.4\n";
    	string http_cmd3 = "Connection: keep-alive\n";
    	string http_cmd4 = "Content-Length: 45\n";
    	string http_cmd5 = "Origin: http://192.168.0.4\n";
    	string http_cmd6 = "User-Agent: RaspberryPI\n";
    	string http_cmd7 = "Content-Type: text/plain;charset=UTF-8\n";
    	string http_cmd8 = "Referer: http://192.168.0.4/debug/clip.html\n";
    	string http_cmd9 = "Accept-Encoding: gzip,deflate,sdch\n";
    	string http_cmd10 = "Accept-Language: en-US,en;q=0.8\n";
    	string http_cmd11 = "\n";
    	string http_cmd12 = "\n";

	char cmd1[http_cmd1.size()+1]; strcpy(cmd1,http_cmd1.c_str());
    	write(s0, cmd1, sizeof(cmd1)-1);
	char cmd2[http_cmd2.size()+1]; strcpy(cmd2,http_cmd2.c_str());
    	write(s0, cmd2, sizeof(cmd2)-1);
	char cmd3[http_cmd3.size()+1]; strcpy(cmd3,http_cmd3.c_str());
    	write(s0, cmd3, sizeof(cmd3)-1);
	char cmd4[http_cmd4.size()+1]; strcpy(cmd4,http_cmd4.c_str());
    	write(s0, cmd4, sizeof(cmd4)-1);
	char cmd5[http_cmd5.size()+1]; strcpy(cmd5,http_cmd5.c_str());
    	write(s0, cmd5, sizeof(cmd5)-1);
	char cmd6[http_cmd6.size()+1]; strcpy(cmd6,http_cmd6.c_str());
    	write(s0, cmd6, sizeof(cmd6)-1);
	char cmd7[http_cmd7.size()+1]; strcpy(cmd7,http_cmd7.c_str());
    	write(s0, cmd7, sizeof(cmd7)-1);
	char cmd8[http_cmd8.size()+1]; strcpy(cmd8,http_cmd8.c_str());
    	write(s0, cmd8, sizeof(cmd8)-1);
	char cmd9[http_cmd9.size()+1]; strcpy(cmd9,http_cmd9.c_str());
    	write(s0, cmd9, sizeof(cmd9)-1);
	char cmd10[http_cmd10.size()+1]; strcpy(cmd10,http_cmd10.c_str());
    	write(s0, cmd10, sizeof(cmd10)-1);
	char cmd11[http_cmd11.size()+1]; strcpy(cmd11,http_cmd11.c_str());
    	write(s0, cmd11, sizeof(cmd11)-1);
	char cmd12[http_cmd12.size()+1]; strcpy(cmd12,http_cmd12.c_str());
    	write(s0, cmd12, sizeof(cmd12)-1);

	char bufferc[1024];
	res = read(s0, bufferc, 1024);
	if (res < 0) { perror("cannot read server reply");  close(s0); return;  }

	close(s0);


	/*
   	printf("Starting channel with the attachment Log-server \n");
	char tabx[1024];
	char* peerHost = (char *) "192.168.0.4";
	short peerPort = atoi("8080");
	int s0 = socket(AF_INET, SOCK_STREAM, 0);
	if (s0 < 0) { perror("Cannot create a socket"); close(s0); return; }
	struct sockaddr_in peeraddr;
	int peeraddr_len;
	memset(&peeraddr, 0, sizeof(peeraddr));
	struct hostent *host = gethostbyname(peerHost);
	if (host == NULL) {  perror("Cannot define host address"); close(s0); return;}
	peeraddr.sin_family = AF_INET;
	peeraddr.sin_port = htons(peerPort);
	memmove(&(peeraddr.sin_addr.s_addr), host->h_addr_list[0], 4);
	int res = connect(s0, (struct sockaddr*) &peeraddr, sizeof(peeraddr));
	if (res < 0) { perror("cannot connect to the remote Thing server"); close(s0); return; }	
    	char http_cmd1[] = "PUT /RasbperryPi/Status/Alive HTTP/1.1\n";
    	char http_cmd2[] = "Host: 192.168.0.4\n";
    	char http_cmd3[] = "Connection: keep-alive\n";
    	char http_cmd4[] = "Content-Length: 45\n";
    	char http_cmd5[] = "Origin: http://192.168.0.4\n";
    	char http_cmd6[] = "User-Agent: RaspberryPI\n";
    	char http_cmd7[] = "Content-Type: text/plain;charset=UTF-8\n";
    	char http_cmd8[] = "Referer: http://192.168.0.4/debug/clip.html\n";
    	char http_cmd9[] = "Accept-Encoding: gzip,deflate,sdch\n";
    	char http_cmd10[] = "Accept-Language: en-US,en;q=0.8\n";
    	char http_cmd11[] = "\n";
    	char http_cmd12[] = "\n";
    	write(s0, http_cmd1, sizeof(http_cmd1)-1);
    	write(s0, http_cmd2, sizeof(http_cmd2)-1);
    	write(s0, http_cmd3, sizeof(http_cmd3)-1);
    	write(s0, http_cmd4, sizeof(http_cmd4)-1);
    	write(s0, http_cmd5, sizeof(http_cmd5)-1);
    	write(s0, http_cmd6, sizeof(http_cmd6)-1);
    	write(s0, http_cmd7, sizeof(http_cmd7)-1);
    	write(s0, http_cmd8, sizeof(http_cmd8)-1);
    	write(s0, http_cmd9, sizeof(http_cmd9)-1);
    	write(s0, http_cmd10, sizeof(http_cmd10)-1);
    	write(s0, http_cmd11, sizeof(http_cmd11)-1);
    	write(s0, http_cmd12, sizeof(http_cmd12)-1);
	char bufferc[1024];
	res = read(s0, bufferc, 1024);
	if (res < 0) { perror("cannot read server reply");  close(s0); return;  }
	close(s0);
	*/
}

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------


//====================HTTP======================
int listenfdx, clientsx;
void HTTPstartServer(char *);
void HTTPrespond(int);
char result[1024];

void call_DVR_request(){
/*
  CURL *curl;
  CURLcode res;
  struct curl_slist *headers = NULL;
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if(curl) {
    const char *data = "{\"jsonrpc\":\"2.0\",\"method\":\"Player.PlayPause\", \"params\" : {\"playerid\" : 2}, \"id\" : 1}";
    headers = curl_slist_append(headers, "content-type: application/json;");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, "http://10.29.6.51:1588/jsonrpc");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long) strlen(data));
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
*/
}

int HTTP_State_API(std::string protocol,std::string operation,std::string path) {
   int sock, n;
   unsigned int length;
   struct sockaddr_in server, from;
   std::string message = protocol + operation + path;
   char buffer[1024];
   strncpy(buffer, message.c_str(),sizeof(buffer));
   buffer[sizeof(buffer)-1] = 0;
   
   //here we call the corresponding url of the DVR
   printf("here we call the corresponding url of the DVR \n");

   printf("done calling the DVR \n");
   return 0;
}

void Attachment_Engine::DVR_Dashboard(){
    printf("Starting TCP channel with the attached web dashboard \n");
    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    char c;    
    char PORT[6];
    strcpy(PORT,"10000");
    int slot=0;
    
    clientsx=-1;
    HTTPstartServer(PORT);
    while (1){
        addrlen = sizeof(clientaddr);
        clientsx = accept (listenfdx, (struct sockaddr *) &clientaddr, &addrlen);
        if (clientsx<0)
            printf ("accept() error \n");
        else{
                HTTPrespond(slot);
                printf("done HTTP response");
        }
    }
    return;
}

void HTTPstartServer(char *port){
    struct addrinfo hints, *res, *p;
    memset (&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo( NULL, port, &hints, &res) != 0){ printf ("HTTP_Main getaddrinfo() error");    return; }
    for (p = res; p!=NULL; p=p->ai_next){
        listenfdx = socket (p->ai_family, p->ai_socktype, 0);
        if (listenfdx == -1) continue;
        if (bind(listenfdx, p->ai_addr, p->ai_addrlen) == 0) break;
    }
    if (p==NULL){ printf ("HTTP_Main socket() or bind()");    return; }
    freeaddrinfo(res);
    if ( listen (listenfdx, 1000000) != 0 ){  printf("HTTP_Main listen() error");         return;}
}

void HTTPrespond(int n)  {
    char mesg[99999], *reqline[3], data_to_send[BYTES], path[99999];
    int rcvd, fd, bytes_read;
    memset( (void*)mesg, (int)'\0', 99999 );
    rcvd=recv(clientsx, mesg, 99999, 0);

    if (rcvd<0)    // receive error
        fprintf(stderr,("HTTP_Main recv() error\n"));
    else if (rcvd==0)    // receive socket closed
        fprintf(stderr,"Client disconnected upexpectedly.\n");
    else{
        printf("Parsing HTTP message \n");
        //printf("%s\n", mesg);

        reqline[0] = strtok (mesg, " \t\n");
        reqline[1] = strtok (NULL, " \t");
        reqline[2] = strtok (NULL, " \t\n");

        if ( strncmp(reqline[0], "GET\0", 4)==0 ){
            printf("GET request for : ");
            printf(reqline[1]);
            printf("\n");
            if ( strncmp( reqline[2], "HTTP/1.1", 8)!=0 && strncmp( reqline[2], "HTTP/1.1", 8)!=0 ){
                printf("HTTP/1.1 Bad request \n");
                write(clientsx, "HTTP/1.1 400 Bad Request\n", 25);
            }
            else{
                printf("call_DVR_request \n");
                //HTTP_State_API("HTTP",reqline[0],reqline[1]);
		call_DVR_request();
                printf("Done call_DVR_request \n");


                printf("Building HTTP Response \n");
                char message[] = "HTTP/1.1 200 Ok\r\nContent-Type: text/html; charset=ISO-8859-4 \r\nContent-Length: 10\r\nConnection: keep-alive\r\n\r\n";
                size_t len = strlen(message);
                char message2[] = "Hello";
                size_t len2 = strlen(result);
                send(clientsx, message, len,0);
                write (clientsx, result, len2);
                printf("Done HTTP Response \n");
            }
        }else if( strncmp(reqline[0], "POST\0", 5)==0 ){
            printf("POST request for : ");
            printf(reqline[1]);
            if ( strncmp( reqline[2], "HTTP/1.1", 8)!=0 && strncmp( reqline[2], "HTTP/1.1", 8)!=0 ){
                printf("HTTP/1.1 Bad request \n");
                write(clientsx, "HTTP/1.1 400 Bad Request\n", 25);
            }else{
                printf("call_DVR_request \n");
                //HTTP_State_API("HTTP",reqline[0],reqline[1]);
		call_DVR_request();
                printf("Done call_DVR_request \n");

                printf("Building HTTP Response \n");
                char message[] = "HTTP/1.1 200 Ok\r\n\r\n";
                size_t len = strlen(message);
                send(clientsx, message, len,0);
                printf("Done HTTP Response \n");

	    }
        }
    }

    shutdown (clientsx, SHUT_RDWR);         //All further send and recieve operations are DISABLED...
    close(clientsx);
    clientsx=-1;
}
