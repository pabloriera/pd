#include "Neurons_network.h"
#include "m_pd.h"

static t_class *snn_class;

typedef struct _snn {

	t_object xSNN_obj;
	
	t_outlet *xSNN_outlet_1;
	t_outlet *xSNN_outlet_2;
	t_outlet *xSNN_outlet_3;

	bool fire_list_toggle;

	vector<size_t> neurons_potential;

	Neurons_network NN;

} t_snn;

void snn_bang(t_snn *xSNN)
{

	vector<size_t> fired;
    fired = xSNN->NN.update();

	for (std::vector<size_t>::iterator it = fired.begin() ; it != fired.end(); ++it)
		outlet_float(xSNN->xSNN_outlet_1,*it);   

	// fire list
	if(xSNN->fire_list_toggle)
	{
		size_t size = xSNN->NN.neurons.size();

		t_atom *m;
        m = (t_atom *)getbytes(size*sizeof(t_atom));
        for (size_t i=0;i<size;i++)
        {
        	SETFLOAT(&m[i], (t_float)0);
        }

        for (std::vector<size_t>::iterator it = fired.begin() ; it != fired.end(); ++it)
          	SETFLOAT(&m[*it], (t_float) 1);

        outlet_list(xSNN->xSNN_outlet_2, gensym("list"), size, m);
		freebytes(m,size*sizeof(t_atom));
	}

	// potential trace
	size_t size = xSNN->neurons_potential.size();
	if(size>0)
	{
		t_atom *m;
        m = (t_atom *)getbytes(size*sizeof(t_atom));
        size_t i = 0;
		for (std::vector<size_t>::iterator it = xSNN->neurons_potential.begin() ; it != xSNN->neurons_potential.end(); ++it)
		{
			SETFLOAT(&m[i], (t_float)xSNN->NN.neurons[(size_t)*it]->V );
			i++;
		}

		outlet_list(xSNN->xSNN_outlet_3, gensym("list"), size, m);
		freebytes(m,size*sizeof(t_atom));
	}
	
}

void snn_neurons_potential(t_snn *xSNN,  t_symbol *s, size_t argc, t_atom *argv)
{
	xSNN->neurons_potential.clear();

	size_t n = argc;
	while(n--){
    	t_float f = atom_getfloat(argv++);
    	xSNN->neurons_potential.push_back(f);
    }
}

void snn_toggle_fire_list(t_snn *xSNN, t_floatarg togg)
{
	xSNN->fire_list_toggle = togg;
}



void snn_anything(t_snn *xSNN,  t_symbol *s, size_t argc, t_atom *argv)
{
	//post("%s",s->s_name);

	size_t n = argc;
	// post("%d",n);
	// while(n--){
 //    	post("%g",atom_getfloat(&argv[n]) );
 //    }
    string sym = string(s->s_name);
    // post("%s",sym.c_str());

	if(sym.compare("set_syn_w_matrix")==0)
	{
		if(argc==3)
		{
	    	xSNN->NN.set_syn_w_matrix(atom_getfloat(&argv[0]),
	    							  atom_getfloat(&argv[1]),
	    							  atom_getfloat(&argv[2]));
		}
	}
	else if(sym.compare("set_syn_d_matrix")==0)
	{
		if(argc==2)
		{
	    	xSNN->NN.set_syn_d_matrix(atom_getfloat(&argv[0]),
	    							  atom_getfloat(&argv[1]));
		}
	}
	else if(sym.compare("set_syn_w")==0)
	{
		if(argc==3)
		{
			xSNN->NN.set_syn_w(atom_getfloat(&argv[0]),
							   atom_getfloat(&argv[1]),
					           atom_getfloat(&argv[2]));
		}
	}
	else if(sym.compare("set_currents")==0)
	{
		if(argc==2)
		{
			xSNN->NN.set_currents(atom_getfloat(&argv[0]),atom_getfloat(&argv[1]));
		}
	}
	else if(sym.compare("set_current")==0)
	{
		if(argc==2)
		{
			xSNN->NN.set_current(atom_getfloat(&argv[0]),atom_getfloat(&argv[1]));
		}
	}
	else if(sym.compare("add_neurons")==0)
	{
		if(argc==1)
		{
			xSNN->NN.add_neurons(atom_getfloat(&argv[0]));
		}
	}
	else if(sym.compare("add_all_synapses")==0)
	{
		xSNN->NN.add_all_synapses();
	}
	else if(sym.compare("set_dts")==0)
	{
		xSNN->NN.set_dts(atom_getfloat(&argv[0]));
	}
	else if(sym.compare("clear")==0)
	{
		xSNN->NN.clear_all();
	}
	else if(sym.compare("prsize_t_nn")==0)
	{
		std::cout << xSNN->NN.dump_json() << std::endl;
	}
}

void snn_append_nn(t_snn *xSNN, t_symbol *str)
{

	if(xSNN->NN.load_from_json(str->s_name))
	{
		post("Loading %s",str->s_name );
		post("%s",xSNN->NN.dump_json().c_str());
	}
	else
	{
		post("File doesn't exist: %s ",str->s_name );

	}
}

void snn_free(t_snn *xSNN)
{

}

//void *snn_new(t_t_floatarg a,t_t_floatarg b,t_t_floatarg c,t_t_floatarg d,t_t_floatarg maxV,t_t_floatarg dt)
void *snn_new(t_symbol *s, size_t argc, t_atom *argv) /* A_GIMME requiere de estos tres argumentos*/
{

  std::cout << "#### 1" << std::endl;

  t_snn *xSNN = (t_snn *)pd_new(snn_class);

  std::cout << "#### 2" << std::endl;

  xSNN->fire_list_toggle = false;
  // xSNN->f = atom_getfloat(argv);
 
  // floatinlet_new (&xSNN->xSNN_obj, &xSNN->f);

  xSNN->xSNN_outlet_1 = outlet_new(&xSNN->xSNN_obj, &s_float);
  xSNN->xSNN_outlet_2 = outlet_new(&xSNN->xSNN_obj, 0);
  xSNN->xSNN_outlet_3 = outlet_new(&xSNN->xSNN_obj, 0);
  
  std::cout << "#### 3" << std::endl;

  xSNN->NN = Neurons_network();

  std::cout << "#### 4" << std::endl;

  xSNN->NN.setup();

  std::cout << "#### 5" << std::endl;


  // xSNN->NN.add_neuron(10);
  // xSNN->NN.add_all_synapses();

  return (void *)xSNN;
}

extern "C"
{
//__declspec(dllexport) void snn_setup(void);
	void snn_setup(void) {

	  snn_class = class_new(gensym("snn"),
								(t_newmethod)snn_new,
								(t_method)snn_free,
								sizeof(t_snn), 
								CLASS_DEFAULT, 
								A_GIMME, 
								0);

	  
	  class_addmethod(snn_class,(t_method)snn_append_nn, gensym("append_nn"),A_SYMBOL, 0);
	  class_addmethod(snn_class,(t_method)snn_anything, gensym("anything"),A_GIMME,0);
	  
	  class_addmethod(snn_class,(t_method)snn_toggle_fire_list, gensym("toggle_fire_list"),A_DEFFLOAT, 0);
	  class_addmethod(snn_class,(t_method)snn_neurons_potential, gensym("neurons_potential"),A_GIMME,0);
	  	  class_addbang(snn_class, snn_bang);


	}
}