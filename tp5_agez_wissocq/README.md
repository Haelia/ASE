Adrien Agez <br/>
Sarah Wissocq


# Compilation
`make` : compile la bibliothèque drive.c et les commandes `dmps`, `format` et `write`

# Utilisation

* `./dmps [cyl] [sect]` affiche les octets du secteur du disque à la position demandée (en format hexadécimal)
* `./format` détruit toutes les données et place tous les octets du disque à `0x00`
* `./write [cyl] [sect]` écrit la valeur `0xdeadbeef` en boucle sur tout le secteur. Il permet de tester l'écriture de la bibliothèque `drive.c`

-----

Le disque (vdiskA.bin) est créé lors du premier appel à l'une des commandes. Il dispose de 16 cylindres, chacun d'eux contenant 16 secteurs de 256 octets.

* `make clean` supprime tous les fichiers objets et executables
* `make realclean` supprime en plus vdiskA.bin
