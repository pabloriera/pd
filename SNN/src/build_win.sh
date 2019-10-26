#! /bin/bash
rm snn.dll
x86_64-w64-mingw32-g++ -fpic -std=c++11 -c utils.cpp -o utils.o
x86_64-w64-mingw32-g++ -fpic -std=c++11 -c Synapse.cpp -o Synapse.o
x86_64-w64-mingw32-g++ -fpic -std=c++11 -c Neuron.cpp -o Neuron.o
x86_64-w64-mingw32-g++ -fpic -std=c++11 -c Neurons_network.cpp -o Neurons_network.o

x86_64-w64-mingw32-gcc -I"/mnt/c/Program Files (x86)/Pd/include/pd" -DPD -DVERSION='""' -mms-bitfields -Wall -W -g -std=c++11 -O3 -funroll-loops -fomit-frame-pointer -o "snn.o" -c "snn.cpp"

#x86_64-w64-mingw32-g++ -fpic -std=c++11 -c snn.cpp -o snn.o 
# x86_64-w64-mingw32-g++ -fpic -std=c++11 -shared utils.o snn.o Neurons_network.o Synapse.o Neuron.o -o snn.dll -L -lpd
#x86_64-w64-mingw32-ld -export_dynamic -o snn.dll utils.o snn.o Neurons_network.o Synapse.o Neuron.o -lc -lm -lpd

x86_64-w64-mingw32-g++  -lm -s -shared -Wl,--enable-auto-import -o "snn.dll" "snn.o" -L"/mnt/c/Program Files (x86)/Pd/src" -L"/mnt/c/Program Files (x86)/Pd/bin" -L"/mnt/c/Program Files (x86)/Pd/obj" -lpd -lwsock32 -lkernel32 -luser32 -lgdi32

cp snn.dll ../
