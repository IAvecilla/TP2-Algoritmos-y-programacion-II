#include "batallas.h"
#define GANO_PRIMERO 1
#define GANO_SEGUNDO -1

#include "gimnasio.h"
#include "pokemon.h"
#include "entrenador.h"

int funcion_batalla_1(void* pkm_1, void* pkm_2) {
    if ( ((pokemon_t*)pkm_1)->velocidad >= (((pokemon_t*)pkm_2)->velocidad) ){
        if (((pokemon_t*)pkm_1)->ataque >= (((pokemon_t*)pkm_2)->defensa) ) {
            return GANO_PRIMERO;
        } else if (((pokemon_t*)pkm_1)->ataque < (((pokemon_t*)pkm_2)->defensa)) {
            return GANO_SEGUNDO;
        }
    } else {
        if (((pokemon_t*)pkm_2)->ataque >= (((pokemon_t*)pkm_1)->defensa) ) {
            return GANO_SEGUNDO;
        } else if (((pokemon_t*)pkm_2)->ataque <= (((pokemon_t*)pkm_1)->defensa)) {
            return GANO_PRIMERO;
        }
    }
    return GANO_PRIMERO;
}

int funcion_batalla_2(void* pkm_1, void* pkm_2) {
    if ( ((pokemon_t*)pkm_1)->velocidad >= (((pokemon_t*)pkm_2)->velocidad) ){
        if (((pokemon_t*)pkm_1)->ataque >= (((pokemon_t*)pkm_2)->ataque) ) {
            return GANO_PRIMERO;
        } else if (((pokemon_t*)pkm_1)->ataque < (((pokemon_t*)pkm_2)->ataque)) {
            return GANO_SEGUNDO;
        }
    } else {
        if (((pokemon_t*)pkm_2)->ataque >= (((pokemon_t*)pkm_1)->ataque) ) {
            return GANO_SEGUNDO;
        } else if (((pokemon_t*)pkm_2)->ataque <= (((pokemon_t*)pkm_1)->ataque)) {
            return GANO_PRIMERO;
        }
    }
    return GANO_PRIMERO;
}

int funcion_batalla_3(void* pkm_1, void* pkm_2) {
    if ( ((pokemon_t*)pkm_1)->velocidad >= (((pokemon_t*)pkm_2)->velocidad) ){
        if (((pokemon_t*)pkm_1)->defensa >= (((pokemon_t*)pkm_2)->defensa) ) {
            return GANO_PRIMERO;
        } else if (((pokemon_t*)pkm_1)->defensa <= (((pokemon_t*)pkm_2)->defensa)) {
            return GANO_SEGUNDO;
        }
    } else {
        if (((pokemon_t*)pkm_2)->defensa >= (((pokemon_t*)pkm_1)->defensa) ) {
            return GANO_SEGUNDO;
        } else if (((pokemon_t*)pkm_2)->defensa <= (((pokemon_t*)pkm_1)->defensa)) {
            return GANO_PRIMERO;
        }
    }
    return GANO_PRIMERO;
}

int funcion_batalla_4(void* pkm_1, void* pkm_2) {
    if ( ((pokemon_t*)pkm_1)->velocidad >= (((pokemon_t*)pkm_2)->defensa) ){
        return GANO_PRIMERO;
    }
    
    return GANO_SEGUNDO;
}

int funcion_batalla_5(void* pkm_1, void* pkm_2) {
    if ( ((pokemon_t*)pkm_1)->ataque >= (((pokemon_t*)pkm_2)->velocidad) ){
        return GANO_PRIMERO;
    }
    
    return GANO_SEGUNDO;
}