#include "hash.h"




/*HASH ABIERTO POR QUE ES MAS FACIL*/

/*Funciones privadas */
size_t _fhash(const char*, size_t);

/*Struct de los nodos del hash*/
struct nodo_hash{
    char *clave;
    void *valor;
    /*struct nodo* ref;*/
    struct nodo_hash* ref;
};


struct hash{
    size_t cantidad;
    size_t tamanio;
    hash_destruir_dato_t destruir_dato;
    lista_t** tabla;
    /*vector_t* tabla;*/
};

hash_t* hash_crear(hash_destruir_dato_t destruir_dato)
{
    hash_t* hash = malloc(sizeof(hash_t));
    if (!hash) return NULL;
    /*Reservo el espacio que necesito para los nodos*/
    if (!(hash->tabla = calloc(TAM_INI, sizeof(nodo_hash_t))))//Quizas sea nodo_hash_t*
    {
        free(hash);
        return NULL;
    }
    hash->destruir_dato = destruir_dato;
    hash->cantidad = 0;
    hash->tamanio = (size_t) TAM_INI;;
    return hash;
}//hash_crear

bool hash_guardar(hash_t *hash, const char *clave, void *dato)
{
    /*
     *XXXXXXXXXXXXXXXXXXXXXXXXXXXX
     *Falta:
     *    Lo del factor de carga
     *    Ver cuando redimensionar
     *    ...
     *XXXXXXXXXXXXXXXXXXXXXXXXXXX
     */



    if (!hash) return NULL;//compurebo la existencia del hash
    //En este momento estoy seguro de que tengo un hash. 
    //Verifico que el vector apunte a algo.
    size_t vect_pos = _fhash(clave, hash->tamanio);

    /*[>Verifico que haya una lista en la posicion del vector<]*/
    /*if (!((hash->tabla) + vect_pos))*/
    /*{*/
        /**(hash->tabla + vect_pos) = lista_crear();*/
    /*}*/

    /*Genero el nodo*/
    nodo_hash_t* nodo_nuevo = malloc(sizeof(nodo_hash_t));
    nodo_nuevo->clave = clave;
    nodo_nuevo->valor = dato;
    nodo_nuevo->ref = NULL;
    

    /*Verifico si hay algo en la pos donde quiero guardar*/
    if (!((hash->tabla) + vect_pos))
    {
        /*Changos, parece que el lugar esta vacio*/
        *((hash->tabla) + vect_pos) = nodo_nuevo;
        hash->cantidad +=1;
        return true;
    }
    /*Aca hay un nodo, asi que lo inserto en primer lugar, y lo apunto al siguiente*/
    /*Obtengo la direccion del primer nodo*/
    nodo_hash_t* nodo_siguiente = *((hash->tabla) + vect_pos);
    /*Inserto mi nuevo nodo en dicho lugar*/
    *((hash->tabla) + vect_pos) = nodo_nuevo;
    /*Apunto "ref"" de mi nuevo nodo a nodo_siguiente*/
    nodo_nuevo->ref = nodo_siguiente;
    /*Aumento la cnatidad en un*/
    hash->cantidad +=1;
    return true;
}//hash_guardar


size_t hash_cantidad(const hash_t *hash)
{
    return hash->cantidad;
}//hash_cantidad


bool hash_pertenece(const hash_t *hash, const char *clave)
{
   //Obtengo la posicion del nodo 
   size_t vect_pos = _fhash(clave, hash->tamanio);
   
   //Voy a dicha posicion y recorro los nodos
   if (!((hash->tabla) + vect_pos)) return false;
   nodo_hash_t* nodo = ((hash->tabla) + vect_pos);
   while (nodo->ref)
   {
       if (*(nodo->clave) == *clave) return true;
       //Avanzo un nodo
       nodo = nodo->ref;

   }//while

   //No la encontre, pucha!
   return false;

}//hash_pertenece

void *hash_obtener(const hash_t *hash, const char *clave)
{
    //Es casi igual a hash_pertenece
    
   //Obtengo la posicion del nodo 
   size_t vect_pos = _fhash(clave, hash->tamanio);
   
   //Voy a dicha posicion y recorro los nodos
   if (!((hash->tabla) + vect_pos)) return NULL;
   nodo_hash_t* nodo = ((hash->tabla) + vect_pos);
   while (nodo->ref)
   {
       if (*(nodo->clave) == *clave) return nodo->valor;
       //Avanzo un nodo
       nodo = nodo->ref;

   }//while

   //No la encontre, pucha!
   return NULL;
}//*hash_obtener


void *hash_borrar(hash_t *hash, const char *clave)
{
    
    //Es casi igual a hash_pertenece
    
   //Obtengo la posicion del nodo 
   size_t vect_pos = _fhash(clave, hash->tamanio);
   
   //Voy a dicha posicion y recorro los nodos
   if (!((hash->tabla) + vect_pos)) return NULL;
   long indice = 0; //long por que anda a saber si hacen pruebas de volumen :P
   nodo_hash_t* nodo = ((hash->tabla) + vect_pos);
   nodo_hash_t* nodo_anterior = nodo;
   while (nodo->ref)
   {
       if (*(nodo->clave) == *clave)
       {
           void* valor = nodo->valor;
           if (indice == 0)
           {
               //Estoy en el primer lugar
               *((hash->tabla) + vect_pos) = nodo->ref;
           }//if
           nodo_anterior->ref = nodo->ref;
           free(nodo);
           hash->cantidad -= 1;           
           return valor;
       }
           
       //Avanzo un nodo
       nodo_anterior = nodo;
       nodo = nodo->ref;
       indice++;

   }//while

   //No la encontre, pucha!
   return NULL;
}//hash_borrar

/*--------------PRIVADAS---------------*/


//Funcion HASH ( ES PRIVADA)
size_t _fhash(const char* clave, size_t tam)
{
    size_t hash=0;
    for (int i=0; i<strlen(clave); i++)
    {
        hash += (size_t) atoi(&clave[i]);
    }
    return hash%tam;
}//_fhash

