CXX = g++
CXXFLAGS= -std=c++17-Wall-Wextra-pthread
app:main.cpp threadfuncs.cpp
$(CXX)$(CXXFLAGS)$^-o$@
run:app
./app
clean:
rm-f appoutput.logtrace.log
.PHONY:runclean

