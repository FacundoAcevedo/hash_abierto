#ifndef TDAS_H

/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct _pila pila_t;
typedef struct nodo nodo_t;
typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;
typedef struct cola cola_t;
typedef struct vector vector_t;



/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/
// Crea una pila.
// Post: devuelve una nueva pila vacia.
pila_t* pila_crear();


// Destruye la pila.
// Pre: la pila fue creada.
// Post: se eliminaron todos los elementos de la pila.
void pila_destruir(pila_t *pila , void destruir_dato(void *));


// Devuelve verdadero o falso, seg????????n si la pila tiene o no elementos apilados.
// Pre: la pila fue creada.
bool pila_esta_vacia(const pila_t *pila);


// Agrega un nuevo elemento a la pila. Devuelve falso en caso de error.
// Pre: la pila fue creada.
// Post: se agreg???????? un nuevo elemento a la pila, valor es el nuevo tope.
bool pila_apilar(pila_t *pila, void* valor );


// Obtiene el valor del tope de la pila. Si la pila tiene elementos,
// se devuelve el valor del tope. Si esta vacia devuelve NULL.
// Pre: la pila fue creada.
// Post: se devuelve el valor del tope de la pila, cuando la pila no esta
// vacia, NULL en caso contrario.
void* pila_ver_tope(const pila_t *pila);
   
    
// Saca el elemento tope de la pila. Si la pila tiene elementos, se quita el
// tope de la pila, y se devuelve ese valor. Si la pila esta vacia, devuelve 
// NULL.
// Pre: la pila fue creada.
// Post: si la pila no estaba vacia, se devuelve el valor del tope anterior 
// y la pila contiene un elemento menos.
void* pila_desapilar(pila_t *pila);


/* *****************************************************************
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/

// Crea una cola.
// Post: devuelve una cola nueva vacia.
cola_t* cola_crear();
 
 
// Destruye la cola. Si se recibe la funcion destruir_dato por parametro,
// para cada uno de los elementos de la cola llama a destruir_dato.
// Pre: la cola fue creada. destruir_dato es una funcion capaz de destruir
// los datos de la cola, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la cola.
void cola_destruir(cola_t *cola, void destruir_dato(void *));

// Devuelve verdadero o falso, segun si la cola tiene o no elementos encolados.
// Pre: la cola fue creada.
bool cola_esta_vacia(const cola_t *cola);


// Agrega un nuevo elemento a la cola. Devuelve falso en caso de error.
// Pre: la cola fue creada.
// Post: se agrega un nuevo elemento a la cola, valor se encuentra al final
// de la cola.
bool cola_encolar(cola_t *cola, void* valor);


// Obtiene el valor del primer elemento de la cola. Si la cola tiene
// elementos, se devuelve el valor del primero, si esta vacia devuelve NULL.
// Pre: la cola fue creada.
// Post: se devuelve el primer elemento de la cola, cuando no esta vacia.
void* cola_ver_primero(const cola_t *cola);


// Saca el primer elemento de la cola. Si la cola tiene elementos, se quita el
// primero de la cola, y se devuelve su valor, si esta vacia, devuelve NULL.
// Pre: la cola fue creada.
// Post: se devuelve el valor del primer elemento anterior, la cola
// contiene un elemento menos, si la cola no estaba vacia.
void* cola_desencolar(cola_t *cola);


/* *****************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/
// Crea una lista.
// Post: devuelve una nueva lista vacia.
lista_t* lista_crear();


// Destruye la lista. Si se recibe la funcion destruir_dato por parametro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una funcion capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *));


// Devuelve dato de tipo size_t expresando la cantidad de elementos que contiene
// la lista. Si la lista no contiene elementos, devuelve 0.
// Pre: la lista fue creada.
size_t lista_largo(const lista_t *lista);


// Devuelve verdadero o falso, segun si la lista tiene o no elementos.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);


// Agrega un nuevo elemento a la lista en la primera posicion. Devuelve 
// falso en caso de error.
// Pre: la lista fue creada.
// Post: se agrego un nuevo elemento a la lista, valor se encuentra al principio
// de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);


// Agrega un nuevo elemento a la lista en la ultima posicion. Devuelve 
// falso en caso de error.
// Pre: la lista fue creada.
// Post: se agrego un nuevo elemento a la lista, valor se encuentra al final
// de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si esta vacia devuelve NULL.
// Pre: la lista fue creada.
// Post: se devuelve el primer elemento de la lista, cuando no esta vacia.
void *lista_ver_primero(const lista_t *lista);


// Saca el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primero de la lista, y se devuelve su valor, si esta vacia, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devuelve el valor del primer elemento anterior, la lista
// contiene un elemento menos, si la lista no estaba vacia.
void *lista_borrar_primero(lista_t *lista);


// Inserta un dato en la lista en la posicion que se pasa por parametro.
// Pre: la lista no es vacia.
// Post: se ha insertado el valor pasado por parametro en la posicion
// senalada por el iterador. Los elementos ubicados luego de esa posicion
// avanzan su posicion en uno.
bool lista_insertar(lista_t *lista, lista_iter_t *iter, void *dato);
        
    
// Elimina el dato en la lista ubicado en la posicion que se pasa por parametro.    
// Pre: la lista no es vacia.
// Post: se elimina de la lista el elemento senalado por el iterador.
void *lista_borrar(lista_t *lista, lista_iter_t *iter);


// Pre: Recibe una lista como parametro y crea un iterador de ella.
// Post: devuelve un iterador de lista posicionado en el primer elemento.
// Si la lista esta vacia devuelve NULL.
lista_iter_t *lista_iter_crear(const lista_t *lista);


// Avanza una posicion en la lista.
// Pre: el iterador fue creado.
// Post: se avanzo una posicion en la actual del iterador. Si la posicion
// actual era la ultima, se devuelve false.
bool lista_iter_avanzar(lista_iter_t *iter);


// Devuelve un puntero al valor de la posicion donde se encuentra el
// iterador. Si la lista esta vacia, devuelve NULL.
// Pre: el iterador fue creado.
void *lista_iter_ver_actual(const lista_iter_t *iter);


// Verifica si se encuentra al final de la lista.
// Pre: el iterador fue creado.
// Post: devuelve true si el iterador se encuentra al final de la lista
// false si no se encuentra al final de la lista.
bool lista_iter_al_final(const lista_iter_t *iter);


// Destruye el iterador.
// Pre: el iterador fue creado.
// Post: se elimina el iterador.
void lista_iter_destruir(lista_iter_t *iter);


/* *****************************************************************
 *                    PRIMITIVAS DEL VECTOR 
 * *****************************************************************/

// Crea un vector de tamaño tam
// Post: vector es una vector vacío de tamaño tam
vector_t* vector_crear(size_t tam);

// Destruye el vector
// Pre: el vector fue creado
// Post: se eliminaron todos los elementos del vector
void vector_destruir(vector_t *vector);

// Cambia el tamaño del vector
// Pre: el vector fue creado
// Post: el vector cambió de tamaño a nuevo_tam y devuelve true
// o el vector queda intacto y devuelve false si no se pudo cambiar el tamaño
// a nuevo_tam
bool vector_redimensionar(vector_t *vector, size_t nuevo_tam);

// Almacena en valor el dato guardado en la posición pos del vector
// Pre: el vector fue creado
// Post: se almacenó en valor el dato en la posición pos. Devuelve false si la
// posición es inválida (fuera del rango del vector, que va de 0 a tamaño-1)
bool vector_obtener(vector_t *vector, size_t pos, int *valor);

// Almacena el valor en la posición pos
// Pre: el vector fue creado
// Post: se almacenó el valor en la posición pos. Devuelve false si la posición
// es inválida (fuera del rango del vector, que va de 0 a tamaño-1) y true si
// se guardó el valor con éxito.
bool vector_guardar(vector_t *vector, size_t pos, int valor);

// Devuelve el tamaño del vector
// Pre: el vector fue creado
size_t vector_obtener_tamanio(vector_t *vector);


#endif // TDAS_H
