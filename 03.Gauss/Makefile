include ../Makefile.inc

release : gauss generate_system

gauss  : util.r.o Gauss_main.r.o Gauss_solve.r.o
	$(LD) -fopenmp -o $@ $^ $(LD_EXTRA_OPT)

generate_system : generate_system.r.o
	$(LD) -o $@ $^

debug   : util.d.o Gauss_main.d.o Gauss_solve.d.o
	$(LD) -fopenmp -o $@ $^ $(LD_EXTRA_OPT)

utest   : Gauss_test.d.o util.d.o Gauss_solve.d.o
	$(LD) -fopenmp -o $@ $^ $(GTEST_LIBS)

run_utest : utest
	./$^

run_itest : release
	echo "Integration tests..."
	./integration_test.sh

all: depends run_utest run_itest

include Makefile.hdrdep