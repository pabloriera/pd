#include "m_pd.h"
#include <string.h>

#define DEFSENDVS 64    /* LATER get send to get this from canvas */

/* ----------------------------- cacho~ ----------------------------- */
static t_class *cacho_tilde_class;

int instancias_cacho=-1;
int instancias_ejecutadas=0;

typedef struct _cacho_tilde
{
    t_object x_obj;
    t_symbol *x_sym;
    int x_n;
    t_sample *x_vec;
    int x_instance;

} t_cacho_tilde;

void *cacho_tilde_new(t_symbol *s)
{

    t_cacho_tilde *x = (t_cacho_tilde *)pd_new(cacho_tilde_class);
    
    instancias_cacho++;
    x->x_instance = instancias_cacho;

    post("Instancias de cacho %d",instancias_cacho);
    

    x->x_sym = s;
    x->x_n = DEFSENDVS;

    if(x->x_instance==0)
    {
        
        pd_bind(&x->x_obj.ob_pd, s);
        x->x_vec = (t_sample *)getbytes(DEFSENDVS * sizeof(t_sample));
        memset((char *)(x->x_vec), 0, DEFSENDVS * sizeof(t_sample));    
    }
    else
    {
     
        t_cacho_tilde *catcher = (t_cacho_tilde *)pd_findbyclass((x->x_sym = s), cacho_tilde_class);
        x->x_vec = catcher->x_vec;
    }

        // printf("x->x_instance %d\n",x->x_instance);
        // pd_bind(&x->x_obj.ob_pd, s);
        // x->x_vec = (t_sample *)getbytes(DEFSENDVS * sizeof(t_sample));
        // memset((char *)(x->x_vec), 0, DEFSENDVS * sizeof(t_sample));    
   


    outlet_new(&x->x_obj, &s_signal);



    return (x);
}

t_int *cacho_tilde_perform(t_int *w)
{

    t_cacho_tilde *x = (t_cacho_tilde *)(w[1]);
    
    t_sample *in = (t_sample *)(w[2]);
    t_sample *out = (t_sample *)(w[3]);
    
    int n = (int)(w[4]);

    while (n--) 
    {
        *out++ = *in;
        if( instancias_ejecutadas==instancias_cacho )
            *in++ = 0; 
    }

    instancias_ejecutadas++;
    if(instancias_ejecutadas>instancias_cacho)
        instancias_ejecutadas=0;

    return (w+5);
}

/* tb: vectorized catch function */
t_int *cacho_tilde_perf8(t_int *w)
{
    
    t_cacho_tilde *x = (t_cacho_tilde *)(w[1]);

    t_sample *in = (t_sample *)(w[2]);
    t_sample *out = (t_sample *)(w[3]);
    
    int n = (int)(w[4]);



    for (; n; n -= 8, in += 8, out += 8)
    {
       out[0] = in[0]; out[1] = in[1]; out[2] = in[2]; out[3] = in[3]; 
       out[4] = in[4]; out[5] = in[5]; out[6] = in[6]; out[7] = in[7]; 
    
        //post("COSA %d\t %d",instancias_ejecutadas,instancias_cacho);

        if( instancias_ejecutadas==instancias_cacho )
        {
           in[0] = 0; in[1] = 0; in[2] = 0; in[3] = 0; 
           in[4] = 0; in[5] = 0; in[6] = 0; in[7] = 0;                 
        }
       
    }
        
    instancias_ejecutadas++;
    if(instancias_ejecutadas>instancias_cacho)
        instancias_ejecutadas=0;

    return (w+5);
}

void cacho_tilde_dsp(t_cacho_tilde *x, t_signal **sp)
{
    if (x->x_n == sp[0]->s_n)
    {
        if(sp[0]->s_n&7)
        dsp_add(cacho_tilde_perform, 4, x,  x->x_vec, sp[0]->s_vec, sp[0]->s_n);
        else
        dsp_add(cacho_tilde_perf8, 4, x, x->x_vec, sp[0]->s_vec, sp[0]->s_n);
    }
    else error("cacho_tilde %s: unexpected vector size", x->x_sym->s_name);
}

void cacho_tilde_free(t_cacho_tilde *x)
{
    instancias_cacho--;
    
    if(x->x_instance==0)
    {
        pd_unbind(&x->x_obj.ob_pd, x->x_sym);
        freebytes(x->x_vec, x->x_n * sizeof(t_sample));
    }

    
}

void cacho_tilde_setup(void)
{
    cacho_tilde_class = class_new(gensym("cacho~"), (t_newmethod)cacho_tilde_new,
        (t_method)cacho_tilde_free, sizeof(t_cacho_tilde), CLASS_NOINLET, A_DEFSYM, 0);
    class_addmethod(cacho_tilde_class, (t_method)cacho_tilde_dsp, gensym("dsp"), 0);
    class_sethelpsymbol(cacho_tilde_class, gensym("rita~"));
}



/* ----------------------------- rita~ ----------------------------- */
static t_class *rita_tilde_class;

typedef struct _rita_tilde
{
    t_object x_obj;
    t_symbol *x_sym;
    t_sample *x_whereto;
    int x_n;
    t_float x_f;
} t_rita_tilde;

 void *rita_tilde_new(t_symbol *s)
{
    t_rita_tilde *x = (t_rita_tilde *)pd_new(rita_tilde_class);
    x->x_sym = s;
    x->x_whereto  = 0;
    x->x_n = DEFSENDVS;
    x->x_f = 0;
    return (x);
}

 t_int *rita_tilde_perform(t_int *w)
{
    t_rita_tilde *x = (t_rita_tilde *)(w[1]);
    t_sample *in = (t_sample *)(w[2]);
    int n = (int)(w[3]);
    t_sample *out = x->x_whereto;
    if (out)
    {
        while (n--)
        {
            *out += (PD_BIGORSMALL(*in) ? 0 : *in);
            out++;
            in++;
        }
    }
    return (w+4);
}

 void rita_tilde_set(t_rita_tilde *x, t_symbol *s)
{


    t_cacho_tilde *catcher = (t_cacho_tilde *)pd_findbyclass((x->x_sym = s), cacho_tilde_class);


    if (catcher)
    {
        if (catcher->x_n == x->x_n)
        {
            x->x_whereto = catcher->x_vec;
        }


        else
        {
            pd_error(x, "rita~ %s: vector size mismatch", x->x_sym->s_name);
            x->x_whereto = 0;
        }
    }
    else
    {
        pd_error(x, "rita~ %s: no matching catch", x->x_sym->s_name);
        x->x_whereto = 0;
    }
}

 void rita_tilde_dsp(t_rita_tilde *x, t_signal **sp)
{
    if (sp[0]->s_n != x->x_n)
    {
        pd_error(x, "rita~ %s: vector size mismatch", x->x_sym->s_name);
    }
    else
    {
        rita_tilde_set(x, x->x_sym);
        dsp_add(rita_tilde_perform, 3,
            x, sp[0]->s_vec, sp[0]->s_n);
    }
}

 void rita_tilde_setup(void)
{
    rita_tilde_class = class_new(gensym("rita~"), (t_newmethod)rita_tilde_new, 0,
        sizeof(t_rita_tilde), 0, A_DEFSYM, 0);
    class_addmethod(rita_tilde_class, (t_method)rita_tilde_set, gensym("set"),
        A_SYMBOL, 0);
    CLASS_MAINSIGNALIN(rita_tilde_class, t_rita_tilde, x_f);
    class_addmethod(rita_tilde_class, (t_method)rita_tilde_dsp, gensym("dsp"), 0);
}


void cachorita_setup(void)
{
    cacho_tilde_setup();
    rita_tilde_setup();
}