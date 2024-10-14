# MSChrono
Projet de gestion de chronomètres pour les compétitions d'escalade. Ce système permet de synchroniser plusieurs écrans via des microcontrôleurs (ESP32, Arduino, etc.) et de contrôler les chronomètres via une application multiplateforme (Android, iOS, PC).*

# Choix Techniques

Ce projet vise à développer un système de chronométrage pour des compétitions d'escalade, avec une gestion centralisée via Wi-Fi et un affichage clair sur des panneaux LED.

## Matériel Utilisé
- **ESP32-C3 Smile Mini** : Choisi pour ses capacités Wi-Fi/Bluetooth, sa compatibilité avec Arduino, et son faible coût.
- **Panneaux LED P5 HUB75 (320x160 mm)** : Sélectionné pour sa haute luminosité, sa robustesse, et sa taille adéquate pour des salles d'escalade.
- **Alimentation 5V/10A** : Fournit une puissance suffisante pour alimenter les panneaux LED.

## Communication et Synchronisation
- **Protocole Wi-Fi** : Utilisé pour la synchronisation des chronomètres via un réseau local. Chaque ESP32 est connecté au Wi-Fi pour recevoir les commandes du serveur central.
- **API REST** : Une API centralisée est utilisée pour envoyer des commandes de démarrage/arrêt aux panneaux LED via les microcontrôleurs ESP32.

## Affichage
- **Interface HUB75** : Compatible avec le microcontrôleur ESP32, l'interface HUB75 permet un contrôle simplifié des panneaux LED pour afficher du texte et des chiffres de manière claire et dynamique.

## Application de Contrôle
- **Flutter (React Native)

