all:
	gcc quickrbf.c -Wall -O3 -o build\quickrbf -lm
	gcc datatrans.c -Wall -O3 -o build\datatrans -lm
	gcc datascale.c -Wall -O3 -o build\datascale -lm
	gcc centerselect.c -Wall -O3 -o build\centerselect -lm
clean:
	rm -f quickrbf datatrans datascale centerselect
