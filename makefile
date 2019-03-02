.PHONY: clean

# defalut
SimpleSocket: 
	gcc -std=c99 simpleClient.c -o sc.out
	gcc -std=c99 simpleServer.c -o ss.out

clean:
	$(RM) *.out