# Atlas Thing Middleware for RaspberryPi

This is the current version (V1.0) for Atlas Thing Middleware for your RaspberryPi (IoT platform that runs a Raspbian, a free operating system based on Linux Debian)-Smart things

## How Atlas-IoT_Thing project directory is organized?

### Main/ : 
holds the entry point to compile Atlas-IoT_Thing  (readme file, cMServices shell script, cMakeLists.txt)

### Architecture/ : holds the following sub-modules of the architecture
		- Communication Manager       : holds the different communication languages supported (MQTT) and the Unix-sockets (CoAP and REST-HTTP are not supported in this version)
		- IoTDDL Parser               : parses the different sections and sub-sections of the IoT-DDL
		- Identity Parser             : builds the identity/language/entity json tweets of the thing
		- API-Generator               : generates bundles to the offered services, handle the service call and generate API to the service 
		- Knowledge Tweeting Manager  : builds the service/relationship json tweets, build tree of services-relationships, find match of the unbounded services
		- Object Engine               : currently not in this version
		- Attachment Engine           : currently not in this version
		- System Function             : set of common system functions that can be called besides the generated bundles for thing's services

### lib/ : holds the different libraries utilized in the project
		- RapidJSON : JSON parser/generator for C++ with both SAX/DOM style API (http://rapidjson.org/)
		- TinyXML2  : C++ XML parser under the ZLib license
		- PahoMQTT  : Eclipse Paho MQTT C client library
		- CoAPLib   : C implementation of the lightweight CoAP application-protocol from the noisy atom, under a BSD license
                - Crypto++  : C++ library of the cryptographic schemes licensed under the Boost Software License 1.0
                - OMALib    : Eclipse Wakaama (formerly liblwm2m), implementation of the Open Mobile Alliance's LightWeight M2M protocol (LwM2M)

### ConfigurationFiles/ : holds the different configuration files (e.g., Atlas IoTDDL) 
[Use the IoT-DDL builder online tool to build an IoT-DDL file for your Atlas thing.](https://atlasframework.github.io/IoT-DDL/DDL_Builder/builder.html)

there is an initial IoTDDL in this version, if you will use it without replacing it with a new one - make sure to change the thing ID in the file.

<br/>
<br/>
<br/>


## Prepare your Atlas smart thing on RaspberryPi through the following steps:


### Step1: run the following linux commands through terminal:

- sudo apt-get update
- sudo apt-get upgrade
- sudo apt-get install gcc-6 g++-6 build-essential    //usually install the latest gcc and g++                                 
 - sudo apt-get install doxygen
 - sudo apt-get install cmake cmake-curses-gui
 - sudo apt-get install libboost-all-dev
 - sudo apt-get install curl libcurl4-openssl-dev
 - sudo apt-get install autogen

### Step2: Get the latest version of the middleware:
From Github, download the zip version of the middleware on your RaspberryPi, then unzip the folder.

### Step3: Install cppMicroservices library
 - unzip the folder named CppMicroServices-development under Atlas-IoT_Thing/lib/ of the middleware, and keep in the lib directory
 - cmake CppMicroServices-development/
 - sudo make
 - sudo make install
 - LD_LIBRARY_PATH=/usr/local/include
 - export LD_LIBRARY_PATH
 - sudo ldconfig
 - //the new version installs the library in /usr/local/include/cppmicroservices4/ rather than /usr/local/include/
 - sudo mv /usr/local/include/cppmicroservices4/ ~/Desktop/
 - sudo mv ~/Desktop/cppmicroservices4/cppmicroservices/ /usr/local/include/


### Step4: Install WiringPi library and enable the hardware interfaces
 - unzip the folder named WiringPi-master under Atlas-IoT_Thing/lib/WiringPi-master/ of the middleware
 - cd to the WiringPi-master folder
 - ./build
 - sudo apt update
 - sudo apt upgrade
 - sudo apt install rpi.gpio
 - sudo raspi-config
 - under “Interfacing Options”, enable both I2C and SPI

### Step5: Compile and Build Atlas middleware
 - Navigate to the directory of Atlas-IoT_thing (use cd command) and Compile as follows:
 - cmake Main/
 - make

### Step6: Add an IoT-DDL
 - [Use this builder tool to build an IoT-DDL file for your Atlas thing.](https://atlasframework.github.io/IoT-DDL/DDL_Builder/builder.html)
 - Navigate to the directory of Atlas-IoT_thing and add the genenerated IoT-DDL.xml file to the /ConfigurationFiles directory (replace the default file)
 
### Step7: Run Atlas middleware
 - Through therminal, and under the directlory of the middleware, run the following command:
 - ./Atlas




