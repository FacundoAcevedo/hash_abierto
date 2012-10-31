#include "hash.h"




/*HASH ABIERTO POR QUE ES MAS FACIL*/

/*Funciones privadas */
size_t _fhash(const char*, size_t);
nodo_hash_t* _nodo_hash_crear(const char *clave, void *dato);
float _factor_de_carga(hash_t *hash);
bool  _hash_redimensionar(hash_t *hash);
bool _hash_trasvasar(hash_t *hash_viejo, hash_t *hash_nuevo);

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
    nodo_t* anterior;
    nodo_t* actual;
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
    hash_iter_t *iter;
    iter->anterior = NULL;
    if (hash->cantidad ==0){
        iter->actual = NULL;
        return iter;
    }//iter
    iter->actual = _buscar_primer_nodo(hash);
    return iter;
}//hash_iter_crear


}//hash_iter_crear
bool hash_iter_avanzar(hash_iter_t *iter);
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
    if (!_hash_trasvasar(hash_viejo, hash_nuevo)) return false;
    return true;
}//_hash_redimensionar

//Funcion _hash_trasvasar
bool _hash_trasvasar(hash_t *hash_viejo, hash_t *hash_nuevo)
{
    if (!hash_viejo || !hash_nuevo) return false;
    //Verificado el asunto, me dispongo a hacer la magia

    //ESTO SE DEBERIA HACER CON ITERADORES... PRGRAMANDO ITERADORES
    return false;

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

