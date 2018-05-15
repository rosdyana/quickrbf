all:
	gcc -Wall -O3 -lm -o quickrbf quickrbf.c 
	gcc -Wall -O3 -lm -o datatrans datatrans.c 
	gcc -Wall -O3 -lm -o datascale datascale.c 
	gcc -Wall -O3 -lm -o centerselect centerselect.c 
clean:
	rm -f quickrbf datatrans datascale centerselect
