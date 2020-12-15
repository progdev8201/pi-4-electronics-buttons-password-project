 echo 'compiling program...';
 g++ main.cpp tp2/Program.cpp tp2/OutputPin.cpp tp2/InputPin.cpp tp2/InputCallback.cpp -lpthread -lwiringPi -lSimpleAmqpClient -Wno-psabi -std=c++14
 echo 'program compiled successfully!';