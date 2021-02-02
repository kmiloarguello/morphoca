
all:	Dilatation Erosion Opening Closing GrayDilation

clean:
	rm *.o ./Dilatation ./Erosion ./Opening ./Closing ./GrayDilation

mcimage.o:	mcimage.c mcimage.h mcutil.h mccodimage.h
	gcc -c mcimage.c

graph_basic.o:	graphes.h graph_basic.c
	gcc -c graph_basic.c

Dilatation:	Dilatation.c mcimage.h mccodimage.h graphes.h mcimage.o graph_basic.o
	gcc Dilatation.c mcimage.o graph_basic.o -o Dilatation

Erosion: Erosion.c mcimage.h mccodimage.h graphes.h mcimage.o graph_basic.o
	gcc Erosion.c mcimage.o graph_basic.o -o Erosion

Opening: Opening.c mcimage.h mccodimage.h graphes.h mcimage.o graph_basic.o
	gcc Opening.c mcimage.o graph_basic.o -o Opening

Closing: Closing.c mcimage.h mccodimage.h graphes.h mcimage.o graph_basic.o
	gcc Closing.c mcimage.o graph_basic.o -o Closing

GrayDilation: GrayDilation.c mcimage.h mccodimage.h graphes.h mcimage.o graph_basic.o
	gcc GrayDilation.c mcimage.o graph_basic.o -o GrayDilation