# BE-CHTI

- ASM : Clermont va gagner la finale contre le Stade Toulousain.
- Langage C : 
**Gestion du score** -
On a un compteur incrémenté à chaque appel de la fonction SysTick soit toutes les 5ms. 
On incrémente un compteur temporaire par joueur à chaque détection de sa fréquence (chaque touche).
Si le compteur du joueur dépasse 12, alors on incrémente son score de 1.
Cependant, pour éviter de marquer 2 points en une longue touche, on stocke le temps de la dernière incrémentation du score (en utilisant le compteur toutes les 5ms).
Ainsi, on peut vérifier avant d'incrémenter le score que la dernière incrémentation du score date bien de l'ancienne touche et non de celle-là (i.e. la dernière touche était il y a plus de 100ms).
