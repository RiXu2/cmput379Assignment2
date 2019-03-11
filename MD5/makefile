.PHONY: clean


hasher: md5Hash.c
	gcc -std=c99 -o md.out md5Hash.c -lssl -lcrypto
	
xmler: 
	gcc -o xml0.out -std=c99 `xml2-config --cflags` XMLexample2.c `xml2-config --libs`
	
clean:
	$(RM) *.out
