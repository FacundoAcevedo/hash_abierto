#include "hash.h"




/*HASH ABIERTO POR QUE ES MAS FACIL*/

/*Funciones privadas */
size_t _fhash(const char*, size_t);
nodo_hash_t* _nodo_hash_crear(const char *clave, void *dato);

/*Struct de los nodos del hash*/
struct nodo_hash{
    char *clave;
    void *valor;
};


struct hash{
    size_t cantidad;
    size_t tamanio;
    hash_destruir_dato_t destruir_dato;
    lista_t** tabla;
};

hash_t* hash_crear(hash_destruir_dato_t destruir_dato)
{
    hash_t* hash = malloc(sizeof(hash_t));
    if (!hash) return NULL;
    /*Reservo el espacio que necesito para los nodos*/
    if (!(hash->tabla = calloc(TAM_INI, sizeof(lista_t*))))//Quizas sea nodo_hash_t*
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

    /*Verifico que haya una lista en la posicion del vector*/
    if (!((hash->tabla) + vect_pos))
    {
        *(hash->tabla + vect_pos) = lista_crear();
    }

    /*Genero el nodo*/
    nodo_hash_t *nodo_nuevo = _nodo_hash_crear(clave, dato);
    if (!nodo_nuevo) return NULL;


    /*Verifico si hay algo en la pos donde quiero guardar*/
   
    /*Changos, parece que el lugar esta vacio*/
    /**((hash->tabla) + vect_pos) = nodo_nuevo;*/
    lista_insertar_primero(*((hash->tabla) + vect_pos), nodo_nuevo);
    hash->cantidad +=1;
    return true;
   
    /*[>Aca hay un nodo, asi que lo inserto en primer lugar, y lo apunto al siguiente<]*/
    /*[>Obtengo la direccion del primer nodo<]*/
    /*nodo_hash_t* nodo_siguiente = *((hash->tabla) + vect_pos);*/
    /*[>Inserto mi nuevo nodo en dicho lugar<]*/
    /**((hash->tabla) + vect_pos) = nodo_nuevo;*/
    /*[>Apunto "ref"" de mi nuevo nodo a nodo_siguiente<]*/
    /*nodo_nuevo->ref = nodo_siguiente;*/
    /*[>Aumento la cnatidad en un<]*/
    /*hash->cantidad +=1;*/
    /*return true;*/
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

   //Creo lo que nececito
   nodo_hash_t* nodo_actual = NULL;
   lista_t* lista = *((hash->tabla) + vect_pos);
   lista_iter_t* iter = lista_iter_crear(lista);
   
   //Verifico que existan
   if (!lista || !iter) return NULL;
   //Recorro la lista buscando la clave
   do
   {
      nodo_actual = lista_iter_ver_actual(iter);
      //Verifico si esta
      if ( strcmp( nodo_actual->clave, clave) == 0){
          free(iter);
          return true; 
      }
      //No esta, entonces avanzo una pocicion
      lista_iter_avanzar(iter);

   }while(lista_iter_ver_actual(iter));
   free(iter);

   //No la encontre, pucha!
   return false;

}//hash_pertenece

void *hash_obtener(const hash_t *hash, const char *clave)
{
}//*hash_obtener


void *hash_borrar(hash_t *hash, const char *clave)
{
}//hash_borrar


/*--------------PRIVADAS---------------*/

//Funcion nodo_hash_crear (PRIVADA)
nodo_hash_t* _nodo_hash_crear(const char *clave, void *dato){
    nodo_hash_t* nodo_hash = NULL;
    if (! (nodo_hash = malloc(sizeof(nodo_hash_t)))) return NULL;
    nodo_hash->clave = clave;
    nodo_hash->valor = dato;

    return nodo_hash;
}//_nodo_hash_crear

//Funcion HASH (PRIVADA)
size_t _fhash(const char* clave, size_t tam)
{
    size_t hash=0;
    for (int i=0; i<strlen(clave); i++)
    {
        hash += (size_t) atoi(&clave[i]);
    }
    return hash%tam;
}//_fhash

