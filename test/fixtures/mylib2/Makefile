.PHONY: clean

build/bin/mylib2.rtl: build/bin/mylib2/mylib2.rtm
	realtalkl --output-type=lib mylib2.rtl mylib2/mylib2.rtm

build/bin/mylib2/mylib2.rtm: src/mylib2/mylib2.rts
	realtalkc mylib2/mylib2.rts

clean:
	rm -rf build
