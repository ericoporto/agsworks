UNAME := $(shell uname)

BASE_subdirs := \
src \
src/misc \
src/service/base \
src/service/gog \
src/service/steam

BASE := $(wildcard $(addsuffix /*.cpp, $(BASE_subdirs)))
OBJS := $(BASE:.cpp=.o)

INCDIR := ../ags/Engine ../ags/Common src/
LIBDIR :=
CC := gcc
CXX := g++
CFLAGS := -fPIC -fvisibility=hidden -O2 -g -Wall -std=c++11
LIBS := -lstdc++ -ldl

ifeq ($(UNAME), Darwin)
TARGET := libagsworks.dylib
else
TARGET := libagsworks.so
endif

CXXFLAGS := $(CFLAGS)
CFLAGS := $(addprefix -I,$(INCDIR)) $(CFLAGS)
CXXFLAGS := $(CFLAGS) $(CXXFLAGS)
ASFLAGS := $(CFLAGS) $(ASFLAGS)

.PHONY: clean all

all: $(TARGET)

$(TARGET): $(OBJS)
ifeq ($(UNAME), Darwin)
	$(CXX) -shared -o $@ $^ $(LDFLAGS) $(LIBS)
else
	$(CXX) -shared -o $@ $^ $(LDFLAGS) $(LIBS)
endif

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS)
