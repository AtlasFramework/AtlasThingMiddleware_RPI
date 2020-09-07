/*
* FileName: IoTDDL_Parser.h
* Description: XML parser and manipulator for the uploaded IoTDDL file in ConfigurationFiles directory
* this parser utilizes tinyxml-2 library under zlib license
* Authors: Ahmed E. Khaled     Date: May. 2018
*/

#ifndef IOTDDLPARSER
#define IOTDDLPARSER

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;


class IoTDDL_Parser {
    char *DDL_FilePath;
    char *TMP_FilePath;
    char *TMP_Filename;
    XMLDocument doc;
    bool fileExist;

    public:
    	IoTDDL_Parser();	
	~IoTDDL_Parser();

	//check the existence of an accessable configuration file (e.g., SD card, file system)
        //this fn should be extended to: if there is no IoTDDL, the architecture should wait for file upload
	bool Check_IoTDDL();


	//get data of tag
	string parseXMLTag(string firstTag, string secondTag, string thirdTag, string fourthTag);
	string parseXMLTag(string firstTag, string secondTag, string thirdTag, string fourthTag, string fifthTag);
	string parseXMLTag(string firstTag, string secondTag, string thirdTag, string fourthTag, string fifthTag, string sixthTag);

	//get array of data of tag
	vector<string> parseXMLTagArray(string firstTag, string secondTag, string thirdTag, string fourthTag, string fifthTag, string sixthTag);

	//retrive data of tag
    	string retriveXMLTag(string maintag, string tag);
	string retriveXMLTag(string mainTag, string mainTag2, string tag);
	string retriveXMLTag(string mainTag, string mainTag2, string mainTag3, string tag);

	//update data of tag with addedInfo
	void updateXMLTag(string maintag, string tag, string addedInfo);
	void updateXMLTag(string mainTag, string mainTag2, string tag, string addedInfo);
    	void updateXMLTag(string mainTag, string mainTag2, string mainTag3, string tag, string addedInfo);

	//Add data with new tag with addedInfo
	void addXMLTag(string maintag, string newtag, string addedInfo);
	void addXMLTag(string mainTag, string mainTag2, string newtag, string addedInfo);
    	void addXMLTag(string mainTag, string mainTag2, string mainTag3, string newtag, string addedInfo);
};
#endif