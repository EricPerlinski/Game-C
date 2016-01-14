#!/bin/sh

mkdir -p paquet/usr/bin/jeuElem
mkdir -p paquet/DEBIAN

echo "
Package: jeu
Version: 1.0
Section: base
Priority: optional
Architecture: all
Depends: liballegro5.0, liballegro-acodec5.0, liballegro-audio5.0, liballegro-image5.0, liballegro-ttf5.0
Maintainer: Mr Valentin GIANNINI <valentin.giannini@telecomnancy.net> , Mr Eric PERLINSKI <eric.perlinski@telecomnancy.net>
Description: Petit jeu
" > paquet/DEBIAN/control


cp -r sounds img font musique map*.txt bin/Jeu.out paquet/usr/bin/jeuElem

echo "
#!/bin/sh
cd /usr/bin/jeuElem
./Jeu.out
" > paquet/usr/bin/jeu

chmod +x paquet/usr/bin/jeu

dpkg-deb --build paquet
