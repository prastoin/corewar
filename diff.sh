game="test/lld_overflow.cor test/wave.cor test/ultima.cor test/youforkmytralala.cor"

echo "zaz VM"
./ressources/corewar -v 30 -d $1 $game > verbose_src
echo "our VM"
./vm -v -d $1 $game
if diff "verbose_src" "verbose" &> /dev/null ; then
	echo "no diff"
else
	./utils/vm_diff $game
	echo "diff"
	exit
fi

game="test/ex.cor test/ex.cro test/ld_94.cor test/lol.cro"

echo "our VM"
./vm -v -d $1 $game
echo "zaz VM"
./ressources/corewar -v 30 -d $1 $game > verbose_src
if diff "verbose_src" "verbose" &> /dev/null ; then
	echo "no diff"
else
	echo "diff"
	exit
fi

game="Car.cor Car.cor Car.cor Car.cor"

echo "our VM"
./vm -v -d $1 $game
echo "zaz VM"
./ressources/corewar -v 30 -d $1 $game > verbose_src
if diff "verbose_src" "verbose" &> /dev/null ; then
	echo "no diff"
else
	echo "diff"
	exit
fi

game="test/lld_overflow.cor test/wave.cor test/ultima.cor test/youforkmytralala.cor"

echo "our VM"
./vm -v -d $1 $game
echo "zaz VM"
./ressources/corewar -v 30 -d $1 $game > verbose_src
if diff "verbose_src" "verbose" &> /dev/null ; then
	echo "no diff"
else
	echo "diff"
	exit
fi
