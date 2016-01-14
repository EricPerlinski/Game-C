#definition du compilateur
CC = gcc

DIR = 
#$(shell pwd)/

SRC = $(DIR)src/

DEST = $(DIR)bin/

#specification des options du compilateur
CFLAGS = -ansi -Wall -g

#librairies
LIB = -lpng

#allegro

ALLEGRO = `pkg-config --cflags --libs allegro-5.0 allegro_ttf-5.0 allegro_acodec-5.0 allegro_audio-5.0 allegro_color-5.0  allegro_image-5.0 allegro_main-5.0 allegro_memfile-5.0 allegro_primitives-5.0 `

#nom de l executable
PROG = Jeu.out

#definition de la cible a reconstruire
all : directories $(DEST)$(PROG)

$(DEST)$(PROG) : $(DEST)main.o $(DEST)tile.o $(DEST)map.o $(DEST)jeu.o $(DEST)munition.o $(DEST)event.o $(DEST)menu.o $(DEST)medikit.o $(DEST)projectile.o $(DEST)perso.o $(DEST)boss.o $(DEST)customlinkedlist.o $(DEST)ennemi.o
	$(CC) $(DEST)main.o $(DEST)tile.o $(DEST)map.o $(DEST)jeu.o $(DEST)munition.o $(DEST)event.o $(DEST)menu.o $(DEST)medikit.o $(DEST)projectile.o $(DEST)perso.o $(DEST)boss.o $(DEST)customlinkedlist.o $(DEST)ennemi.o -o $(DEST)$(PROG) $(ALLEGRO) -lm

$(DEST)main.o : $(SRC)main.c $(SRC)main.h
	$(CC) $(CFLAGS) -c $(SRC)main.c -o $(DEST)main.o

$(DEST)ennemi.o : $(SRC)ennemi.c $(SRC)ennemi.h
	$(CC) $(CFLAGS) -c $(SRC)ennemi.c -o $(DEST)ennemi.o

$(DEST)projectile.o : $(SRC)projectile.c $(SRC)projectile.h
	$(CC) $(CFLAGS) -c $(SRC)projectile.c -o $(DEST)projectile.o

$(DEST)event.o : $(SRC)event.c $(SRC)event.h
	$(CC) $(CFLAGS) -c $(SRC)event.c -o $(DEST)event.o

$(DEST)boss.o : $(SRC)boss.c $(SRC)boss.h
	$(CC) $(CFLAGS) -c $(SRC)boss.c -o $(DEST)boss.o

$(DEST)tile.o : $(SRC)tile.c $(SRC)tile.h
	$(CC) $(CFLAGS) -c $(SRC)tile.c -o $(DEST)tile.o

$(DEST)perso.o : $(SRC)perso.c $(SRC)perso.h
	$(CC) $(CFLAGS) -c $(SRC)perso.c -o $(DEST)perso.o

$(DEST)medikit.o : $(SRC)medikit.c $(SRC)medikit.h
	$(CC) $(CFLAGS) -c $(SRC)medikit.c -o $(DEST)medikit.o

$(DEST)munition.o : $(SRC)munition.c $(SRC)munition.h
	$(CC) $(CFLAGS) -c $(SRC)munition.c -o $(DEST)munition.o

$(DEST)map.o : $(SRC)map.c $(SRC)map.h
	$(CC) $(CFLAGS) -c $(SRC)map.c -o $(DEST)map.o


$(DEST)jeu.o : $(SRC)jeu.c $(SRC)jeu.h
	$(CC) $(CFLAGS) -c $(SRC)jeu.c -o $(DEST)jeu.o

$(DEST)menu.o : $(SRC)menu.c $(SRC)menu.h
	$(CC) $(CFLAGS) -c $(SRC)menu.c -o $(DEST)menu.o

$(DEST)customlinkedlist.o : $(SRC)customlinkedlist.c $(SRC)customlinkedlist.h
	$(CC) $(CFLAGS) -c $(SRC)customlinkedlist.c -o $(DEST)customlinkedlist.o
	

#########################################################################

#exemple edition de liens et production de l executable
#$(PROG) : fich.o fich2.o
#	$(CC) fich.o fich2.o  -o $(PROG)


#exemple compilation du fichier ListATree
#$(DEST)fich.o : $(SRC)fich.c 
#	$(CC) $(CFLAGS) $(LIB) -c $(SRC)fich.c -o $(DEST)fich.o

#########################################################################



#creation du dossier bin
directories:
	@mkdir -p $(DEST) 2>/dev/null

#destruction de tous les fichiers intermediaires crees
clean : 
	@rm -f $(DEST)*.o

#suppression de tout ce qui peut etre regenere
mrproper : clean
	@rm -f $(DEST)$(PROG)
