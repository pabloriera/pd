#pragma once

#include "Neuron.h"

using namespace std;

class Neuron;

class Synapse {
public:
    Synapse();

    void set(float w, float d);
    void update();
    void random_weight(float min, float max);

    Neuron* from;
    Neuron* to;

    size_t id;

	float weight, delay;
};
