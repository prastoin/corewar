./asm test.s
../ressources/asm test.s
hexdump yolo.cor > yolo.txt ;
hexdump test.cor > test.txt ;
vimdiff yolo.txt test.txt ;
