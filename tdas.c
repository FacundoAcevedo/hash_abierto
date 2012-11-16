#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "tdas.h"


struct nodo{
    void* valor;
    struct nodo* ref;
};

struct lista{
	nodo_t* inicio;
    nodo_t* fin;
	size_t largo;
};


struct lista_iter{
	nodo_t* anterior;
	nodo_t* actual;
};

struct vector
{
    size_t tam;
    int* datos;
};




//FUNCION AUXILIAR: creacion de un nodo
//Recibe un valor. Devuelve el nodo.
 nodo_t* nodo_crear(void* valor){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if (nodo == NULL) return NULL;
    nodo->valor = valor;
    nodo->ref = NULL;
    return nodo;
}

/* ******************************************************************
 *                    FUNCIONES BASICAS DE LA LISTA
 * *****************************************************************/


lista_t* lista_crear(){
	lista_t* lista;
	lista = malloc(sizeof(lista_t));
	if (lista == NULL) return NULL;
	lista->inicio = NULL;
	lista->fin = NULL;
	lista->largo = 0;	
	return lista;
}
 
 
void lista_destruir(lista_t *lista, void destruir_dato(void *)){
	if (lista==NULL) return;
	while (!lista_esta_vacia(lista)){
		void *borrado = lista_borrar_primero(lista);
		if (destruir_dato!= NULL){
			destruir_dato(borrado);	
			}
		}
    free(lista);
 }


size_t lista_largo(const lista_t *lista){
	return lista->largo;
	}


bool lista_esta_vacia(const lista_t *lista){
	if ((int)lista_largo(lista)==0) return true;
	return false;
}


bool lista_insertar_primero(lista_t *lista, void *dato){
	
	nodo_t* nuevo_nodo = nodo_crear(dato);
    if (nuevo_nodo == NULL) return false;
    if (lista_esta_vacia(lista)){
        lista->inicio = nuevo_nodo;
		lista->fin = nuevo_nodo;
		}
    else{
	    nuevo_nodo->ref = lista->inicio;
        lista->inicio = nuevo_nodo;
		}
	lista->largo += 1;
	return true; 
}
	

bool lista_insertar_ultimo(lista_t *lista, void *dato){
	nodo_t* nuevo_nodo = nodo_crear(dato);
    if (nuevo_nodo == NULL) return false;
    else if (lista->largo == 0){
        lista->inicio = nuevo_nodo;
        lista->fin = nuevo_nodo;
		}
    else{
        (lista->fin)->ref = nuevo_nodo;
        lista->fin= nuevo_nodo;
		}
	lista->largo += 1;
	return true; 
 }


void *lista_ver_primero(const lista_t *lista){
	if (lista_esta_vacia(lista)) return NULL;
	return (lista->inicio)->valor;
	}


void *lista_borrar_primero(lista_t *lista){
    if (lista_esta_vacia(lista)) return NULL;

    nodo_t *nodo_a_borrar = lista->inicio;
    void *valor = nodo_a_borrar->valor;
    lista->inicio= (nodo_a_borrar)->ref;
    
    free(nodo_a_borrar);
    lista->largo -=1;
    if (lista->largo == 0) lista->fin = NULL;
    return valor;
    }


bool lista_insertar(lista_t *lista, lista_iter_t *iter, void *dato){
	/*if (!iter->anterior  && iter->actual){ // o sea si iter = posicion inicial*/
	if (!iter->anterior ){ // o sea si iter = posicion inicial
        //Estoy en el primer nodo
        lista_insertar_primero(lista, dato);
        //Actualizo el iterador
        iter->actual = lista->inicio;
        iter->anterior = NULL;
        return true;
        }//if
    nodo_t* nodo_nuevo = nodo_crear(dato); 
    if (nodo_nuevo == NULL) return false;
    (iter->anterior)->ref = nodo_nuevo;
    nodo_nuevo->ref = iter->actual;
    lista->largo +=1;
    //Actualizo el iterador
    iter->actual = nodo_nuevo;
    return true;
    }
        
	
void *lista_borrar(lista_t *lista, lista_iter_t *iter)
{
	if (lista_esta_vacia(lista)) return NULL;

	nodo_t* puntero_auxiliar = iter->actual;
	void* dato_auxiliar = puntero_auxiliar->valor;
	
	if (iter->actual) iter->actual = (iter->actual)->ref;
	else return NULL;

	// Caso particular: borrar el primer elemento de la lista
	if (!(iter->anterior))
		lista->inicio = (lista->inicio)->ref;
	
	else {
		(iter->anterior)->ref = iter->actual;
		// Caso particular: borra el ultimo elemento de la lista
		if (!(iter->actual)) lista->fin = iter->anterior;
	}
	
	free (puntero_auxiliar);
	lista->largo -= 1;
	
	if (lista->largo == 0) lista->fin = NULL;
	
	return dato_auxiliar;
}

/* ******************************************************************
 *                    FUNCIONES DEL ITERADOR
 * *****************************************************************/

lista_iter_t *lista_iter_crear(const lista_t *lista)
{
    /*if (lista_esta_vacia(lista)) return NULL;*/
    lista_iter_t* iter = malloc(sizeof(lista_iter_t));
    iter->anterior = NULL;
    iter->actual = lista->inicio;
    return iter;
}


bool lista_iter_avanzar(lista_iter_t *iter){
	if (!(iter->actual)) return false;
	/*if (!((iter->actual)->ref))*/
		/*return false;*/
	iter->anterior = iter->actual;
	iter->actual = (iter->actual)->ref;
	return true;
}


void *lista_iter_ver_actual(const lista_iter_t *iter){
	if (!iter->actual) return NULL;
	void* valor = (iter->actual)->valor;
	return valor;
}


bool lista_iter_al_final(const lista_iter_t *iter){
    if (!iter) return NULL;
	else if ((!(iter->actual) && iter->anterior)\
        || (!iter->actual && !iter->anterior))  return true;

	return false;
}


void lista_iter_destruir(lista_iter_t *iter){
    free(iter);

} 


/* ******************************************************************
 *                    FUNCIONES BASICAS DE LA COLA
 * *****************************************************************/


struct cola
{
    nodo_t* prim;
    nodo_t* ultimo;
    long int tamanio;
};


cola_t* cola_crear()
{
    cola_t* cola = malloc(sizeof(cola_t));
    if (cola == NULL) return NULL;
    nodo_t* nodo1 = NULL;
    nodo_t* nodo2 = NULL;
    cola->prim = nodo1;
    cola->ultimo= nodo2;
    cola->tamanio =0;
    return cola;
}


 void cola_destruir(cola_t *cola, void destruir_dato(void *)){
	if (cola==NULL) return;
	if (cola->prim == NULL){
		 free(cola);
		 return;
		}
	void* borrado;
	int i;
	for (i=0; i<cola->tamanio; i++){
		borrado = cola_desencolar(cola);
		if (destruir_dato!= NULL){
			destruir_dato(borrado);	
			}
		}
    free(cola);
}


bool cola_esta_vacia(const cola_t *cola)
{
    if (!cola) return false;
    else if(cola->tamanio == 0) return true;
    return false;
}


bool cola_encolar(cola_t *cola, void* valor)
{
	if (!cola) return false;
    //Construyo el nuevo nodo que quiero encolar. nuevo_nodo->valor = valor
    // y nuevo_nodo->referencia = NULL porque esta al final de la cola
    nodo_t* nuevo_nodo = nodo_crear(valor);
    
    //Si la cola estÃ¡ vacÃ­a, nuevo_nodo es el primer y el Ãºltimo nodo de la cola
    if (cola->tamanio == 0){
		cola->prim = nuevo_nodo;
		cola->ultimo = nuevo_nodo;
	}
    else {
		cola->ultimo->ref = nuevo_nodo;
		cola->ultimo = nuevo_nodo; 
		}
	cola->tamanio += 1;
	return true;
}


void* cola_ver_primero(const cola_t *cola)
{
    if (cola_esta_vacia(cola)==true) return NULL;
    void* primero = (cola->prim)->valor;
    return primero;
}


void* cola_desencolar(cola_t *cola)
{
    if (cola_esta_vacia(cola) == true) return NULL;
    nodo_t* nodo_aux = NULL;
    
    //Guardo el valor a desencolar
    void* desencolado = (cola->prim)->valor;
    // Cambio la referencia de prim. Ahora vale la referencia al segundo nodo.
    nodo_aux = cola->prim;

    cola->prim = (cola->prim)->ref;
    // Destruyo el nodo desencolado? Â¿Â¿??
    free(nodo_aux);

    // Disminuyo el tamanio
    cola->tamanio -= 1;
    // Devuelvo el valor del desencolado
    return desencolado;
}    



/* *****************************************************************
 *                    FUNCIONES DE LA PILA
 * *****************************************************************/

struct _pila 
{
    //apunto datos a NULL, para que no apunte a cualquier lado
    void** datos ;
    size_t tamanio;
    size_t cantidad;
};


pila_t* pila_crear()
{
    pila_t* pila = malloc(sizeof(pila_t));
    if (pila == NULL) return NULL;
    pila->tamanio = 0;
    pila->cantidad = 0;
    pila->datos = malloc(sizeof(void*));
    if (!pila->datos){
		free(pila);
		return NULL;
		}
    return pila;
}


void pila_destruir(pila_t *pila , void destruir_dato(void *))
{
    if (pila->datos != NULL) {
		while (!pila_esta_vacia(pila)){
			void* desapilado;
			desapilado = pila_desapilar(pila);
			if (destruir_dato)
				destruir_dato(desapilado);
		}
	}
	free(pila->datos);
    free(pila);
}


bool pila_esta_vacia(const pila_t *pila)
{
	//Verifico que fue creada.
	if (pila == NULL) return false;
    if (pila->cantidad == 0) return true;
    
    return false;
}


bool pila_apilar(pila_t *pila, void* valor )
{
    //es una pre-condicion, pero igual, verifico que la pila haya sido creada
    if (pila == NULL) return false;
    
    //le reasigno memoria a pila.dato si llegue paso un multiplo de 10 en cantidad 
    if ((pila->tamanio - pila->cantidad) == 0){
	    pila->datos = realloc(pila->datos, 10* sizeof(void*));
	    pila->tamanio += 10;
		}    
    //verifico que todo haya funcionado
    //~ if (pila->datos == NULL) return false;
    pila->cantidad += 1;
	*(pila->datos + pila->cantidad) = valor;
	return true;
}


void* pila_ver_tope(const pila_t *pila){    
    if (pila->cantidad > 0) 
		return pila->datos[pila->cantidad];
    return NULL;
}
   
    
void* pila_desapilar(pila_t *pila)
{
    if (pila_ver_tope(pila) == NULL) return NULL;
    // Guardo un puntero al valor que voy a desapilar
    void* desapilado;
    desapilado = pila_ver_tope(pila);
	if ((pila->tamanio - pila->cantidad) == 10){
		pila->tamanio -= 10;
		void** nuevo_datos;
		nuevo_datos= realloc(pila->datos, pila->tamanio*sizeof(void*));
		if (nuevo_datos == NULL) return NULL;
		pila->datos = nuevo_datos;
		}
    pila->cantidad -=1;
    // Devuelvo la referencia al valor desapilado
    return desapilado;
}


/*
 *Vector dinamico
 */


// Crea un vector de tamaño tam
// Post: vector es una vector vacío de tamaño tam
vector_t* vector_crear(size_t tam)
{
    vector_t* vector = malloc(sizeof(vector_t));
    if (vector == NULL) return NULL;

    vector->datos = malloc(tam * sizeof(int));
    if (vector->datos == NULL)
    {
        free(vector);
        return NULL;
    }
    vector->tam = tam;
    return vector;
}

// Destruye el vector
// Pre: el vector fue creado
// Post: se eliminaron todos los elementos del vector
void vector_destruir(vector_t *vector)
{
    free(vector->datos);
    free(vector);
}



// Cambia el tamaño del vector
// Pre: el vector fue creado
// Post: el vector cambió de tamaño a nuevo_tam y devuelve true
// o el vector queda intacto y devuelve false si no se pudo cambiar el tamaño
// a nuevo_tam
bool vector_redimensionar(vector_t *vector, size_t tam_nuevo)
{
    int* datos_nuevo = realloc(vector->datos, tam_nuevo * sizeof(int));
    // Cuando tam_nuevo es 0, es correcto que devuelva NULL
    // En toda otra situación significa que falló el realloc
    if (tam_nuevo > 0 && datos_nuevo == NULL)
    {
        return false;
    }
    vector->datos = datos_nuevo;
    vector->tam = tam_nuevo;
    return true;
}

// Almacena en valor el dato guardado en la posición pos del vector
// Pre: el vector fue creado
// Post: se almacenó en valor el dato en la posición pos. Devuelve false si la
// posición es inválida (fuera del rango del vector, que va de 0 a tamaño-1)
bool vector_obtener(vector_t *vector, size_t pos, int *valor)
{
    if (pos >= 0 && pos < (vector->tam)) //Verifico que sea una posicion valida.
    {
        int *p;
        p = (vector->datos + pos);
        *valor = *p;
    }


    return false;
}

// Almacena el valor en la posición pos
// Pre: el vector fue creado
// Post: se almacenó el valor en la posición pos. Devuelve false si la posición
// es inválida (fuera del rango del vector, que va de 0 a tamaño-1) y true si
// se guardó el valor con éxito.
bool vector_guardar(vector_t *vector, size_t pos, int valor)
{
    if (pos >= 0 &&  pos < (vector->tam))
        {
            int *p;
            p = (vector->datos + pos);
            *p = valor;
            return true;
        }

    return false;
}

// Devuelve el tamaño del vector
// Pre: el vector fue creado
size_t vector_obtener_tamanio(vector_t *vector)
{
    return vector->tam;
}

