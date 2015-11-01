#include "m_pd.h"
#include <string.h>

#define DEFSENDVS 64    /* LATER get send to get this from canvas */
#define DEBUG 1

/* ----------------------------- mcatch~ ----------------------------- */
static t_class *mcatch_tilde_class;

typedef struct _mcatch_tilde
{
    t_object x_obj;
    t_symbol *x_sym;
    int x_n;
    t_sample *x_vec;
    int x_first;
    int* x_executed;
    int* x_total_instances;
    int cr_i_am;
    int instance;

} t_mcatch_tilde;


char cr_names[40][20];
int cr_i=0;
void *cr_class[40];

int find_class_ix(t_symbol *s)
{

    int i,j=-1;
    for( i=0;i<cr_i;i++)
    {
        if( strcmp(  cr_names[i], s->s_name )==0 )
        {
            j = i;
            break;
        }
    }

    return j;
}




void *mcatch_tilde_new(t_symbol *s)
{

    t_mcatch_tilde *x = (t_mcatch_tilde *)pd_new(mcatch_tilde_class);

    x->x_sym = s;
    x->x_n = DEFSENDVS;
	x->x_first=0;

    int nuevo = 1;

    int j;
    j = find_class_ix(s);

    if(j!=-1)
    {
        nuevo = 0;
        x->cr_i_am = j; 
    }
        
    
    if(nuevo == 1)
    {
    	// cr_class[cr_i] = (t_mcatch_tilde *)getbytes( sizeof(t_mcatch_tilde) );
        // memcpy( cr_class[cr_i], x, 1);
        cr_class[cr_i] = &x->x_obj.ob_pd;


    	strcpy( cr_names[cr_i] , s->s_name );
        x->cr_i_am = cr_i;
        
        // pd_bind(&x->x_obj.ob_pd, s);


        x->x_vec = (t_sample *)getbytes(DEFSENDVS * sizeof(t_sample));
        memset((char *)(x->x_vec), 0, DEFSENDVS * sizeof(t_sample));    

        //Memory allocation for shared variables between instances
        x->x_total_instances  = getbytes(sizeof(int));
        x->x_executed = getbytes(sizeof(int));
		
		*(x->x_total_instances) = 0;
		*(x->x_executed) = 0;

        x->instance = 0;

		post("Cacho: %d %s. Cacho nuevo. Instancia n=%d",x->cr_i_am,s->s_name,*(x->x_total_instances));
        if (DEBUG) printf("Cacho: %d %s. Cacho nuevo. Instancia n=%d\n",x->cr_i_am,s->s_name,*(x->x_total_instances));

        cr_i++;
		x->x_first = 1;

    }
    else
    {   
    	
        t_mcatch_tilde *catcher = (t_mcatch_tilde *) cr_class[x->cr_i_am];

        // t_mcatch_tilde *catcher = (t_mcatch_tilde *)pd_findbyclass((x->x_sym = s), mcatch_tilde_class);
        x->x_vec = catcher->x_vec;

        x->x_total_instances = catcher->x_total_instances;

        *(x->x_total_instances) = *(x->x_total_instances) + 1 ;

        x->instance = *(x->x_total_instances);

        x->x_executed = catcher->x_executed;

		post("Cacho: %d %s. Cacho existente. Instancia n=%d",x->cr_i_am,s->s_name,*(x->x_total_instances));
		if(DEBUG) printf("Cacho: %d %s. Cacho existente. Instancia n=%d\n",x->cr_i_am,s->s_name,*(x->x_total_instances));

    }

    outlet_new(&x->x_obj, &s_signal);

    return (x);
}

t_int *mcatch_tilde_perform(t_int *w)
{

    t_mcatch_tilde *x = (t_mcatch_tilde *)(w[1]);
    
    t_sample *in = (t_sample *)(w[2]);
    t_sample *out = (t_sample *)(w[3]);
    
    int n = (int)(w[4]);

    while (n--) 
    {
        *out++ = *in;
        if( *(x->x_executed)==*(x->x_total_instances) )
            *in++ = 0; 
    }

    *(x->x_executed) = *(x->x_executed) + 1;
    if(*(x->x_executed)>*(x->x_total_instances))
        *(x->x_executed)=0;

    return (w+5);
}

/* tb: vectorized catch function */
t_int *mcatch_tilde_perf8(t_int *w)
{
    
    t_mcatch_tilde *x = (t_mcatch_tilde *)(w[1]);

    t_sample *in = (t_sample *)(w[2]);
    t_sample *out = (t_sample *)(w[3]);
    
    int n = (int)(w[4]);



    for (; n; n -= 8, in += 8, out += 8)
    {
       out[0] = in[0]; out[1] = in[1]; out[2] = in[2]; out[3] = in[3]; 
       out[4] = in[4]; out[5] = in[5]; out[6] = in[6]; out[7] = in[7]; 
    
        if( *(x->x_executed)==*(x->x_total_instances) )
        {
           in[0] = 0; in[1] = 0; in[2] = 0; in[3] = 0; 
           in[4] = 0; in[5] = 0; in[6] = 0; in[7] = 0;                 
        }
       
    }
        
	*(x->x_executed) = *(x->x_executed) + 1;
    if(*(x->x_executed)>*(x->x_total_instances))
        *(x->x_executed)=0;

    return (w+5);
}

void mcatch_tilde_dsp(t_mcatch_tilde *x, t_signal **sp)
{
    if (x->x_n == sp[0]->s_n)
    {
        if(sp[0]->s_n&7)
        dsp_add(mcatch_tilde_perform, 4, x,  x->x_vec, sp[0]->s_vec, sp[0]->s_n);
        else
        dsp_add(mcatch_tilde_perf8, 4, x, x->x_vec, sp[0]->s_vec, sp[0]->s_n);
    }
    else error("mcatch_tilde %s: unexpected vector size", x->x_sym->s_name);
}

void mcatch_tilde_free(t_mcatch_tilde *x)
{
    *(x->x_total_instances) = *(x->x_total_instances) - 1;
    
    // if(*(x->x_total_instances)==0) //one left alone
   

    if(*(x->x_total_instances)==-1)//all gone
    {
        // pd_unbind(&x->x_obj.ob_pd, x->x_sym);
        
        
        // pd_unbind( cr_class[x->cr_i_am], x->x_sym);

        strcpy( cr_names[x->cr_i_am] , "" );
        cr_class[x->cr_i_am] = NULL;

        freebytes(x->x_vec, x->x_n * sizeof(t_sample));
        freebytes(x->x_total_instances, sizeof(int) );
        freebytes(x->x_executed, sizeof(int) );

        post("Cacho: %d %s. Se borró la %d. No hay más instancias. ",x->cr_i_am,x->x_sym->s_name,x->instance );
        if(DEBUG) printf("Cacho: %d %s. Se borró la %d. No hay más instancias \n",x->cr_i_am,x->x_sym->s_name,x->instance );

    }
    else
    {
        post("Cacho: %d %s. Se borró la %d. Cantidad de instancias n=%d",x->cr_i_am,x->x_sym->s_name,x->instance ,*(x->x_total_instances));
        if(DEBUG) printf("Cacho: %d %s. Se borró la %d. Cantidad de instancias n=%d\n",x->cr_i_am,x->x_sym->s_name,x->instance ,*(x->x_total_instances));
        if(DEBUG) printf("%g\n", x->x_vec[0]);
    }



    // if(x->x_first==1)
    // {
    //     pd_unbind(&x->x_obj.ob_pd, x->x_sym);
    //     freebytes(x->x_vec, x->x_n * sizeof(t_sample));

    //     freebytes(x->x_total_instances, sizeof(int) );
    //     freebytes(x->x_executed, sizeof(int) );

    //     post("Cacho: %d %s. No hay más instancias n=%d",cr_i,x->x_sym->s_name,*(x->x_total_instances));
    //     if(DEBUG) printf("Cacho: %d %s. No hay más instancias n=%d\n",cr_i,x->x_sym->s_name,*(x->x_total_instances));
    // }
    // else
    // {
    // 	post("Cacho: %d %s. Cantidad de instancias n=%d",cr_i,x->x_sym->s_name,*(x->x_total_instances));
    //     if(DEBUG) printf("Cacho: %d %s. Cantidad de instancias n=%d\n",cr_i,x->x_sym->s_name,*(x->x_total_instances));
    // }

    
}

void mcatch_tilde_setup(void)
{
    mcatch_tilde_class = class_new(gensym("mcatch~"), (t_newmethod)mcatch_tilde_new,
        (t_method)mcatch_tilde_free, sizeof(t_mcatch_tilde), CLASS_NOINLET, A_DEFSYM, 0);
    class_addmethod(mcatch_tilde_class, (t_method)mcatch_tilde_dsp, gensym("dsp"), 0);
    class_sethelpsymbol(mcatch_tilde_class, gensym("mthrow~"));
}



/* ----------------------------- mthrow~ ----------------------------- */
static t_class *mthrow_tilde_class;

typedef struct _mthrow_tilde
{
    t_object x_obj;
    t_symbol *x_sym;
    t_sample *x_whereto;
    int x_n;
    t_float x_f;
    int cr_i_am;

} t_mthrow_tilde;

 void *mthrow_tilde_new(t_symbol *s)
{
    t_mthrow_tilde *x = (t_mthrow_tilde *)pd_new(mthrow_tilde_class);

    int j;
    j = find_class_ix(s);

    if(j!=-1)
        x->cr_i_am=j;

    x->x_sym = s;
    x->x_whereto = 0;
    x->x_n = DEFSENDVS;
    x->x_f = 0;
    return (x);
}

 t_int *mthrow_tilde_perform(t_int *w)
{
    t_mthrow_tilde *x = (t_mthrow_tilde *)(w[1]);
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

 void mthrow_tilde_set(t_mthrow_tilde *x, t_symbol *s)
{

    //Hacer que esto ande, que busque automaticamente la clase que pertenece. Hacer que cr_class sea un mapa
    int j;
    j = find_class_ix(x->x_sym);

    t_mcatch_tilde *catcher = NULL;

    if(x->cr_i_am != -1)
        catcher = (t_mcatch_tilde *) cr_class[x->cr_i_am];
    

    // t_mcatch_tilde *catcher = (t_mcatch_tilde *)pd_findbyclass((x->x_sym = s), mcatch_tilde_class);

    if (catcher != NULL)
    {
        if (catcher->x_n == x->x_n)
        {
            x->x_whereto = catcher->x_vec;
        }
        else
        {
            post("%d\t%d",catcher->x_n, x->x_n);
            pd_error(x, "mthrow~ %s: vector size mismatch with mcatch~", x->x_sym->s_name);
            x->x_whereto = 0;
        }
    }
    else
    {
        pd_error(x, "mthrow~ %s: no matching mcatch", x->x_sym->s_name);
        x->x_whereto = 0;
    }
}

 void mthrow_tilde_dsp(t_mthrow_tilde *x, t_signal **sp)
{
    if (sp[0]->s_n != x->x_n)
    {
        pd_error(x, "mthrow~ %s: vector size mismatch", x->x_sym->s_name);
    }
    else
    {
        mthrow_tilde_set(x, x->x_sym);
        dsp_add(mthrow_tilde_perform, 3,
            x, sp[0]->s_vec, sp[0]->s_n);
    }
}

 void mthrow_tilde_setup(void)
{
    mthrow_tilde_class = class_new(gensym("mthrow~"), (t_newmethod)mthrow_tilde_new, 0,
        sizeof(t_mthrow_tilde), 0, A_DEFSYM, 0);
    class_addmethod(mthrow_tilde_class, (t_method)mthrow_tilde_set, gensym("set"),
        A_SYMBOL, 0);
    CLASS_MAINSIGNALIN(mthrow_tilde_class, t_mthrow_tilde, x_f);
    class_addmethod(mthrow_tilde_class, (t_method)mthrow_tilde_dsp, gensym("dsp"), 0);
}


void multicatch_setup(void)
{
    mcatch_tilde_setup();
    mthrow_tilde_setup();
}