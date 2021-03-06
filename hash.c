#include "hash.h"


/*Funciones privadas */
long int  _fhash(const char*, long int );
nodo_hash_t* _nodo_hash_crear(char *clave, void *dato);
float _factor_de_carga(hash_t *hash);
bool  _hash_redimensionar(hash_t *hash);
long int  _buscar_lista(const hash_t* hash, long int inicio);
bool  _hash_reemplazar(hash_t* hash,char *clave, void* dato);
void**  _obtenerListaOcupante(const hash_t *hash,const  char* clave, long int vect_pos);
char *my_strdup(const char *str);

/*Struct de los nodos del hash*/
struct nodo_hash{
    char *clave;
    void *valor;
};


struct hash{
    size_t cantidad;
    long int tamanio;
    hash_destruir_dato_t destruir_dato;
    lista_t** tabla;
};

struct hash_iter{
    const hash_t *hash; 
    long int pos_vect;
    lista_iter_t *iter_lista;
};


/*----------------------------------------------------------*/
hash_t* hash_crear(hash_destruir_dato_t destruir_dato)
{
    hash_t* hash = malloc(sizeof(hash_t));
    if (!hash) return NULL;

    /*Reservo el espacio que necesito para los nodos*/
    lista_t** tabla = calloc(TAM_INICIAL, sizeof(lista_t*));
    if (!tabla)
    {
        free(hash);
        return NULL;
    }
    for (unsigned int i = 0; i < TAM_INICIAL; i++){
          tabla[i]=lista_crear();        
    } 
    hash->tabla = tabla;
    hash->destruir_dato = destruir_dato;
    hash->cantidad = 0;
    hash->tamanio = (long int) TAM_INICIAL;;
    return hash;
}//hash_crear

bool hash_guardar(hash_t *hash, const char *clave, void *dato)
{


    if (!clave) return NULL;
    if (!hash) return NULL;//compurebo la existencia del hash
    //Reviso si hay que redimensionar
    if (_factor_de_carga(hash) >= (float) 0.7){
        if (!_hash_redimensionar(hash)) return NULL;
    }//if
    //En este momento estoy seguro de que tengo un hash. 

    long int vect_pos = _fhash(clave, hash->tamanio);

    /*Verifico que haya una lista en la posicion del vector*/
    if (!hash->tabla[vect_pos])
    {
        (hash->tabla[vect_pos]) = lista_crear();
    }
    /*Copio la clave por si me la modifican desde afuer   */
    char *clave_copia =my_strdup(clave);

    //Verifico si ya existe algo con esa clave
    if (hash_pertenece(hash,clave))
    {
        if (_hash_reemplazar(hash,clave_copia,dato)){
            /*Le doy free por la clave esta duplicada, 
             * y no se va a volver a guardar*/
            free(clave_copia);
            return true;
        }
        free(clave_copia);
        return false;
    }//if

    /*Genero el nodo*/
    nodo_hash_t *nodo_nuevo = _nodo_hash_crear(clave_copia, dato);
    if (!nodo_nuevo) return NULL;

    /*Inserto mi nodo*/
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
   long int vect_pos = _fhash(clave, hash->tamanio);
   void** vector_lnp =  _obtenerListaOcupante(hash, clave, vect_pos);
   if (!vector_lnp) return false;
   free(vector_lnp);
   return true;

}//hash_pertenece

void *hash_obtener(const hash_t *hash, const char *clave)
{
   //Obtengo la posicion del nodo
   long int vect_pos = _fhash(clave, hash->tamanio);
   void** vector_lnp =  _obtenerListaOcupante(hash, clave, vect_pos);
   if (!vector_lnp) return NULL;
   //obtengo el nodo
   //
   // posiciones:
   //   0->dir lista 1-> nodo 2-> pos en lista
   //
   nodo_hash_t* nodo = (nodo_hash_t*) (vector_lnp[1]);

   //libero el vector recibido
   free(vector_lnp);

   //devuelvo lo obtenido
   return (nodo)->valor;
}//*hash_obtener


void *hash_borrar(hash_t *hash, const char *clave)
{
   long int vect_pos = _fhash(clave, hash->tamanio);
   void** vector_lnp =  _obtenerListaOcupante(hash, clave, vect_pos);
   if (!vector_lnp) return NULL;

   lista_t* lista = (lista_t*) (vector_lnp[0]);
   nodo_hash_t* nodo = (nodo_hash_t*) (vector_lnp[1]);
   size_t pos_nodo = *(size_t*) (vector_lnp[2]);
   lista_iter_t* iter = lista_iter_crear(lista);
   //saco el valor
   void* valor = nodo->valor;
   
   //Avanzo la hasta llegar al nodo
   for (int i=0; i< pos_nodo; i++)
       lista_iter_avanzar(iter);

    free(nodo->clave);
    //quito el nodo de la lista
    free((nodo_hash_t*) lista_borrar(lista, iter));
    //libero memoria
    lista_iter_destruir(iter);
    free(vector_lnp);

    hash->cantidad -=1;

    return valor;
}//hash_borrar

void hash_destruir(hash_t *hash)
{
    long int largo_hash = hash->tamanio;

    //Recorro toda la tabla del hash
    for (long int  i =0; i < largo_hash ; i++)
        if (hash->tabla[i])
        {
            /*Borro los nodo_hash_t*/
            lista_iter_t *iter = lista_iter_crear(hash->tabla[i]);
            while (!lista_iter_al_final(iter))
            {
                nodo_hash_t* nodo_hash = lista_iter_ver_actual(iter);
                free(nodo_hash->clave);
                if(hash->destruir_dato) hash->destruir_dato(nodo_hash->valor);
                free(nodo_hash);
                lista_iter_avanzar(iter);
            }//while
            lista_iter_destruir(iter);
            lista_destruir(hash->tabla[i], NULL);
        }//if
    free(hash->tabla);
    free(hash);



}//hash_destruir

hash_iter_t *hash_iter_crear(const hash_t *hash)
{
    if (!hash) return NULL;
    hash_iter_t *iter = malloc(sizeof(hash_iter_t));
    iter->hash = hash;
    if (hash->cantidad ==0){
        iter->iter_lista = NULL;
        iter->pos_vect=-1;
        return iter;
    }//iter
    /*Busco la primer lista*/
    long int pos = _buscar_lista(hash,0);
    if (pos !=  -1)
    {
        //Me paro en el primer nodo
        iter->iter_lista = lista_iter_crear(hash->tabla[pos]);
        if (!iter->iter_lista){
            free(iter);
            return NULL;
        }//if
        iter->pos_vect = pos;
        return iter;
    }
    iter->iter_lista = NULL; 
    return iter;
}//hash_iter_crear


bool hash_iter_avanzar(hash_iter_t *iter)
{
    if (hash_cantidad(iter->hash) <=0) return false;
    //si estoy al final de una lista
    lista_iter_avanzar(iter->iter_lista);
    if (lista_iter_al_final(iter->iter_lista))
    {
        //busco la siguiente lista
        long int pos = _buscar_lista(iter->hash,(long int) (iter->pos_vect +1));
        if (pos == -1) return false; // no hay mas listas campeon
        // hay otra lista, entonces destruyo el iterador viejo y le doy paso 
        // a la juventud
        iter->pos_vect  = pos;
        lista_iter_destruir(iter->iter_lista);
        iter->iter_lista = lista_iter_crear(iter->hash->tabla[pos]);
        return true;
    } //if
    
    //si estoy en medio de la lista
    return true;
}//hash_iter_avanzar

const char *hash_iter_ver_actual(const hash_iter_t *iter)
{
    if (iter->hash->cantidad == 0) return NULL;
    else if (! lista_iter_ver_actual(iter->iter_lista)) return NULL;
    return ((char*) ((nodo_hash_t*) lista_iter_ver_actual(iter->iter_lista))->clave );

}//hash_iter_ver_actual

bool hash_iter_al_final(const hash_iter_t *iter)
{
    if (hash_cantidad(iter->hash) <=0) return true;
    else if (lista_iter_al_final(iter->iter_lista)) return true;
    return false;

}//hash_iter_al_final
void hash_iter_destruir(hash_iter_t* iter)
{
    lista_iter_destruir(iter->iter_lista);
    free(iter);
}//hash_iter_destruir


/*--------------PRIVADAS---------------*/

//Funcion nodo_hash_crear (PRIVADA)
nodo_hash_t* _nodo_hash_crear(char *clave, void *dato){
    nodo_hash_t* nodo_hash = NULL;
    if (! (nodo_hash = malloc(sizeof(nodo_hash_t)))) return NULL;
    nodo_hash->clave = clave;
    nodo_hash->valor = dato;

    return nodo_hash;
}//_nodo_hash_crear

//Funcion _buscar_primer_nodo
long int _buscar_lista(const hash_t* hash,long int inicio)
{
    long int i;
    if( inicio > hash->tamanio) return -1;
    for (i=inicio; i<hash->tamanio; i++)
    {
        if (hash->tabla[i] && lista_largo(((lista_t*) hash->tabla[i]))!=0) return i;
    }
    return -1; //Devuelvo -1 por que si es 0 se confunde con null
}//_buscar_primer_lista

//Funcion _factor_de_carga
float  _factor_de_carga(hash_t *hash){
    //Devuelve el factor de carga
    return ((float) (hash->cantidad / hash->tamanio));
}//_factor_de_carga
    

//Funcion _hash_redimensionar
bool _hash_redimensionar(hash_t *hash)
{
    long int nuevoTam = hash->tamanio*5;
    lista_t** vectorNuevo =  calloc(nuevoTam, sizeof(lista_t*));
    if (!vectorNuevo) return false;

    //creo las sublistas
    for (unsigned int i = 0; i < nuevoTam; i++){
          vectorNuevo[i]=lista_crear();        
    } 
      
    //voy sacando los nodos del vector original, y reubicandolos en el nuevo:
    for (unsigned int i = 0; i < hash->tamanio; i++){
          while ( !lista_esta_vacia(hash->tabla[i])){
            nodo_hash_t* nodo = lista_borrar_primero(hash->tabla[i]);
            int pos = _fhash(nodo->clave, nuevoTam);
            lista_insertar_ultimo(vectorNuevo[pos], nodo);
          }//whie
          /*Destruyo las listas*/
          lista_destruir(hash->tabla[i], NULL);
    }//for
    
     //liberamos el vector viejo:
     free(hash->tabla);
     hash->tabla = vectorNuevo;
     hash->tamanio = nuevoTam;
     return true;
    
}//_hash_redimensionar

bool _hash_reemplazar(hash_t* hash,char* clave, void* dato)
{
   //Obtengo la posicion del nodo 
   long int vect_pos = _fhash(clave, hash->tamanio);
   
   void** vector_lnp =  _obtenerListaOcupante(hash, clave, vect_pos);
   nodo_hash_t* nodo = vector_lnp[1];
   if (hash->destruir_dato)
       hash->destruir_dato(nodo->valor);

    nodo->valor = dato;
    free(vector_lnp);
    return true;
}//_hash_reemplazar

void** _obtenerListaOcupante(const hash_t *hash,const  char* clave, long int vect_pos)
{
   //Devuelvo un vector donde en 0 tiene la direccion de la lista,
   // en 1 el nodo y en 2  la pocicion ( dentro de la liasta) 
   // donde esta el nodo
   
   //Voy a dicha posicion y recorro los nodos
   if (!hash->tabla[vect_pos]) return NULL;

   //Creo lo que nececito
   nodo_hash_t* nodo_actual = NULL;
   lista_t* lista = hash->tabla[vect_pos];
   lista_iter_t* iter = lista_iter_crear(lista);
   
   //Verifico que existan
   if (!lista || !iter) return NULL;
   void** listaynodo = malloc(sizeof(void*)*3);

   size_t indice = 0;
   size_t *pindice; //no se cual es el maximo de un size_t
   //pero no creo que la lista se llene tanto como para que explote
   //si pasa, revisar ACA.

   //Recorro la lista buscando la clave
   while(lista_iter_ver_actual(iter))
   {
      nodo_actual = lista_iter_ver_actual(iter);
      //Verifico si esta
      if ( !strcmp( nodo_actual->clave, clave) && nodo_actual){
          lista_iter_destruir(iter);
          pindice=&indice;
          listaynodo[0] = lista;
          listaynodo[1] = nodo_actual;
          listaynodo[2] = pindice;
          
          //Hay que destruirlo!
          return listaynodo;
          
          /*return lista; */
      }//if

      //No esta, entonces avanzo una pocicion
      lista_iter_avanzar(iter);
      indice++;

   }
   lista_iter_destruir(iter);
   free(listaynodo);

   //No la encontre, pucha!
   return NULL;

}//_obtenerListaOcupante

//Funcion HASH (PRIVADA)
long int  _fhash(const char* clave, long int  tam)
{
    long int hash=0;
    long int i = 0;
    while (clave[i] != '\0')
    {
        hash += ((long int) clave[i]+13);
        i++;
    }
    return hash%tam;
}//_fhash

//funcion mi_strdup
char *my_strdup(const char *str) {
    return strcpy(malloc( strlen(str) + 1),str);
}//my_strdup

