all: beJson

beJson: bencode.c bencParse.c
	gcc bencode.c bencParse.c -o beJson

clean:
	rm -f beJson