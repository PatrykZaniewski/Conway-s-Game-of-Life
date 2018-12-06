compile: ./src/main.c
	cc -Wall -std=c99 ./src/main.c ./src/read.c ./src/print.c ./src/border.c ./src/generation.c -lpng -o life.out

no_input: life.out
	./life.out

no_number: life.out
	./life.out ./tests/a.txt

minus_number: life.out
	./life.out ./tests/a.txt -15

no_output: life.out
	./life.out ./tests/a.txt 15

wrong_size: life.out
	./life.out ./tests/wrong_size.txt 5 export.txt

data_notcomplete: life.out
	./life.out ./tests/data_notcomplete.txt 5 export.txt

wrong_symbol: life.out
	./life.out ./tests/wrong_symbol.txt 5 export.txt

data_overload: life.out
	./life.out ./tests/data_overload.txt 5 export.txt

ok: life.out
	./life.out ./tests/ok.txt 5 export.txt


