CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD -Werror=vla
EXEC = chess
OBJECTS = main.o graphicsdisplay.o window.o cpu.o decorator.o level1cpu.o level2cpu.o level3cpu.o level4cpu.o openseq.o chessboard.o posn.o chesspiece.o linepiece.o pawn.o king.o knight.o queen.o rook.o bishop.o textdisplay.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lX11

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}

#add these back: graphicsdisplay.o window.o
