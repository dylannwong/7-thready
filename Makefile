fib: thready.c patterns.c
	gcc -o thready thready.c patterns.c

clean:
	rm thready

test: thready
	bash test.sh
