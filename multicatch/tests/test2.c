#include "stdlib.h"
#include "stdio.h"

typedef struct class_t class_t;

struct class_t
{
        size_t id;

        void (*create)();

};

class_t_create() { }

int main()
{

    client_t client;
    client.AddClient = client_t_AddClient; // probably really done in some init fn

    //code ..

    client.AddClient(&client);

}
