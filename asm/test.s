.name "42"
.comment "Just \"a basic Winner \n Program"

live	%42		# entree
	ld	%0,r5
	ld	%0,r5
	zjmp	%42

	sti	r1,%42,%1
	ld	%2304,r10
	ld	%1,r5
	ld	%0,r11

	ld	%4,r3
	live	%42
	live	%742
	sub	r3,r5,r3
	zjmp	%42
	ld	%0,r11
	live	%42
	sti	r10,%-510,%0
	sti	r10,%-404,%0
	sti	r10,%-303,%0
	sti	r10,%-202,%0
	ld	%0,r11
