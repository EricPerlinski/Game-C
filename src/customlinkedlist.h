#ifndef CUSTOMLINKEDLIST_H
#define CUSTOMLINKEDLIST_H


#include <stdlib.h>
#include <stdio.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

typedef struct element{
	void* object;
	struct element* precedent;
	struct element* suivant;
} Elem;

typedef struct custom{
	Elem* tete;
	Elem* queue;
	int size;
} List;

typedef void (* FonctionTraitement)(Elem* e, void* var);
typedef void (* FonctionFree)(void* e);

List* liste_init();
void liste_dest(List* l, FonctionFree fct_free);


void liste_add_last(List* l, void* obj);

Elem* liste_tete(List* l);
bool liste_a_un_suivant(List* l, Elem* el);
Elem* liste_suivant(List* l, Elem* el);
void liste_supprime(List* l, Elem* el);

void liste_traiter_foncteur(List* l,void* jeu, FonctionTraitement traitement);
void liste_free(List* l, FonctionFree fct_free);

Elem* elem_init(void* obj);
void elem_dest(Elem* el);

#endif
