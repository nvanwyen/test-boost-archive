LIB=-lboost_iostreams -lboost_serialization
HPP=arc.hpp item.hpp
CPP=main.cpp
EXE=test-archive2

all: ${EXE}

${EXE}: ${HPP} ${CPP}
	@g++ -g -o ${EXE} ${CPP} ${LIB}
	@echo "To test: use \"make run\""

run: ${EXE}
	@./${EXE} save a.file
	@./${EXE} load a.file
	@cat a.file
	@echo

clean:
	@rm -f ${EXE} *.o a.file
