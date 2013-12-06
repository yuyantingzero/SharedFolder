
SCONS_OPTIONS="USE_MYSQL=false"

all: m5 ALPHA/libm5shared.so


#
#	@echo "No default build target. Current build targets include:"
#	@echo "     simple_test      (only demonstrates linking)"
#	@echo "     random_tester    (validates correctness of system)"
#	@echo "     speed_tester     (tests speed of system)"

simple_test: 
	make ALPHA/simple_test

random_tester: 
	make ALPHA/random_tester

speed_tester: 
	make ALPHA/speed_tester

m5:
	ln -s .. m5

ext:
	ln -s ../../ext ext

ALPHA/libm5static.a: m5 ext
	scons ALPHA/libm5static.a ${SCONS_OPTIONS}

ALPHA/libm5shared.so: m5 ext
	scons ALPHA/libm5shared.so ${SCONS_OPTIONS}

ALPHA/simple_test: m5 m5/m5elements/simple_test.c ALPHA/libm5shared.so
	gcc -o ALPHA/simple_test m5/m5elements/simple_test.c -O5 -g -Im5 -LALPHA/ -Llibelf/lib -lz -lm5shared -lelf

ALPHA/random_tester: m5 m5/m5elements/tester.cpp ALPHA/libm5shared.so
	g++ -o ALPHA/random_tester m5/m5elements/tester.cpp -O5 -g -Im5 -LALPHA/ -Llibelf/lib -lz -lm5shared -lelf

ALPHA/speed_tester: m5 m5/m5elements/speed_tester.cpp ALPHA/libm5shared.so
	gcc -o ALPHA/speed_tester m5/m5elements/speed_tester.cpp -O5 -g -Im5 -LALPHA/ -Llibelf/lib -lz -lm5shared -lelf

clean:
	-rm ALPHA/libm5static.a ALPHA/libm5shared.so ALPHA/simple_test ALPHA/random_tester ALPHA/speed_tester

spotless: clean
	-scons --clean ALPHA/libm5shared.so
	-scons --clean ALPHA/libm5static.so



