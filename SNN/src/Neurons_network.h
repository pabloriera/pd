#pragma once

#define MAXN 100

#include "utils.h"
#include "Neuron.h"
#include "Synapse.h"

#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;

typedef map <int, Neuron*> neuronMap;
typedef map <int, Synapse*> synapseMap;

float drand();
float random_normal();

class Neurons_network {
public:
    Neurons_network();

    void setup(size_t _NN_sp_buffersize=64);
    vector<int> update();

    void clear_all();

    Neuron* add_neuron(size_t id);
    void add_neurons(size_t num=1);
    void remove_neuron(size_t id);
    Synapse* add_synapse(size_t source_id,size_t target_id );
    
    Synapse* synapseFromTo(size_t neuron_from_id, size_t neuron_to_id);

    void remove_synapse(size_t id);

    string dump_json();
    bool load_from_json(string filename, int mode = 0);
    void add_all_synapses();

    void reset();
    void draw();

    void set_dts(float dt);
    void set_currents(float dc_mean, float dc_std);
    void set_current(int neuron_id, float dc);

    void set_syn_w_matrix(float type_prop,float syn_w_mean, float syn_w_weight);
    void set_syn_d_matrix(float syn_d_mean, float syn_d_weight);

    void set_syn_w(size_t neuron_from_id, size_t neuron_to_id, float w);

    void togg_syn_matrix();

    Neuron* neuronId(size_t id);
    Synapse* synapseId(size_t id);

    neuronMap neurons;
    synapseMap synapses;

    size_t neuron_id,synapse_id;

    size_t NN_sp_buffersize;

    bool bool_syn_matrix;


};
