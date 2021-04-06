	PRESERVE8
	THUMB   
	include Driver/DriverJeuLaser.inc

; ====================== zone de rservation de donnes,  ======================================
;Section RAM (read only) :
	area    mesdata,data,readonly


;Section RAM (read write):
	area    maram,data,readwrite
	export CurrentIndex
CurrentIndex	dcd	0
	export SortieSon
SortieSon	dcw	0
	extern Son

	
; ===============================================================================================
	


		
;Section ROM code (read only) :		
	area    moncode,code,readonly
; crire le code ici		

	export timer4_callback

timer4_callback proc
; int current_index
; int SortieSon
; int *Son

; SortieSon = *(Son + current_index) (Opération de pointeur, se décaler de 2 octets pour s'aligner sur le son)
;
; if(current_index > 5512)
;    current_index = 0;
;
	; Chargement de l'index
	ldr r0, =CurrentIndex
	ldr r1, [r0]

	; Ajout de l'index  l'adresse de "Son", deux fois pour dcaler de 2 octets
	ldr r2, =Son
;	ldr r2, [r2]
	add r2, r1
	add r2, r1

	; Lecture du son actuel
	ldrsh r2, [r2]

	;Mise sur [0;719]
	;on décale sur [0, 2^16] puis on divise par 2^16*719
	ldr r3, =719 ;On met 719 dans r3
	mul r2, r3	;On multiplie r2 par 719
	asr r2, r2, #15 ;On divise r2 par 2^15
	add r2, #719 ;On ajoute à r2 719
	asr r2, r2, #1 ;On divise r2 par 2
	
	
	; Stockage du son actuel
	ldr r3, =SortieSon
	str r2, [r3]

	; Incrment de CurrentIndex
	add r1, #1
	str r1, [r0]
	
	bx lr
	endp
	END	