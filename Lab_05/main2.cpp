// Hydrogen Wave function Metropolis sampling
// Transizione Gaussiana

#include "../RandomNumberGenerator/random.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// Scrivo tutto in unità di raggio di Bohr
// Pdf Ground State
	double pdf_gs(double x, double y, double z) {
		return exp(-2*sqrt(x*x+y*y+z*z))/M_PI;
  }

// Pdf stato eccitato 2p
	double pdf_2p(double x, double y, double z) {
		return  z*z*exp(-sqrt(x*x+y*y+z*z))/(32.*M_PI);
  } // cos(theta) = z / |r|

int main(int argc, char *argv[]) {

  // Generazione del random number generator
  Random rnd("../RandomNumberGenerator/Primes",
             "../RandomNumberGenerator/seed.in");

  // Definizioni variabili 
  const int M = 1E6; // Numero di posizioni estratte
  const int N = 100; // Numero di blocchi
  // const int Neq = 1E3; // Numero di step per raggiungere l'equilibrio

  const double del_gs = 0.873; // range della probabilità di Transizione T tra due vettori posizione. Questo valore è scelto per ottenere un'accettazione di circa il 50% nel caso della pdf ground state. In coordinate cartesiante, T è uniforme in [x_n-del;x_n+del], [y_n-del;y_n+del] e [z_n-del;z_n+del]

  const double del_2p = 1.371; // analogo ma per il caso eccitato
	
  std::vector<double> Accepted_gs(M); // Vettore che conterrà 1 se il dato è accettato o 0 se non lo è, lo analizzerò a blocchi e la sua media dovrà essere 50% per il delta fissato

  std::vector<double> Accepted_2p(M); // analogo ma per il caso eccitato
	
  std::vector<double> Position_gs(M); // Vettore che conterrà i valori di r che seguono la pdf del Ground State

  std::vector<double> Position_2p(M); // Analogo ma per il caso eccitato

  const int N_coord = 10000; // Numero di coordinate da salvare
  float coordinate_2p[N_coord][3]; // Dove salverò le coordinate x,y,z delle posizioni
	
  double x = 0.; // valore iniziale, valore più probabile della pdf
  double y = 0.;
  double z = 0.;

  double A = 0.; // Prob di accettazione

// Caso Ground State
  for (int i = 0; i < M; i++) { // Per ogni simulazione
  	  Position_gs[i] = sqrt(x*x+y*y+z*z);
	  
	  double x_new = rnd.Gauss(x, del_gs*del_gs); // posizione al passo successivo, usando la probabilità di Transizione gaussiana
	  double y_new = rnd.Gauss(y, del_gs*del_gs);
	  double z_new = rnd.Gauss(z, del_gs*del_gs);

// dato che T è simmetrica, il rapporto q è dato solo dal rapporto tra la probabilità da campionare valutata in x_new e x_old
	  double q = pdf_gs(x_new, y_new, z_new)/pdf_gs(x,y,z);
	  
	  if( q < 1 ){ A = q;}
	  else{ A = 1.;}

	  double r = rnd.Rannyu();
	  if( r <= A ){ // se è vero accetto la nuova posizione, quindi x diveta x_new
		 x = x_new;
		 y = y_new;
		 z = z_new;
		 Accepted_gs[i] = 1.; // Conto il fatto che ho accettato
	  }
	  else{ Accepted_gs[i] = 0.;} // Non accettato, la nuova posizione resta quella vecchia, quindi sovrascrivo x_new con un altro tentativo a partire sempre da x
  } // fine ciclo / step del random walk

	
// Caso eccitato
	x = 0.; // resetto la posizione iniziale al valore più probabile di questa seconda pdf (r=z^2 -> z=1 se x=y=0)
	y = 0.;
	z = 1.;
	
  for (int i = 0; i < M; i++) { // Per ogni simulazione
  	  Position_2p[i] = sqrt(x*x+y*y+z*z);
	  
	  double x_new = rnd.Gauss(x, del_2p*del_2p); // posizione al passo successivo, usando la probabilità di Transizione
	  double y_new = rnd.Gauss(y, del_2p*del_2p);
	  double z_new = rnd.Gauss(z, del_2p*del_2p);

	  if(i > M - N_coord) { // Prendo solo le ultime
 	  	coordinate_2p[i-(M-N_coord)][0] = x_new;
 	  	coordinate_2p[i-(M-N_coord)][1] = y_new;
 	  	coordinate_2p[i-(M-N_coord)][2] = z_new;
	  }
	  
// dato che T è simmetrica, il rapporto q è dato solo dal rapporto tra la probabilità da campionare valutata in x_new e x_old
	  double q = pdf_2p(x_new, y_new, z_new)/pdf_2p(x,y,z);
	  
	  if( q < 1 ){ A = q;}
	  else{ A = 1.;}

	  double r = rnd.Rannyu();
	  if( r <= A ){ // se è vero accetto la nuova posizione, quindi x diveta x_new
		 x = x_new;
		 y = y_new;
		 z = z_new;
		 Accepted_2p[i] = 1.; // Conto il fatto che ho accettato
	  }
	  else{ Accepted_2p[i] = 0.;} // Non accettato, la nuova posizione resta quella vecchia, quindi sovrascrivo x_new con un altro tentativo a partire sempre da x
  } // fine ciclo / step del random walk

// stampo il file con le posizioni
  ofstream out;
  out.open("dati_gauss/Posizioni_2p.txt");

  // Controllo la corretta apertura del file
  if (!out.is_open()) {
    cerr << "Error: unable to open Posizioni_2p.txt" << endl;
    exit(1);
  }

	for(int i=0; i < N_coord; i++) {
		out << coordinate_2p[i][0] << " " << coordinate_2p[i][1] << " " << coordinate_2p[i][2] << endl;
	}
	
  out.close();
	
  Ave_Block(Accepted_gs, N, "dati_gauss/Accettazioni_gs_gauss.txt");
  Ave_Block(Position_gs, N, "dati_gauss/Posizioni_gs_gauss.txt");

  Ave_Block(Accepted_2p, N, "dati_gauss/Accettazioni_2p_gauss.txt");
  Ave_Block(Position_2p, N, "dati_gauss/Posizioni_2p_gauss.txt");
	
  rnd.SaveSeed();
  return 0;
}
