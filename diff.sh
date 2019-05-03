echo "test 1"
./vm -v -d 1500 test/lld_overflow.cor test/wave.cor test/ultima.cor test/youforkmytralala.cor
./ressources/corewar -v 30 -d 1500 test/lld_overflow.cor test/wave.cor test/ultima.cor test/youforkmytralala.cor > verbose_src
if diff "verbose_src" "verbose" &> /dev/null ; then
	echo "no diff"
else
	echo "diff"
	exit
fi
