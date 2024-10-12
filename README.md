# smartplant overview
Système de gestion automatisée des plantes, en utilisant des capteurs et des actionneurs pour créer un environnement idéal pour la croissance des plantes.
•	L'arrosage s'active automatiquement basé sur ces estimations et des seuils prédéfinis.
•	De manière similaire, la LED s'allume automatiquement lorsque l'intensité lumineuse ambiante est inférieure au seuil prédéfini.

## Choix des capteurs et actuateurs
- **Capteurs**: 
  - DHT11 : Mesure l'humidité ambiante et la température. Bien que ne mesurant pas directement l'humidité du sol, ces données permettront d'inférer les besoins en eau des plantes.
  - Photorésistance : Pour surveiller l'intensité lumineuse, essentielle pour la photosynthèse.

- **Actuateurs**:
  - Relais : Utilisé pour activer une pompe à eau, permettant un arrosage automatique basé sur les données environnementales.
  - LED : Pensée comme un éclairage complémentaire pour fournir un supplément de lumière à la plante lorsque l'intensité lumineuse ambiante est insuffisante.

- **Dashboard IRT**:
  - Affichage :
    •	Humidité ambiante, température et intensité lumineuse.
    •	Statut du système (arrosage actif, niveau d'eau).
  - Contrôle :
    •	Bouton pour lancer ou arrêter manuellement l'arrosage.
    •	Réglages pour ajuster les seuils d'activation de l'arrosage.
    •	Bouton pour allumer ou éteindre manuellement la LED.
    •	Options pour régler le seuil de luminosité déclenchant automatiquement l'allumage de la LED.

## Technologies utilisees
- **BLE**
- **MQTT**
- **Interface serial via FTDI**
