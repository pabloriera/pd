#! /bin/bash
rm snn.dll
x86_64-w64-mingw32-g++ -fpic -std=c++11 -c utils.cpp -o utils.o
x86_64-w64-mingw32-g++ -fpic -std=c++11 -c Synapse.cpp -o Synapse.o
x86_64-w64-mingw32-g++ -fpic -std=c++11 -c Neuron.cpp -o Neuron.o
x86_64-w64-mingw32-g++ -fpic -std=c++11 -c Neurons_network.cpp -o Neurons_network.o
x86_64-w64-mingw32-g++ -fpic -std=c++11 -c snn.cpp -o snn.o 
x86_64-w64-mingw32-g++ -fpic -std=c++11 -shared utils.o snn.o Neurons_network.o Synapse.o Neuron.o -o snn.dll -L -lpd

cp snn.dll ../
