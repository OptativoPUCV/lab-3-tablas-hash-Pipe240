#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * mapa, char * clave, void * valor) {
    if (mapa == NULL || clave == NULL) return;

    // agrandar si al insertar se supera 0.7 
    if ( (mapa->size + 1) * 10 > mapa->capacity * 7 )
    enlarge(mapa);

    long indice = hash(clave, mapa->capacity);
    long primer_borrado = -1;

    while (1) {
        Pair *casilla = mapa->buckets[indice];

        if (casilla == NULL) {
            long destino = (primer_borrado != -1) ? primer_borrado : indice;
            mapa->buckets[destino] = createPair(clave, valor);
            mapa->current = destino;
            mapa->size++;
            return;
        }

        if (casilla->key == NULL) {
            if (primer_borrado == -1) primer_borrado = indice;
        } else if (is_equal(casilla->key, clave)) {
            mapa->current = indice; // clave duplicada: no insertar 
            return;
        }

        indice = (indice + 1) % mapa->capacity; // sondeo lineal circular 
    }
}


void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap * createMap(long capacidad) {
    HashMap *mapa = (HashMap *) malloc(sizeof(HashMap));
    if (mapa == NULL) return NULL;

    mapa->buckets = (Pair **) calloc(capacidad, sizeof(Pair *));
    if (mapa->buckets == NULL) {
        free(mapa);
        return NULL;
    }

    mapa->size = 0;
    mapa->capacity = capacidad;
    mapa->current = -1;
    return mapa;
}


void eraseMap(HashMap * map,  char * key) {    


}

Pair * searchMap(HashMap * mapa, char * clave) {
    if (mapa == NULL || clave == NULL) return NULL;

    long indice = hash(clave, mapa->capacity);
    long inicio = indice;

    while (1) {
        Pair *casilla = mapa->buckets[indice];

        if (casilla == NULL) {
            // al encontrar NULL sabemos que la clave no está (corta la búsqueda) 
            return NULL;
        }

        // si la casilla es válida y coincide la clave, éxito 
        if (casilla->key != NULL && is_equal(casilla->key, clave)) {
            mapa->current = indice;  
            return casilla;
        }

        // continuar sondeo lineal circular 
        indice = (indice + 1) % mapa->capacity;

        // resguardo por si acaso
        if (indice == inicio) return NULL;
    }
}


Pair * firstMap(HashMap * map) {

    return NULL;
}

Pair * nextMap(HashMap * map) {

    return NULL;
}
