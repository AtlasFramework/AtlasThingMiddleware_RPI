# change this to your cppmicroservices install directory
US.dir := /usr/local

#------------------------------------------------------------------------------#
#------------------------------------------------------------------------------#

# GNU AutoGen is a new dependency in addition to cppmicroservices
CXX := g++
AR  := usResourceCompiler4
AG  := autogen

# same flags as described in `cppus-test'
CXXFLAGS := -std=c++14 -Wall -fPIC -lwiringPi -lm -lrt -lcrypt 
DIRFLAGS := -I$(US.dir)/include/cppmicroservices/ -I..
LDFLAGS  := -Wl,-rpath,.
LIBFLAGS := -lCppMicroServicesd

# dynamic Makefile target
TARGET := $(firstword $(MAKECMDGOALS))

# allow the bundle dependency to be specified by its folder name
./$(TARGET): ./$(TARGET)/lib$(TARGET).so
	@echo "built bundle $(TARGET)"

./$(TARGET)/lib$(TARGET).so: ./$(TARGET)/main.o | ./$(TARGET)/resources.zip
	@echo "> creating $@..."
	@$(CXX) -shared -o $@ $^ $(CXXFLAGS) $(LDFLAGS) $(LIBFLAGS)
	@$(AR) -z $| -b $@

./$(TARGET)/main.o: ./$(TARGET)/main.cpp
	@$(CXX) $(CXXFLAGS) $(DIRFLAGS) -DUS_BUNDLE_NAME=$(TARGET) -o $@ -c $<

# build the resources zip (always no folder, only the manifest)
./$(TARGET)/resources.zip: ./$(TARGET)/manifest.json
	@echo "> generating resources..."
	@$(AR) -m $< -n $(TARGET) -o $@

# generate the sources
./$(TARGET)/main.cpp: ./$(TARGET)/service.def
	@echo "> generating bundle source code..."
	@$(AG) -T ../ServiceTemplate/service.tpl ./$(TARGET)/service.def
	@$(RM) $@
	@mv service.cpp $@

./$(TARGET)/manifest.json: ./$(TARGET)/main.cpp
	@$(RM) $@
	@mv service.json $@
