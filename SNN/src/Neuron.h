#pragma once

#include "utils.h"
#include "Synapse.h"


class Synapse;

class Neuron {
public:
    Neuron();

    void currentBuffer(float w, float d, Neuron* _neurona);
    void reset();

    void setup(size_t id);
    bool update();


    size_t id;
    vector<Synapse*> synapses;

    //Izhikevich
    float a,b,c,d;

    float t;
    float dt;
    float V,u;
    float Vnorm;

    float maxV,minV;
    float dc;
    float I;
    float Ibuf;

    bool event;

    //Synaptic variables
    float sp,s0;
    float tau;

    //vector buffers
    vector<float> sp_buff;
    int sp_bufferSize;
    int sp_buff_ptr;

    int syn_type;
    bool fix_syn_type;



    //Firing rate
    void FRset(float Fc,float Q);
    void FRcalc();

    struct s_fr{
        vector<float> v;
        float W;
        float N;
        float B_0;
        float B1;
        float B2;
        float A1;
        float A2;
        float Fs;
        float Fc;
    } fr;

    float FR;


};
