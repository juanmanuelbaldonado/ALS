CPP = /usr/local/opt/llvm/bin/clang++
CPPFLAGS = -I /usr/local/opt/llvm/include \
			-I /usr/local/Cellar/boost/1.71.0/include/ \
			-L /usr/local/Cellar/boost/1.71.0/lib/ \
			-I /usr/local/include/eigen3/ \
			-O3 \
			-fopenmp \
			-lboost_program_options

LDFLAGS = -L/usr/local/opt/llvm/lib

main: src/main.cpp
	@echo "Building source.."
	$(CPP) $(CPPFLAGS) $^ -o bin/main $(LDFLAGS)

clean:
	@echo "Cleaning up.."
	rm bin/main