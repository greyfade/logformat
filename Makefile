
ifdef DEBUG
CXXFLAGS += -DDEBUG -g -pg -O0
endif
# test for c++11/14/17 support
CXXFLAGS += $(shell (echo "int main(){}" | ${CXX} -o /dev/null -std=c++14 -xc++ - && echo "-std=c++14") || (echo "int main(){}" | ${CXX} -o /dev/null -std=c++11 -xc++ - && echo "-std=c++11") || (echo "int main(){}" | ${CXX} -o /dev/null -std=c++1y -xc++ - && echo "-std=c++1y") || (echo "int main(){}" | ${CXX} -o /dev/null -std=c++0x -xc++ - && echo "-std=c++0x") )

INCLUDES = -Icatch/include

default_target: all
.PHONY : default_target

clean:
	rm -f logformat_test

logformat_test: *.cpp *.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o logformat_test logformat_test.cpp

test: clean all
	./logformat_test
.PHONY : test

all: logformat_test
.PHONY : all
