#include <Arduino.h>
#define X 50 //Taille du tableau de moyenne de valeurs lues
#define Y 10 //Taille du tableau de valeurs de temps où les battements ont lieux
#define SEUIL 1

const int capteur = A0; //Port de lecture du capteur
unsigned int reading = 0; //Valeur lue sur le port du capteur
unsigned int c = 0; //L'incrémanteur pour le tableau de valeurs lues par le capteur
unsigned long b = 0; //L'incrémanteur pour le tableau de temps où sont mesurés les battements
float moyenne = 0; //La moyenne des valeurs du tableau
float moyenneMax = 0;
double d = 0; //La validation des X premières valeurs
unsigned int readed[X] = {0}; //Le tableau de valeurs lues par le capteur
unsigned long tempsBattements[Y] = {0}; //Le tableau de temps où sont mesurés les battements
int tableauVU[X] = {0};
int tableauMax[Y] = {0};
double bpm = 0.0; //Les battements par minute
float tempsMoyen = 0.0;
float valeurUtile = 0;
unsigned int tempsPrecedent = 0; //Evolue avec les valeurs du tableau de temps mais avec un tour de retard
unsigned long temps = 0;
int max = 0;
int a = 0;
int e = 0;
int f = 0;

void setup() {
  pinMode(capteur,INPUT); //Paramétrage du port de lecture du capteur en entrée
  Serial.begin(9600); //Fréquence de l'information en bits/s
}

void loop() {

  temps = millis(); //Définition du temps (exprimé en millisecondes)

  //Lecture et écriture des valeurs dans les variables et le tableau
  reading = analogRead(capteur); //Lecture du capteur
  valeurUtile = (float)reading - moyenne;

  //Moyenne glissante
  if(d != 0) { //Si la moyenne a déjà été calculée
    d -= readed[c];
    readed[c] = reading;
    d += readed[c];
    moyenne = (float)d / (float)(X);
  }
    
    max = tableauVU[0];
    tableauVU[c] = valeurUtile;
    
  //Moyenne max
  if(d != 0) { //Si la moyenne a déjà été calculée
    for(int i = 0; i < X; i++) {
      max = (tableauVU[i] > max) ? tableauVU[i] : max;
    }
    f -= tableauMax[e];
    tableauMax[e] = max;
    f += tableauMax[e];
    moyenneMax = (float)f / (float)X;
    e = (e + 1) % Y;
  }

  Serial.println(bpm); //Affichage du bpm calculé
  readed[c] = reading; //Placement de la valeur lue dans le tableau
  c = (c + 1) % X; //Incrémentation

  //Première moyenne
  if(c == 0 && d == 0) { //Si l'incrémenteur vaut X et le tableau n'a encore jamais été rempli
    for(int i = 0; i < X; i++) { //Pour chaque valeur du tableau
      d += readed[i]; //Somme des valeurs du tableau
    }
    moyenne = d / X; //Division de la somme des valeurs par le nombre de valeurs présentes dans le tableau
  }

  //Détection battement
  if(valeurUtile >= moyenneMax + SEUIL) {
    if(temps - tempsPrecedent >= 300) {
      tempsBattements[b] = temps;
      tempsPrecedent = tempsBattements[b];
      b++;
      if(b == Y) b = 0;
    }
  }

  //Calcul du bpm  
  if(b == 0) {
    for(int i = 0; i < Y-1; i++) b += abs(tempsBattements[i+1] - tempsBattements[i]);
    tempsMoyen = (float)b / (float)(Y-1);
    bpm = (100 * 60) / tempsMoyen;
    bpm *= 10;
    b = 0;
  }
}