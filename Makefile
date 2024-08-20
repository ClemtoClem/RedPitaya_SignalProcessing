PROJECT_NAME = RedPitaya_SignalProcessing 
CXX		 = $(CROSS_COMPILE)g++
SRCDIR	  = ./src
BINDIR	  = ./bin

INSTALL_DIR ?= /opt/redpitaya/

CFLAGS	  = -std=c++20 -Wall -Wextra -pedantic -g
CFLAGS	 += -I$(INSTALL_DIR)/include
CFLAGS	 += -I$(INSTALL_DIR)/include/apiApp
CFLAGS	 += -I$(INSTALL_DIR)/include/rp-api
CFLAGS	 += -I$(INSTALL_DIR)/include/api
CFLAGS	 += -I$(INSTALL_DIR)/include/api2
CFLAGS	 += -I$(INSTALL_DIR)/rp_sdk
CFLAGS	 += -I$(SRCDIR)

LDFLAGS	 = -L$(INSTALL_DIR)/lib
LDFLAGS	+= -L$(INSTALL_DIR)/rp_sdk

LDLIBS	  = -static -lm -lrp-hw-can -lrp -lrp-hw-calib -lrp-hw-profiles
LDLIBS	 += -lrp-gpio -lrp-i2c -lrp_sdk -lrpapp
LDLIBS	 += -lrp-hw -lm -lstdc++ -lpthread -li2c -lsocketcan

# List of source files
SRCS = $(wildcard $(SRCDIR)/*.cpp)

# List of object files
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.o,$(SRCS))

# Name of the final executable
EXECUTABLE = $(PROJECT_NAME)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BINDIR)/%.o: $(SRCDIR)/%.cpp | $(BINDIR)
	$(CXX) $(CFLAGS) -c $< -o $@

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	$(RM) $(OBJS) $(EXECUTABLE)
