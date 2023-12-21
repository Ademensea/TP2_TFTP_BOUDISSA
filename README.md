TP 2 :Client TFTP

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
