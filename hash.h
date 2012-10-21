#ifndef HASH_H
#define HASH_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <tdas.h>


#define TAM_INI 10
/*
 *TIPOS
 */
typedef struct nodo_hash nodo_hash_t;
typedef struct hash hash_t;

/*
 *PRIMITIVAS
 */
 
hash_t hash_crear(void);


#endif
