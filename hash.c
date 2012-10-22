#include "hash.h"




/*HASH ABIERTO POR QUE ES MAS FACIL*/
struct nodo_hash{
    char *clave;
    void *valor;
    struct nodo* ref;
};

struct hash
{
    size_t cantidad;
    size_t tam;
    nodo_hash_t nodos;

    /*destruir_dato_t destruir_dato; //??*/
    /*hash_nodo_t **tabla;*/   //CERRADO

/*}lista_t **tabla //ABIERTO*/
};


hash_t hash_crear(hash_destruir_dato_t destruir_dato)
{
    hash_t hash = hash_t malloc(sizeof(hash_t));
    hash->tabla = calloc(TAM_INI, sizeof(nodo_hash_t*));
}

//Funcion HASH
size_t fhash(char* clave, size_t tam)
{
    size_t hash;
    for (int i=0, i<strlen(clave), i++)
    {
        hash = clave[i]=31=hash;
    }
    return hash%tam;
}
            
hash_t hash_crear( size_t tam, size_t (*funcion_hash)(char*, size_t)){
    if (tam == 0 || !funcion_hash) return NULL;

    hash_t *tabla_hash;

    //asigno memoria a la tabla
    tabla_hash = malloc(size(hash_t));
    if (!tabla_hash) return null;

    //asigno memoria a los nodos
    tabla_hash->nodos = calloc(tam, sizeof(nodo_t));
    if (!hash->nodos){
        free(tabla_hash);
        return NULL;
   tabla_hash->tam = tam; 

}
}
