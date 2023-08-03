Il codice è analogo a quello utilizzato per l'esercitazione 4, ma nei vari file input è stato imposto iNVET = 1. Il file da caricare cambia in base allo stato di aggregazione da analizzare. Rispetto al codice originale, sono stati aggiunti alcuni parametri d'ingresso impostabili nel file input:

1) PrintAcceptance
Se 1, non esegue il codice ma stampa a video il valore di accettazione calcolato durante la fase di equilibrazione, per cui si è fissato neq = 1000 per Metropolis e 2000 per Verlet negli stati liquido e solido, 100000 per lo stato gassoso
Se 0 non stampa questi valori e prosegue con il codice.

2) PrintEqValues
Se 1, non esegue il codice ma stampa un file con i valori relativi all'energia e alla temperatura del sistema per ogni step eseguito (scelto da file di input "neq") durante l'equilibrazione. Per questa fase è stato fissato neq = 1000, mentre nstep non rientra.

3) PrintEnergy
Se 1, stampa i valori dell'energia dopo l'equilibrazione (per Metropolis). In questo caso è stato fissato nstep = 500000 e neq = 200

Le tail corrections sono state aggiunte solamente alla parte di algoritmo di Metropolis, includendola per completezza anche al calcolo dell'energia potenziale per il campionamento della distribuzione di Boltzmann. Quest'ultima non è in realtà necessaria perché si lavora con differenze di energia, quindi la correzione si semplifica.

Per l'analisi della g(r) sono stati impostati neq = 200, nblk = 100 e nstep = 1000