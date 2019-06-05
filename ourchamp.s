.name "Our Champ !"
.comment "Our beloved champ"

sti r1, %:live, %1
sti r1, %:init, %1
ld %0, r1
init:	live %0
		fork %:init
live:	live %0
		zjmp %:live
