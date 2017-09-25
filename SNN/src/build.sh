#! /bin/bash
rm snn.pd_linux
g++ -fpic -std=c++11 -c utils.cpp -o utils.o
g++ -fpic -std=c++11 -c Synapse.cpp -o Synapse.o
g++ -fpic -std=c++11 -c Neuron.cpp -o Neuron.o
g++ -fpic -std=c++11 -c Neurons_network.cpp -o Neurons_network.o
g++ -fpic -std=c++11 -c snn.cpp -o snn.o
g++ -fpic -std=c++11 -shared utils.o snn.o Neurons_network.o Synapse.o Neuron.o -o snn.pd_linux
cp snn.pd_linux ../