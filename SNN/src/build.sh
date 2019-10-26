#! /bin/bash
rm snn.pd_linux
g++ -fpic -std=c++11 -O6 -funroll-loops -fomit-frame-pointer -c Synapse.cpp -o Synapse.o
g++ -fpic -std=c++11 -O6 -funroll-loops -fomit-frame-pointer -c Neuron.cpp -o Neuron.o
g++ -fpic -std=c++11 -O6 -funroll-loops -fomit-frame-pointer -c Neurons_network.cpp -o Neurons_network.o
g++ -fpic -std=c++11 -O6 -funroll-loops -fomit-frame-pointer -c snn.cpp -o snn.o
g++ -fpic -std=c++11 -O6 -funroll-loops -fomit-frame-pointer  -c utils.cpp -o utils.o
g++ -fpic -std=c++11 -shared utils.o snn.o Neurons_network.o Synapse.o Neuron.o -o snn.pd_linux
cp snn.pd_linux ../