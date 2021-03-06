CCFLAGS=-O2 -g
SRCFILES=meerkat.cpp networks.cpp parsing.cpp signal_handling.cpp
EXPERIMENTFILES=meerkat_experiment.cpp networks.cpp parsing_experiment.cpp signal_handling.cpp

ifeq ($(OS),Windows_NT)
	CCFLAGS += -D WIN32
	ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
		CCFLAGS += -D AMD64
	endif
	ifeq ($(PROCESSOR_ARCHITECTURE),x86)
		CCFLAGS += -D IA32
	endif
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		CCFLAGS += -std=c++11 -D LINUX
	endif
	ifeq ($(UNAME_S),Darwin)
		CCFLAGS += -D OSX
	endif
	UNAME_P := $(shell uname -p)
	ifeq ($(UNAME_P),x86_64)
		CCFLAGS += -D AMD64
	endif
	ifneq ($(filter %86,$(UNAME_P)),)
		CCFLAGS += -D IA32
	endif
	ifneq ($(filter arm%,$(UNAME_P)),)
		CCFLAGS += -D ARM
	endif
endif

all: meerkat meerkat-experiment

meerkat: $(SRCFILES)
	g++ $(CCFLAGS) -o meerkat $^

meerkat-experiment: $(EXPERIMENTFILES)
	g++ $(CCFLAGS) -o meerkat-experiment $^

clean:
	rm -rf *.o *.dSYM meerkat meerkat-experiment
