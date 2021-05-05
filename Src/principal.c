

#include "DriverJeuLaser.h"

int DFT_ModuleAuCarre(void *, int) ;
extern int * LeSignal;
extern int * LeSignalSinus;



int main(void)
{

// ===========================================================================
// ============= INIT PERIPH (faites qu'une seule fois)  =====================
// ===========================================================================

// Après exécution : le coeur CPU est clocké à 72MHz ainsi que tous les timers
CLOCK_Configure();
	
int tab[64] ;
for (int i = 0; i<64; i++) {
	if (i == 31) {}
	tab[i] = DFT_ModuleAuCarre(&LeSignal, i);
}


	
	

//============================================================================	
	
	
	while	(1)
	{
	}
}

