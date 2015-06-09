LIB=-lboost_iostreams -lboost_serialization
CPP=test-archive.cpp
EXE=test-archive

all: ${EXE}

${EXE}: ${CPP}
	@g++ -g -o ${EXE} ${CPP} ${LIB}
	@echo "To test: use \"make run\""

run: ${EXE}
	@./${EXE} save a.file
	@./${EXE} load a.file
	@cat a.file
	@echo

clean:
	@rm -f ${EXE} *.o a.file
