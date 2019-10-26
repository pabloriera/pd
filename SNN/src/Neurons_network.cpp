#include "Neurons_network.h"

Neurons_network::Neurons_network(){

}

void Neurons_network::setup(size_t _NN_sp_buffersize)
{

    bool_syn_matrix = true;
    NN_sp_buffersize = _NN_sp_buffersize;

    neuron_id = 0;
    synapse_id = 0;
}

string Neurons_network::dump_json()
{
    json jn;

    for (neuronMap::iterator it=neurons.begin(); it!=neurons.end(); ++it)
    {
        json jp;
        jp = {{"dc",(float)it->second->dc}};
        jn[to_string(it->first)] = jp;
    }

    json jsl;

    for (synapseMap::iterator sit=synapses.begin(); sit!=synapses.end(); ++sit)
    {
        json jl = {{"from",sit->second->from->id},{"to",sit->second->to->id},{"weight",sit->second->weight}};
        jsl.push_back(jl);        
    }

    json j;
    j["neurons"] = jn;
    j["synapses"] = jsl;

    return j.dump(4);
}

bool Neurons_network::load_from_json(string filename,size_t mode)
{

    if (!file_exists(filename))
    {
        cout << "File doesn't exist" << "\n";
        return false;
    }
    
    else
    {


        ifstream t(filename);
        string str;

        t.seekg(0, ios::end);   
        str.reserve(t.tellg());
        t.seekg(0, ios::beg);

        str.assign((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());

        auto j = json::parse(str);

        size_t base_id = neuron_id;

        if (j.find("neurons") != j.end()) {
        
            json j2 = j["neurons"];
            
            for (json::iterator it = j2.begin(); it != j2.end(); ++it)
            {
                // cout << it.key() << " : " << it.value() << "\n";

                size_t id = stoi( it.key() );

                cout << "mode " << mode << "\n";

                if (mode==0)//append network
                    id+=base_id;
                
                Neuron* n;

                try
                {
                    n = neurons.at(id);
                    cout << id << " existed" << '\n';

                }
                catch (const out_of_range& oor)
                {
                    n = add_neuron(id);
                    cout << id << " adding" << '\n';
                }

                json j3 = it.value();

                if(j3.find("dc") != j3.end())
                {
                    n->dc = (float)j3["dc"];
                    cout << n->id << ": dc = " << n->dc << '\n';
                }
                    
                // for (json::iterator it3 = j3.begin(); it3 != j3.end(); ++it3)
                    // cout << it3.key() << " : " << it3.value() << "\n";         

            }

        }
        else 
            cout << "Missing neurons field" << "\n";

        if (j.find("synapses") != j.end()) {
        
            json j2 = j["synapses"];
            
            for (json::iterator it = j2.begin(); it != j2.end(); ++it)
            {
                
                json jsyn = *it;
                size_t from_id = jsyn["from"];
                size_t to_id = jsyn["to"];

                if(mode==0)//append
                {
                    from_id+=base_id;
                    to_id+=base_id;
                }

                Synapse* s;
                bool syn_existed = false;

                for (synapseMap::iterator sit=synapses.begin(); sit!=synapses.end(); ++sit)

                    if(sit->second->from->id==from_id && sit->second->to->id==to_id)
                    {
                        s = sit->second;
                        syn_existed = true;
                        cout << "synapse (" << from_id << "," << to_id << ") existed" << '\n';
                    }
                    

                if(!syn_existed)
                {
                    s = add_synapse(from_id,to_id);
                    cout << "synapse (" << from_id << "," << to_id << ") added" << '\n';
                }
                else
                {
                    if(jsyn.find("weight") != jsyn.end())
                    {
                        s->weight = jsyn["weight"];
                        cout << "synapse (" << from_id << "," << to_id << "): weight = " << s->weight << '\n';
                    }
                }   
                
                // for (json::iterator it3 = j3.begin(); it3 != j3.end(); ++it3)
                    // cout << it3.key() << " : " << it3.value() << "\n";         

            }

        }
        else 
            cout << "missing neurons field" << "\n";

        return true;
    }
}



Neuron* Neurons_network::add_neuron(size_t id)
{    
    Neuron* neuron = new Neuron;
    neurons[id] = neuron;    
    neurons[id]->setup(id);
    neurons[id]->sp_bufferSize = NN_sp_buffersize;
    neuron_id = id+1;
    return neuron;
}

void Neurons_network::add_neurons(size_t num)
{
    for (size_t i=1;i<=num;i++)
    {
        Neuron* neuron = new Neuron;
        neuron->setup(neuron_id);
        neurons[neuron_id] = neuron;
        neuron_id++;        
    }

}

Synapse* Neurons_network::add_synapse(size_t source_id,size_t target_id )
{

    Synapse* synapse = new Synapse;

    synapse->from = neuronId(source_id);
    synapse->to = neuronId(target_id);

    synapse->id = synapse_id;
    synapses[synapse_id] = synapse;

    neuronId(source_id)->synapses.push_back(synapse);

    synapse_id++;

    return synapse;

}


void Neurons_network::clear_all()
{
    neurons.clear();
    synapses.clear();
    neuron_id = 0;
    synapse_id = 0;
}


void Neurons_network::remove_neuron(size_t id)
{
    neurons.erase(id);    
}

void Neurons_network::remove_synapse(size_t id)
{
    synapses.erase(id);
}

void Neurons_network::add_all_synapses()
{
    for (neuronMap::iterator it=neurons.begin(); it!=neurons.end(); ++it)
        for (neuronMap::iterator jt=neurons.begin(); jt!=neurons.end(); ++jt)
            if (it->first!=jt->first)
            {
                bool existed = false;

                for (synapseMap::iterator st=synapses.begin(); st!=synapses.end(); ++st)
                {
                    if((st->second->from->id==it->first) && (st->second->to->id==jt->first) )
                    {
                        existed = true;
                        cout << "skipping "  << it->first << ", " <<jt->first<<endl;
                    }
                    
                }

                if(!existed)
                {
                    cout <<it->first << ", " <<jt->first<<endl;
                    add_synapse(it->first,jt->first);
                }
            }
}


vector<size_t> Neurons_network::update()
{

    vector<size_t> fired;

    for (neuronMap::iterator it=neurons.begin(); it!=neurons.end(); ++it)
    {
        size_t i = it->first;

        if(neurons[i]->update())
        {
            fired.push_back(i);
        }
    }

    if (bool_syn_matrix)
        for (synapseMap::iterator it=synapses.begin(); it!=synapses.end(); ++it)
        {
            size_t i = it->first;
            synapses[i]->to->currentBuffer( synapses[i]->weight, synapses[i]->delay, synapses[i]->from );
        }

    return fired;
}

Neuron* Neurons_network::neuronId(size_t id)
{
    neuronMap::iterator it;
    it = neurons.find(id);
    if (it != neurons.end())
        return it->second;
    else
        return 0;
}

Synapse* Neurons_network::synapseId(size_t id)
{
    
    synapseMap::iterator it;
    it = synapses.find(id);
    if (it != synapses.end())
        return it->second;
    else
        return 0;
}

Synapse* Neurons_network::synapseFromTo(size_t neuron_from_id, size_t neuron_to_id)
{    
    for (synapseMap::iterator it=synapses.begin(); it!=synapses.end(); ++it)
    {
        if (it->second->from->id==neuron_from_id && it->second->to->id==neuron_to_id )
            return it->second;
    }
    return NULL;
}

void Neurons_network::set_currents(float dc_mean, float dc_std)
{
    for (neuronMap::iterator it=neurons.begin(); it!=neurons.end(); ++it)
        it->second->dc = dc_mean + dc_std*random_normal();
}

void Neurons_network::set_current(size_t neuron_id, float dc)
{
    neuronMap::const_iterator it = neurons.find(neuron_id);
    if( it!=neurons.end() )
        neurons[neuron_id]->dc = dc;
}

void Neurons_network::set_dts(float dt)
{
    for (neuronMap::iterator it=neurons.begin(); it!=neurons.end(); ++it)
        it->second->dt = dt;
}

void Neurons_network::set_syn_w(size_t neuron_from_id, size_t neuron_to_id, float w)
{
    Synapse* syn = synapseFromTo(neuron_from_id,neuron_to_id);
    syn->weight = w;
}

void Neurons_network::set_syn_w_matrix(float type_prop, float syn_w_mean, float syn_w_std)
{
    for (synapseMap::iterator it=synapses.begin(); it!=synapses.end(); ++it)
    {
        size_t i = it->first;
        it->second->from->syn_type = (i < type_prop*neurons.size())*2-1;

        float aux = MAX(syn_w_mean + syn_w_std*random_normal(),0);
        synapses[i]->weight = aux;
    }
}

void Neurons_network::set_syn_d_matrix(float syn_d_mean, float syn_d_std)
{

    for (synapseMap::iterator it=synapses.begin(); it!=synapses.end(); ++it)
    {
        float aux = syn_d_mean + syn_d_std*random_normal();
        aux = floor( CLAMP(aux,0,it->second->to->sp_bufferSize) );
        it->second->delay = aux;
    }

}


void Neurons_network::togg_syn_matrix()
{
    bool_syn_matrix = !bool_syn_matrix;
}

void Neurons_network::reset()
{
    for (neuronMap::iterator it=neurons.begin(); it!=neurons.end(); ++it)
        it->second->reset();
}

float drand()   /* uniform distribution, (0..1] */
{    return (rand()+1.0)/(RAND_MAX+1.0);   }

float random_normal()  /* normal distribution, centered on 0, std dev 1 */
{    return sqrt(-2*log(drand())) * cos(2*M_PI*drand());    }

