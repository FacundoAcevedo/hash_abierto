#include "hash.h"




/*HASH ABIERTO POR QUE ES MAS FACIL*/

/*Funciones privadas */
size_t _fhash(const char*, size_t);

/*Struct de los nodos del hash*/
struct nodo_hash{
    char *clave;
    void *valor;
    struct nodo* ref;
};


struct hash{
    size_t cantidad;
    size_t tamanio;
    hash_destruir_dato_t destruir_dato;
    /*lista_t** tabla;*/
    vector_t* tabla;
};

hash_t* hash_crear(hash_destruir_dato_t destruir_dato)
{
    hash_t* hash = malloc(sizeof(hash_t));
    if (!hash) return NULL;
    /*if (!(hash->tabla = calloc(TAM_INI, sizeof(nodo_hash_t*))))*/
    /*{*/
        /*free(hash);*/
        /*return NULL;*/
    /*}*/
    hash->tabla = vector_crear(TAM_INI); //Creo la tabla
    if (!hash->tabla) return NULL;
    hash->destruir_dato = destruir_dato;
    hash->cantidad = 0;
    hash->tamanio = (size_t) TAM_INI;;
    return hash;
}//hash_crear

bool hash_guardar(hash_t *hash, const char *clave, void *dato)
{
    if (!hash) return NULL;//compurebo la existencia del hash
    //En este momento estoy seguro de que tengo un hash, que apunta a un
    //vector dinamico.
    //Verifico que el vector apunte a algo.
    size_t vect_pos = _fhash(clave, hash->tamanio);
    if (!(hash->tabla)[vect_pos])
    {
            (hash->tabla)[vect_pos] = lista_crear();
    }



}//hash_guardar

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

