CXX:=$(shell sh -c 'type $(CXX) >/dev/null 2>/dev/null && echo $(CXX) || echo g++')

OPTIMIZATION = -O0

WARNINGS  =     \
                -Wall \
                -Wno-sign-compare \
                -Wno-deprecated-register \
                -Wno-unused-function

DEBUG =-g

STDC = -std=c++1y

LDFLAGS = -lstdc++ -pthread

INCLUDES =

ASIO_STANDALONE_FLAG = -DASIO_STANDALONE

CXXFLAGS = $(OPTIMIZATION) -fPIC -fstack-protector $(CFLAGS) $(WARNINGS) $(DEBUG) $(STDC) -DUSE_CXX0X

STDLIB =
ifeq ($(shell uname -s), FreeBSD)
STDLIB +=  -stdlib=libc++
LIBXML_INCLUDES =
LIBXML_INCLUDES = -I/usr/local/include/libxml2
FREEBSD_ASIO_INCLUDE ?= -I/raspberry/asio/product/include
INCLUDES += $(FREEBSD_ASIO_INCLUDE)

endif
CXXFLAGS +=  $(STDLIB)

CFLAGS=-c -Wall

SOURCES = cdnsd.cpp

EXECUTABLE = cdnsd-bin

OBJECTS=$(SOURCES:.cpp=.o)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(ASIO_STANDALONE_FLAG) $(INCLUDES) $(CXXFLAGS) $< -o $@

clean:
	rm $(OBJECTS) $(EXECUTABLE) $(TESTING_OBJ) $(TESTING_EXEC)

