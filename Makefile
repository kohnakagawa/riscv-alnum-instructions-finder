CXX = g++
SPIKE_DIR = ../riscv-isa-sim

INCLUDE = -I$(SPIKE_DIR)/riscv -I$(SPIKE_DIR)/build -I$(SPIKE_DIR)/softfloat

OBJS = main.o
STATIC_LIBS = $(SPIKE_DIR)/build/libspike_main.a $(SPIKE_DIR)/build/libriscv.a

all: finder.out

main.o: main.cpp
	g++ -Wall -Wextra -std=c++14 -c -O3 $(INCLUDE) $< -o $@

finder.out: $(OBJS) $(STATIC_LIBS)
	g++ -Wall -Wextra -std=c++14 -O3 $(OBJS) $(STATIC_LIBS) -o $@

clean:
	rm -rf $(OBJS)

run: finder.out
	./finder.out 32 alnum | awk '{print $$1}' | sort | uniq > inst_list_rv32_alnum.txt
	./finder.out 64 alnum | awk '{print $$1}' | sort | uniq > inst_list_rv64_alnum.txt
	./finder.out 32 ascii | awk '{print $$1}' | sort | uniq > inst_list_rv32_ascii.txt
	./finder.out 64 ascii | awk '{print $$1}' | sort | uniq > inst_list_rv64_ascii.txt
