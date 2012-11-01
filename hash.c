#include "hash.h"




/*HASH ABIERTO POR QUE ES MAS FACIL*/

/*Funciones privadas */
size_t _fhash(const char*, size_t);
nodo_hash_t* _nodo_hash_crear(const char *clave, void *dato);
float _factor_de_carga(hash_t *hash);
bool  _hash_redimensionar(hash_t *hash);
bool _hash_trasvasar(hash_t *hash_viejo, hash_t *hash_nuevo);
long int  _buscar_lista(const hash_t* hash, long int inicio);

/*Struct de los nodos del hash*/
struct nodo_hash{
    const char *clave;
    void *valor;
};


struct hash{
    size_t cantidad;
    size_t tamanio;
    hash_destruir_dato_t destruir_dato;
    lista_t** tabla;
};

struct hash_iter{
    /*nodo_t* anterior;*/
    /*nodo_t* actual;*/
    const hash_t *hash; //FTW!
    size_t pos_vect;
    lista_iter_t *iter_lista;
};


/*----------------------------------------------------------*/
hash_t* hash_crear(hash_destruir_dato_t destruir_dato)
{
    hash_t* hash = malloc(sizeof(hash_t));
    if (!hash) return NULL;
    /*Reservo el espacio que necesito para los nodos*/
    /*if (!(hash->tabla = (lista_t*) calloc(TAM_INI, sizeof(lista_t*))))//Quizas sea nodo_hash_t**/

    void* tabla;
    tabla = (lista_t*) calloc(TAM_INI, sizeof(lista_t*));
    if (!tabla)
    {
        free(hash);
        return NULL;
    }
    hash->tabla = tabla;
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
     *    Ver cuando redimensionar
     *    ...
     *XXXXXXXXXXXXXXXXXXXXXXXXXXX
     */



    if (!hash) return NULL;//compurebo la existencia del hash
    //Reviso si hay que redimensionar
    if (_factor_de_carga(hash) >= 0.7){
        if (!_hash_redimensionar(hash)) return NULL;
    }//if
    //En este momento estoy seguro de que tengo un hash. 
    size_t vect_pos = _fhash(clave, hash->tamanio);

    /*Verifico que haya una lista en la posicion del vector*/
    /*if (!((hash->tabla) + vect_pos))*/
    if (!hash->tabla[vect_pos])
    {
        /**(hash->tabla + vect_pos) = lista_crear();*/
        (hash->tabla[vect_pos]) = lista_crear();
    }

    /*Genero el nodo*/
    nodo_hash_t *nodo_nuevo = _nodo_hash_crear(clave, dato);
    if (!nodo_nuevo) return NULL;


   
    /*Inserto mi nodo*/
    /*lista_insertar_primero(*((hash->tabla) + vect_pos), nodo_nuevo);*/
    lista_insertar_primero(hash->tabla[vect_pos], nodo_nuevo);
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
   /*if (!((hash->tabla) + vect_pos)) return false;*/
   if (!hash->tabla[vect_pos]) return false;

   //Creo lo que nececito
   nodo_hash_t* nodo_actual = NULL;
   /*lista_t* lista = *((hash->tabla) + vect_pos);*/
   lista_t* lista = hash->tabla[vect_pos];
   lista_iter_t* iter = lista_iter_crear(lista);
   
   //Verifico que existan
   if (!lista || !iter) return NULL;
   //Recorro la lista buscando la clave
   do
   {
      nodo_actual = lista_iter_ver_actual(iter);
      //Verifico si esta
      if ( strcmp( nodo_actual->clave, clave) == 0){
          lista_iter_destruir(iter);
          return true; 
      }//if

      //No esta, entonces avanzo una pocicion
      lista_iter_avanzar(iter);

   }while(lista_iter_ver_actual(iter));
   lista_iter_destruir(iter);

   //No la encontre, pucha!
   return false;

}//hash_pertenece

void *hash_obtener(const hash_t *hash, const char *clave)
{
    /*EAEA se parece a hash_pertenece... asi que C&P y toqueteo*/
    /*PD: estos comentarios nos vas a hacer desaprobar...*/

   void *dato = NULL;

   //Obtengo la posicion del nodo 
   size_t vect_pos = _fhash(clave, hash->tamanio);
   
   //Voy a dicha posicion y recorro los nodos
   /*if (!((hash->tabla) + vect_pos)) return false;*/
   if (!hash->tabla[vect_pos]) return NULL;

   //Creo lo que nececito
   nodo_hash_t* nodo_actual = NULL;
   /*lista_t* lista = *((hash->tabla) + vect_pos);*/
   lista_t* lista = hash->tabla[vect_pos];
   lista_iter_t* iter = lista_iter_crear(lista);
   
   //Verifico que existan
   if (!lista || !iter) return NULL;
   //Recorro la lista buscando la clave
   do
   {
      nodo_actual = lista_iter_ver_actual(iter);
      //Verifico si esta
      if ( strcmp( nodo_actual->clave, clave) == 0){
          dato = lista_iter_ver_actual(iter);
          lista_iter_destruir(iter);
          return dato; 
      }//if

      //No esta, entonces avanzo una pocicion
      lista_iter_avanzar(iter);

   }while(lista_iter_ver_actual(iter));
   lista_iter_destruir(iter);

   //No la encontre, pucha!
   return NULL;

}//*hash_obtener


void *hash_borrar(hash_t *hash, const char *clave)
{
    //... C&P de hash_obtener y toqueteo un poco
   //Obtengo la posicion del nodo 
   size_t vect_pos = _fhash(clave, hash->tamanio);
   
   //Voy a dicha posicion y recorro los nodos
   /*if (!((hash->tabla) + vect_pos)) return false;*/
   if (!hash->tabla[vect_pos]) return NULL;

   //Creo lo que nececito
   nodo_hash_t* nodo_actual = NULL;
   /*lista_t* lista = *((hash->tabla) + vect_pos);*/
   lista_t* lista = hash->tabla[vect_pos];
   lista_iter_t* iter = lista_iter_crear(lista);
   
   //Verifico que existan
   if (!lista || !iter) return NULL;
   //Recorro la lista buscando la clave
   do
   {
      nodo_actual = lista_iter_ver_actual(iter);
      //Verifico si esta
      if ( strcmp( nodo_actual->clave, clave) == 0){
          lista_iter_destruir(iter);
          return lista_borrar(lista, iter);
      }//if

      //No esta, entonces avanzo una pocicion
      lista_iter_avanzar(iter);

   }while(lista_iter_ver_actual(iter));
   lista_iter_destruir(iter);

   //No la encontre, pucha!
   return false;
}//hash_borrar


hash_iter_t *hash_iter_crear(const hash_t *hash)
{
    if (!hash) return NULL;
    hash_iter_t *iter = malloc(sizeof(hash_iter_t));
    iter->hash = hash;
    /*iter->anterior = NULL;*/
    if (hash->cantidad ==0){
        /*iter->actual = NULL;*/
        iter->iter_lista = NULL;
        return iter;
    }//iter
    /*Busco la primer lista*/
    size_t pos = _buscar_lista(hash,0);
    if (pos !=  -1)
    {
        //Me paro en el primer nodo SIII!!!!!
        iter->iter_lista = lista_iter_crear(hash->tabla[pos]);
        iter->pos_vect = pos;
        return iter;
    }
    iter->iter_lista = NULL; 
    return iter;
}//hash_iter_crear


bool hash_iter_avanzar(hash_iter_t *iter)
{
    //si estoy al final de una lista
    if (lista_iter_al_final(iter->iter_lista))
    {
        //busco la siguiente lista
        long int pos = _buscar_lista(iter->hash,(long int) (iter->pos_vect +1));
        if (pos == -1) return false; // no hay mas listas campeon
        // hay otra lista, entonces destruyo el iterador viejo y le doy paso 
        // a la juventud
        lista_iter_destruir(iter->iter_lista);
        iter->iter_lista = lista_iter_crear(iter->hash->tabla[pos]);
        return true;
    } //if
    
    //si estoy en medio de la lista
    lista_iter_avanzar(iter->iter_lista);
    return true;
}//hash_iter_avanzar

const char *hash_iter_ver_actual(const hash_iter_t *iter);
bool hash_iter_al_final(const hash_iter_t *iter);
void hash_iter_destruir(hash_iter_t* iter);


/*--------------PRIVADAS---------------*/

//Funcion nodo_hash_crear (PRIVADA)
nodo_hash_t* _nodo_hash_crear(const char *clave, void *dato){
    nodo_hash_t* nodo_hash = NULL;
    if (! (nodo_hash = malloc(sizeof(nodo_hash_t)))) return NULL;
    nodo_hash->clave = clave;
    nodo_hash->valor = dato;

    return nodo_hash;
}//_nodo_hash_crear

//Funcion _buscar_primer_nodo
long int _buscar_lista(const hash_t* hash,long int inicio)
{
    for (long int i=inicio; i<hash->tamanio; i++)
    {
        if (hash->tabla[i]) return i;
    }
    return -1; //Devuelvo -1 por que si es 0 se confunde con null
}//_buscar_primer_lista

//Funcion _factor_de_carga
float  _factor_de_carga(hash_t *hash){
    //Devuelve el factor de carga
    return (float) (hash->cantidad / hash->tamanio);
}//_factor_de_carga
    

//Funcion _hash_redimensionar
bool _hash_redimensionar(hash_t *hash)
{
    hash_t* hash_viejo = hash;
    hash_t* hash_nuevo = hash_crear(hash->destruir_dato);
    //llevo el factor de carga a 0.2
    size_t nuevo_tam = hash->tamanio;
    //voy aumentando el tamanio sumandole CONST_REDIM
    while ((float) (hash->cantidad / nuevo_tam) > 0.2)  nuevo_tam +=CONST_REDIM;
    //Ya tengo mi nuevo tamanio
    hash_nuevo->tamanio = nuevo_tam;
    //trasvaso el hash_viejo en el hash_nuevo
    if (!_hash_trasvasar(hash_viejo, hash_nuevo)) 
    {
       free(hash_nuevo);
       return false;
    }//if
    
    //trasvasamiento OK
    return true;

}//_hash_redimensionar

//Funcion _hash_trasvasar
bool _hash_trasvasar(hash_t *hash_viejo, hash_t *hash_nuevo)
{
    if (!hash_viejo || !hash_nuevo) return false;
    //Verificado el asunto, me dispongo a hacer la magia
    //Creo un iterador sobre el viejo hash
    hash_iter_t *iter_hash_viejo = hash_iter_crear(hash_viejo);
    //Mi iter_hash esta parado en el primer nodo 

    //Si no hay nodo
    if (hash_viejo->cantidad == 0) return false; //Esto nunca deberia llegar a pasar
    long int indice = 0;
    const char *clave = NULL;
    void *dato = NULL;
    nodo_hash_t *nodo_hash = NULL;
    nodo_t *nodo_lista = NULL;
    lista_iter_t *iter_lista = NULL;
    
    // Ahora sabiendo que hay nodos, comienzo a travasar
    while ( indice < hash_viejo->cantidad)
    {
        //Obtengo el iterador de las listas
        iter_lista = iter_hash_viejo->iter_lista;
        nodo_lista = iter_lista->actual;
        nodo_hash = nodo_lista->valor;
        //Obtengo la clave
        //clave = iter_hash_viejo->iter_lista->actual->valor->clave;
        clave = nodo_hash->clave;
        //Obtengo el dato
        //dato  = iter_hash_viejo->iter_lista->actual->valor->valor;
        dato  = nodo_hash->valor;
        //Lo guardo en mi nuevo hash
        hash_guardar(hash_nuevo, clave, dato);
        //Avanzo el iterador
        hash_iter_avanzar(iter_hash_viejo);
        //Le sumo uno al indice
        indice +=1;
    }//while

    //Comprobacion del final
    if (hash_viejo->cantidad != hash_nuevo->cantidad) return false;
    //Viendo que esta todo mas que en orden, procedo a liberar al hash viejo y su 
    //iterador y a retornar true
    free(hash_viejo);
    hash_iter_destruir(iter_hash_viejo);
    return true;

}//_hash_trasvasar

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

