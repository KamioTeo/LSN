// ESERCIZIO 1: Integrazione Monte Carlo di M_PI/2*cos(M_PI*x/2) sul quarto di periodo

#include <iostream>
#include <fstream>
#include <string>
#include "../RandomNumberGenerator/random.h"

using namespace std;

// Funzione di cui calcolare l'integrale
double func(double x) {
    return (M_PI/2) * cos(M_PI*x/2);
}

// Funzione della pdf da usare nel secondo metodo
double pdf(double x) {
    return 2. * (1-x);
}

// Generatore distribuzione pdf del tipo 2(1-x)
double cumulative_inverse(double y){
	  return 1-sqrt(1-y);
  }

// f(x) / g(x)
double g(double x){
	return func(cumulative_inverse(x))/pdf(cumulative_inverse(x));
}

int main (int argc, char *argv[]){

// Generazione del random number generator 
	Random rnd("../RandomNumberGenerator/Primes", "../RandomNumberGenerator/seed.in");

// Definisco le variabili che mi servono per l'analisi
	const int M = 1E6; // Numero di estrazioni
	const int N = 100; // Numero di blocchi
	
	const int a = 0; // Primo estremo d'integrazione
	const int b = 1; // Secondo estremo d'integrazione

// Punto 1: estrazione uniforme della variabile casuale
	std::vector<double> Integral_unif(M); // Vettore che conterrà gli addendi per il calcolo della media dell'integrale

	for (int i = 0; i < M; i++) { // Per ogni simulazione
		Integral_unif[i] = (b-a)*func(rnd.Rannyu(a, b)); // aggiungo al vettore l'addendo che serve per il calcolo della media dell'integrale sul blocco. I numeri uniformi vanno estratti nell'intervallo d'integrazione
	}

// Uso la funzione Ave_Block per salvare su file esterno le medie cumulative e i loro errori, al variare degli N blocchi
	Ave_Block(Integral_unif, N, "dati/Dati_es2-1-1.txt");

// PUNTO 2: Analogo ma con estrazione da distribuzione non uniforme
// IN questo caso i numeri sono estratti seguendo una distribuzione come la funzione pdf definita sopra. Per farlo si utilizzo il metodo dell'inversa della cumulativa
// In questo caso non impongo i limiti d'integrazione perché, per come è ricavata l'inversa, vale solo tra 0 e 1

	std::vector<double> Integral_non_unif(M); // Vettore che conterrà gli addendi per il calcolo della media dell'integrale

	for (int i = 0; i < M; i++) { // Per ogni simulazione
		Integral_non_unif[i] = g(rnd.Rannyu()); // Creo l'addendo per il calcolo della media dell'integrale con il secondo metodo
	}

// Analogo a prima, uso la funzione Ave_Block ma sul nuovo vettore
	Ave_Block(Integral_non_unif, N, "dati/Dati_es2-1-2.txt");

   rnd.SaveSeed();
   return 0;
}
