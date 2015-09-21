.PHONY: clean

build/bin/mylib.rtl: build/bin/mylib/mylib.rtm
	realtalkl --output-type=lib mylib.rtl mylib/mylib.rtm

build/bin/mylib/mylib.rtm: src/mylib/mylib.rts
	realtalkc mylib/mylib.rts

clean:
	rm -rf build
