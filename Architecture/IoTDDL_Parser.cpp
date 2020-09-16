/*
* FileName: IoTDDL_Parser.h
* Description: XML parser and manipulator for the uploaded IoTDDL file in ConfigurationFiles directory
* this parser utilizes tinyxml-2 library under zlib license
* Authors: Ahmed E. Khaled     Date: May. 2018
*/

#include "IoTDDL_Parser.h"

//constructor
IoTDDL_Parser::IoTDDL_Parser(){
    	DDL_FilePath = "ConfigurationFiles/Atlas_IoTDDL.xml";
    	TMP_FilePath = "ConfigurationFiles/tmp.xml";
    	TMP_Filename = "ConfigurationFiles/xtmp.xml";
	if(!doc.LoadFile("ConfigurationFiles/Atlas_IoTDDL.xml"))  fileExist = true;
	else{
		fileExist = false;                    
		cout<<"can't find the Atlas IoTDDL XML configuration file"<<endl;
	}
}


//deconstructor
IoTDDL_Parser::~IoTDDL_Parser(){
}


//get data of tag with 4th level
string IoTDDL_Parser::parseXMLTag(string firstTag, string secondTag, string thirdTag, string fourthTag){
	if(!fileExist) return "";
try{
	const char * a = firstTag.c_str();
	const char * b = secondTag.c_str();
	const char * c = thirdTag.c_str();
	const char * d = fourthTag.c_str();
	return doc.FirstChildElement(a)->FirstChildElement(b)->FirstChildElement(c)->FirstChildElement(d)->GetText();

}catch( ... ){
  return "";
}


}

//get data of tag with 5th level
string IoTDDL_Parser::parseXMLTag(string firstTag, string secondTag, string thirdTag, string fourthTag, string fifthTag){
	if(!fileExist) return "";
	try{
		const char * a = firstTag.c_str();
		const char * b = secondTag.c_str();
		const char * c = thirdTag.c_str();
		const char * d = fourthTag.c_str();
		const char * e = fifthTag.c_str();
		return doc.FirstChildElement(a)->FirstChildElement(b)->FirstChildElement(c)->FirstChildElement(d)->FirstChildElement(e)->GetText();
	}catch( ... ){
  		return "";
	}
}

//get data of tag with 6th level
string IoTDDL_Parser::parseXMLTag(string firstTag, string secondTag, string thirdTag, string fourthTag, string fifthTag, string sixthTag){
	if(!fileExist) return "";
	try{
		const char * a = firstTag.c_str();
		const char * b = secondTag.c_str();
		const char * c = thirdTag.c_str();
		const char * d = fourthTag.c_str();
		const char * e = fifthTag.c_str();
		const char * f = sixthTag.c_str();
		return doc.FirstChildElement(a)->FirstChildElement(b)->FirstChildElement(c)->FirstChildElement(d)->FirstChildElement(e)->FirstChildElement(f)->GetText();
	}catch( ... ){
	  return "";
	}
}

vector<string> IoTDDL_Parser::parseXMLTagArray(string firstTag, string secondTag, string thirdTag, string fourthTag, string fifthTag, string sixthTag) {
	if (!fileExist) return {};
	const char * a = firstTag.c_str();
	const char * b = secondTag.c_str();
	const char * c = thirdTag.c_str();
	const char * d = fourthTag.c_str();
	const char * e = fifthTag.c_str();

	vector<string> data;
	auto* nn = doc.FirstChildElement(a)->FirstChildElement(b)->FirstChildElement(c)->FirstChildElement(d)->FirstChildElement(e);
	for (auto* n = nn->FirstChildElement(); n != nullptr; n = n->NextSiblingElement()) {
		if (sixthTag == n->Value())
			data.push_back(n->GetText());
	}

	return data;
}

//Check the existence of the IoT-DDL configuration file
bool IoTDDL_Parser::Check_IoTDDL(){
	ifstream in(DDL_FilePath);
    	if (in.is_open()){
    		  in.close();
	  	return true;
	}
	return false;
}


/**********************************************************************************************************************/
/**********************************************Retrive data of a given tag*********************************************/
/**********************************************************************************************************************/

//Retrive data of an XML tag of 2nd level
string IoTDDL_Parser::retriveXMLTag(string mainTag, string tag){
      	string line;
      	string s1 = "<"+tag+">";
      	string s2 = "</"+tag+">";
      	bool mainTagB = false;
      	std::size_t found=0;
      	std::size_t found1=0;
      	std::size_t found2=0;
      	ifstream in(DDL_FilePath);
      	if (in.is_open()){
         	while (getline(in,line)){
			found = line.find("<"+mainTag+">"); //found tag in the mainTag seciton
			if (found!=std::string::npos){
				mainTagB = true;
			}
			if(mainTagB){
				found1 = line.find(s1);
				if (found1!=std::string::npos){
					found2 = line.find(s2);
					if (found2!=std::string::npos){
						line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
						line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
						line = line.substr(s1.size(), line.size());
						line = line.erase(line.size() - s2.size());
						in.close();
						return line;
					}else{
						in.close();
						return "";
					}
				}
			}
         	}//end of while
	 	in.close();
	 	return "";
    	}else {
      		in.close();
      		return "";
    	}
}

//Retrive data of an XML tag of third level
string IoTDDL_Parser::retriveXMLTag(string mainTag, string mainTag2, string tag){
      	string line;
      	string s1 = "<"+tag+">";
      	string s2 = "</"+tag+">";
      	bool mainTagB = false;
      	bool mainTag2B = false;  
      	std::size_t found=0;
      	std::size_t found1=0;
      	std::size_t found2=0;
      	ifstream in(DDL_FilePath);
      	if (in.is_open()){
         	while (getline(in,line)){
			found = line.find("<"+mainTag+">"); //found tag in the mainTag seciton
			if (found!=std::string::npos){
				mainTagB = true;
			}
			if(mainTagB){
				found = line.find("<"+mainTag2+">"); //found tag in the mainTag seciton
				if (found!=std::string::npos){
					mainTag2B = true;
				}
			}
			if(mainTagB&&mainTag2B){
				found1 = line.find(s1);
				if (found1!=std::string::npos){
					found2 = line.find(s2);
					if (found2!=std::string::npos){
						line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
						line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
						line = line.substr(s1.size(), line.size());
						line = line.erase(line.size() - s2.size());
						in.close();
						return line;
					}else{
						in.close();
						return "";
					}
				}
			}
      		}//end of while
      		in.close();
      		return "";
    	}else {
      		in.close();
      		return "";
    	}
}

//Retrive data of an XML tag of forth level
string IoTDDL_Parser::retriveXMLTag(string mainTag, string mainTag2, string mainTag3, string tag){
	string line;
	string s1 = "<"+tag+">";
    	string s2 = "</"+tag+">";
	bool mainTagB = false;
	bool mainTag2B = false;
	bool mainTag3B = false;
	std::size_t found=0;
	std::size_t found1=0;
    	std::size_t found2=0;
      	ifstream in(DDL_FilePath);
    	if (in.is_open()){
      		while (getline(in,line)){
			found = line.find("<"+mainTag+">"); //found tag in the mainTag seciton
			if (found!=std::string::npos){
				mainTagB = true;
			}
			if(mainTagB){
				found = line.find("<"+mainTag2+">"); //found tag in the mainTag seciton
				if (found!=std::string::npos){
					mainTag2B = true;
				}
			}
			if(mainTagB&&mainTag2B){
				found = line.find("<"+mainTag3+">"); //found tag in the mainTag seciton
				if (found!=std::string::npos){
					mainTag3B = true;
				}
			}
			if(mainTagB&&mainTag2B&&mainTag3B){
				found1 = line.find(s1);
				if (found1!=std::string::npos){
					found2 = line.find(s2);
					if (found2!=std::string::npos){
						line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
						line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
						line = line.substr(s1.size(), line.size());
						line = line.erase(line.size() - s2.size());
						in.close();
						return line;
					}else{
						in.close();
						return "";
					}
				}
			}
      		}//end of while
      		in.close();
      		return "";
    	}else {
      		in.close();
      		return "";
    	}
}


/**********************************************************************************************************************/
/**********************************************Update data of a given tag**********************************************/
/**********************************************************************************************************************/


/*
* Update data of an XML tag of second level
*/
void IoTDDL_Parser::updateXMLTag(string maintag, string tag, string addedInfo){
	string line;
      	string s1 = "<"+tag+">";
      	string s2 = "</"+tag+">";
      	std::size_t found1=0;
      	std::size_t found2=0;
	bool mainTagB = false;
	std::size_t found=0;
	ifstream in(DDL_FilePath);
	ofstream out(TMP_FilePath);
    	if (in.is_open()){
        	while (getline(in,line)){
			found = line.find("<"+maintag+">");
            		if (found!=std::string::npos){
                		mainTagB = true;
            		}
			if(mainTagB){
				found1 = line.find(s1);
				if (found1!=std::string::npos){
					found2 = line.find(s2);
					if (found2!=std::string::npos){   //found the s1 and s2 on the same line 
						mainTagB = false;
                				out<<"<"+tag+">"+addedInfo+"</"+tag+">";
                				out<<"\n";
						continue;
					}
				}
			}
			out<<line;
            		out<<"\n";
        	}//end of while
        	in.close();
        	out.close();

		//write back the content of the temp xml file to IoT_DDL file
		ifstream inn(TMP_FilePath);
		ofstream outt(DDL_FilePath);
		if (inn.is_open()){
        		while (getline(inn,line)){
				outt<<line;
            			outt<<"\n";
        		}//end of while
        		inn.close();
        		outt.close();
    		}else {
        		inn.close();
        		outt.close();
    		}
		//done writing back to IoTDDL
    	}else {
        	in.close();
        	out.close();
    	}
}

/*
* Update data of an XML tag of third level
*/
void IoTDDL_Parser::updateXMLTag(string maintag, string mainTag2, string tag, string addedInfo){
	string line;
      	string s1 = "<"+tag+">";
      	string s2 = "</"+tag+">";
      	std::size_t found1=0;
      	std::size_t found2=0;
	bool mainTagB = false;
	bool mainTag2B = false;
	std::size_t found=0;
	ifstream in(DDL_FilePath);
	ofstream out(TMP_FilePath);
    	if (in.is_open()){
        	while (getline(in,line)){
			found = line.find("<"+maintag+">");
            		if (found!=std::string::npos){
                		mainTagB = true;
            		}
			if(mainTagB){
				found = line.find("<"+mainTag2+">"); //found tag in the mainTag seciton
				if (found!=std::string::npos){
					mainTag2B = true;
				}
			}
			if(mainTagB&&mainTag2B){
				found1 = line.find(s1);
				if (found1!=std::string::npos){
					found2 = line.find(s2);
					if (found2!=std::string::npos){   //found the s1 and s2 on the same line 
						mainTagB = false;
                				out<<"<"+tag+">"+addedInfo+"</"+tag+">";
                				out<<"\n";
						continue;
					}
				}
			}
			out<<line;
            		out<<"\n";
        	}//end of while
        	in.close();
        	out.close();

		//write back the content of the temp xml file to IoT_DDL file
		ifstream inn(TMP_FilePath);
		ofstream outt(DDL_FilePath);
		if (inn.is_open()){
        		while (getline(inn,line)){
				outt<<line;
            			outt<<"\n";
        		}//end of while
        		inn.close();
        		outt.close();
    		}else {
        		inn.close();
        		outt.close();
    		}
		//done writing back to IoTDDL
    	}else {
        	in.close();
        	out.close();
    	}
}

/*
* Update data of an XML tag of forth level
*/
void IoTDDL_Parser::updateXMLTag(string maintag, string mainTag2, string mainTag3, string tag, string addedInfo){
	string line;
      	string s1 = "<"+tag+">";
      	string s2 = "</"+tag+">";
      	std::size_t found1=0;
      	std::size_t found2=0;
	bool mainTagB = false;
	bool mainTag2B = false;
	bool mainTag3B = false;
	std::size_t found=0;
	ifstream in(DDL_FilePath);
	ofstream out(TMP_FilePath);
    	if (in.is_open()){
        	while (getline(in,line)){
			found = line.find("<"+maintag+">");
            		if (found!=std::string::npos){
                		mainTagB = true;
            		}
			if(mainTagB){
				found = line.find("<"+mainTag2+">"); //found tag in the mainTag seciton
				if (found!=std::string::npos){
					mainTag2B = true;
				}
			}
			if(mainTagB&&mainTag2B){
				found = line.find("<"+mainTag3+">"); //found tag in the mainTag seciton
				if (found!=std::string::npos){
					mainTag3B = true;
				}
			}
			if(mainTagB&&mainTag2B&&mainTag3B){
				found1 = line.find(s1);
				if (found1!=std::string::npos){
					found2 = line.find(s2);
					if (found2!=std::string::npos){   //found the s1 and s2 on the same line 
						mainTagB = false;
                				out<<"<"+tag+">"+addedInfo+"</"+tag+">";
                				out<<"\n";
						continue;
					}
				}
			}

			out<<line;
            		out<<"\n";
        	}//end of while
        	in.close();
        	out.close();

		//write back the content of the temp xml file to IoT_DDL file
		ifstream inn(TMP_FilePath);
		ofstream outt(DDL_FilePath);
		if (inn.is_open()){
        		while (getline(inn,line)){
				outt<<line;
            			outt<<"\n";
        		}//end of while
        		inn.close();
        		outt.close();
    		}else {
        		inn.close();
        		outt.close();
    		}
		//done writing back to IoTDDL
    	}else {
        	in.close();
        	out.close();
    	}
}


/**********************************************************************************************************************/
/**********************************************Add data with new tag***************************************************/
/**********************************************************************************************************************/

/*
* Add data for a new XML tag of second level
*/
void IoTDDL_Parser::addXMLTag(string maintag, string newtag, string addedInfo){
	string line;
	bool mainTagB = false;
	std::size_t found=0;
	ifstream in(DDL_FilePath);
	ofstream out(TMP_FilePath);
    	if (in.is_open()){
        	while (getline(in,line)){
			found = line.find("<"+maintag+">");
            		if (found!=std::string::npos){
                		mainTagB = true;
            		}
			out<<line;
            		out<<"\n";
			if(mainTagB){
				mainTagB = false;
                		out<<"<"+newtag+">"+addedInfo+"</"+newtag+">";
                		out<<"\n";
			}
        	}//end of while
        	in.close();
        	out.close();

		//write back the content of the temp xml file to IoT_DDL file
		ifstream inn(TMP_FilePath);
		ofstream outt(DDL_FilePath);
		if (inn.is_open()){
        		while (getline(inn,line)){
				outt<<line;
            			outt<<"\n";
        		}//end of while
        		inn.close();
        		outt.close();
    		}else {
        		inn.close();
        		outt.close();
    		}
		//done writing back to IoTDDL
    	}else {
        	in.close();
        	out.close();
    	}
}

/*
* Add data for a new XML tag of third level
*/
void IoTDDL_Parser::addXMLTag(string maintag, string mainTag2, string newtag, string addedInfo){
	string line;
	bool mainTagB = false;
	bool mainTag2B = false;
	std::size_t found=0;
	ifstream in(DDL_FilePath);
	ofstream out(TMP_FilePath);
    	if (in.is_open()){
        	while (getline(in,line)){
			found = line.find("<"+maintag+">");
            		if (found!=std::string::npos){
                		mainTagB = true;
            		}
			if(mainTagB){
				found = line.find("<"+mainTag2+">"); //found tag in the mainTag seciton
				if (found!=std::string::npos){
					mainTag2B = true;
				}
			}
			out<<line;
            		out<<"\n";
			if(mainTagB&&mainTag2B){
				mainTagB = false;
                		out<<"<"+newtag+">"+addedInfo+"</"+newtag+">";
                		out<<"\n";
			}
        	}//end of while
        	in.close();
        	out.close();

		//write back the content of the temp xml file to IoT_DDL file
		ifstream inn(TMP_FilePath);
		ofstream outt(DDL_FilePath);
		if (inn.is_open()){
        		while (getline(inn,line)){
				outt<<line;
            			outt<<"\n";
        		}//end of while
        		inn.close();
        		outt.close();
    		}else {
        		inn.close();
        		outt.close();
    		}
		//done writing back to IoTDDL
    	}else {
        	in.close();
        	out.close();
    	}
}

/*
* Add data for a new XML tag of forth level
*/
void IoTDDL_Parser::addXMLTag(string maintag, string mainTag2, string mainTag3, string newtag, string addedInfo){
	string line;
	bool mainTagB = false;
	bool mainTag2B = false;
	bool mainTag3B = false;
	std::size_t found=0;
	ifstream in(DDL_FilePath);
	ofstream out(TMP_FilePath);
    	if (in.is_open()){
        	while (getline(in,line)){
			found = line.find("<"+maintag+">");
            		if (found!=std::string::npos){
                		mainTagB = true;
            		}
			if(mainTagB){
				found = line.find("<"+mainTag2+">"); //found tag in the mainTag seciton
				if (found!=std::string::npos){
					mainTag2B = true;
				}
			}
			if(mainTagB&&mainTag2B){
				found = line.find("<"+mainTag3+">"); //found tag in the mainTag seciton
				if (found!=std::string::npos){
					mainTag3B = true;
				}
			}
			out<<line;
            		out<<"\n";
			if(mainTagB&&mainTag2B&&mainTag3B){
				mainTagB = false;
                		out<<"<"+newtag+">"+addedInfo+"</"+newtag+">";
                		out<<"\n";
			}
        	}//end of while
        	in.close();
        	out.close();

		//write back the content of the temp xml file to IoT_DDL file
		ifstream inn(TMP_FilePath);
		ofstream outt(DDL_FilePath);
		if (inn.is_open()){
        		while (getline(inn,line)){
				outt<<line;
            			outt<<"\n";
        		}//end of while
        		inn.close();
        		outt.close();
    		}else {
        		inn.close();
        		outt.close();
    		}
		//done writing back to IoTDDL
    	}else {
        	in.close();
        	out.close();
    	}
}
