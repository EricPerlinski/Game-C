#include "customlinkedlist.h"


List* liste_init(){
	List* l = (List*)malloc(sizeof(List));
	l->tete=NULL;
	l->queue=NULL;
	l->size=0;
	return l;
}

void liste_dest(List* l, FonctionFree fct_free){
	if(l->size!=0){
		Elem* e1 = l->tete;
		Elem* e2 = NULL;
		bool first=true;
		while(liste_a_un_suivant(l,e1) || first){

			e2=e1;
			if(first){
				first=false;
			}else{
				e2=e1;
				e1 = liste_suivant(l,e1);
				elem_dest(e2);
			}
			((*fct_free)(e1->object));
		}
	}
	free(l);
}



void liste_add_last(List* l,void* obj){
	Elem* el = elem_init(obj);
	if(l->size==0){
		l->tete=el;
		l->queue=el;
	}else{
		l->queue->suivant=el;
		el->precedent=l->queue;
		l->queue=el;	
	}
	l->size++;
}

Elem* liste_tete(List* l){
	return l->tete;
}





bool liste_a_un_suivant(List* l, Elem* el){
	return el->suivant!=NULL;
}

Elem* liste_suivant(List* l, Elem* el){
	return el->suivant;	
}


void liste_supprime(List* l, Elem* el){
	Elem* prec = el->precedent;
	Elem* suc = el->suivant;
	if(prec == NULL && suc == NULL){
		l->tete=NULL;
		l->queue=NULL;
	}else if(prec == NULL){
		l->tete=suc;
		suc->precedent=NULL;
	}else if(suc==NULL){
		l->queue=prec;
		prec->suivant=NULL;
	}else{
		prec->suivant=suc;
		suc->precedent=prec;
	}
	l->size--;
}

void liste_traiter_foncteur(List* l,void* jeu, FonctionTraitement traitement){
	if(l->size!=0){
		Elem* e = l->tete;
		bool first=true;
		while(liste_a_un_suivant(l,e) || first){
			if(first){
				first=false;
			}else{
				e = liste_suivant(l,e);
			}
			((*traitement)(e,jeu));
		}

	}

}



/**************************************/
/*              ELEM                  */
/**************************************/


Elem* elem_init(void* obj){
	Elem* elem = (Elem*)malloc(sizeof(Elem));
	elem->precedent=NULL;
	elem->suivant=NULL;
	elem->object=obj;
	return elem;
}

void elem_dest(Elem* el){
	free(el);
}

