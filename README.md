TP 2 :Client TFTP                                                                                        BOUDISSA ADEM 


Objectifs : Réaliser un client TFTP, à l’aide des RFC et de captures wireshark

1/ Question 1: Utilisation des arguments passés à la ligne de commande des programmes gettftp et puttftp pour obtenir les informations de requêtes (serveur et fichier)

On récupère dans argv[1] l’adresse du serveur (on pourrait augmenter la taille du char pour accepter des adresses plus longues, mais dans notre cas c’est suffisant), et on le place dans la variable host
et dans argv[2] le nom du fichier concerné qu’on place dans la variable file.

2/ Question 2 : Appel à getaddrinfo pour obtenir l’adresse du serveur 

On commence par tout initialiser à 0 avec la fonction memset.
On définit le socktype comme étant SOCK_DGRAM soit pour un connexion du type UDP et le family comme étant AF_INET pour l’utilisation d’une adresse IPV4 et finalement le protocol à 0 pour le protocole par défaut. 
Ensuite on effectue la recherche d'adresse en utilisant le nom d'hôte et le service "69" avec avec “getaddrinfo()” dont on renvoie les résultats dans la variable res avec 0 en cas de succès et -1 en cas d'erreur.  

3/ Question 3: Réservation d’un socket de connexion vers le serveur 

On utilise la fonction socket() pour créer un nouveau socket ayant pour paramètres family, socktype et protocol.
Cette fonction renvoie un descripteur de socket, qui est un entier non négatif si le socket est créé avec succès, -1 en cas d'erreur.
Si une erreur survient, le programme affiche un message d'erreur. 

4/ Question 4 a) : Construction d’une requête en lecture (RRQ) correctement formée, et envoie au serveur

On se base sur la construction d’une requête en lecture de ce type 




![Capture](https://github.com/Ademensea/TP2_TFTP_BOUDISSA/assets/152478943/b8766f4b-7108-4fb5-82a5-8cfb50c6a028)


Le code op. va de 1 à 5 
01 pour une demande de lecture de paquet (RRQ, client vers serveur)
02 pour une demande d’écriture de paquet (WRQ, client vers serveur)
03 pour un transfert de data d’un paquet entre le client et le serveur (DATA)
04 pour un accusé de réception de la data d’un paquet entre le client et le serveur (ACK)
05 pour un code d’erreur dans le transfert TFTP

On doit donc créer un code qui crée une chaîne représentant cette requête, puis qui l’envoie avec la fonction sendto(). 
Le code op. est contenue dans buf[0] et buf[1]. 
Le nom du fichier est lui écrit à partir de buf[2] 
On écrit ensuite octet. 
Le buffeur étant mis à 0 à l’aide de la fonction memset, pas besoin de remettre les deux octet précisés à 0. 
buf est ensuite envoyé à l’aide de la fonction sendto()


Question 4 b) : Réception d’un fichier constitué d’un seul paquet de données (DAT) et son acquittement (ACK)


![Capture](https://github.com/Ademensea/TP2_TFTP_BOUDISSA/assets/152478943/76a12751-ad9e-4abc-97cc-92f0ca8460f6)

Voilà donc la forme des paquets qu’on reçoit par le serveur. Le Block# défini quel paquet de données est envoyé par le serveur, sur cette partie il est censé être toujours égal à 1.


Pour la réception du paquet on a donc : 
BUF_SIZE_DATA = 516 car on ne peut transférer que 512 octet + les 4 octets dûs à l’entête.
C’est la taille maximale d’un paquet en UDP. 
On utilise donc la fonction recvfrom() pour recevoir les données du serveur qu’on écrit dans la variable buf.


Sur Wireshark, on se place en loopback, et voilà ce qu’on obtient 

![Capture](https://github.com/Ademensea/TP2_TFTP_BOUDISSA/assets/152478943/b6171690-4ccb-4e29-95c9-dc3d0f942c28)

On constate que l’adresse IP reste similaire pour la communication, mais que le port utilisé pour communiquer avec le serveur change. Le port de la machine reste 33572, mais celui du serveur passe de 1069 à 34343. 
On peut d’abord constater que la première requête RRQ était bien effectuée vers le serveur.
On peut également constater que la dernière requête d’ACK est bien envoyée au serveur (ici écrite en hexadécimal) avec  0004 0001, donc 04 01. 

Question 4 c) : Réception d’un fichier constitué de plusieurs paquets de données (DAT) et leurs acquittements respectifs (ACK) 

Le but de cette question est de répéter l’envoie de plusieurs paquets de données sur un seul fichier, avec leurs acquittements.

Malheuresement à partir de cette question , j'ai rencontré des difficultés avec mon terminal qui disait : permission denied je n'arrivais pas à tester la fonction 
De plus le fichier makefile ne voualait pas excuter ma fonction .

5/ Question 5 : Construction d’une requête en écriture (WRQ) correctement formée, et envoie au serveur

Le but ici est d’envoyer un fichier sur le serveur. Le serveur créera alors une copie de ce fichier. 
On commence donc par faire une requête d’écriture de paquet au serveur (WRQ).
La seule différence par rapport à la question  4.a est le code opératoire qui passe de 1 à 2. 

Pour l' Envoie d’un fichier constitué d’un seul paquet de données (DAT) et de la réception de son acquittement (ACK)
On ouvre le fichier en lecture seule afin de pouvoir lire son contenu.
Pour cela on utilise  la fonction open() pour l’ouverture et la fonction read() pour le lire. 
On alloue à la variable du contenu de ce fichier une taille de 512 octets afin de garder les 4 octets réservés à l’opcode





