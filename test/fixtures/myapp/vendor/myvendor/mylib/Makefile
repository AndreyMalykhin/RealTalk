.PHONY: clean

build/bin/mylib.rtl: build/bin/mylib/mylib.rtm
	realtalkl --debug --output-type=lib mylib.rtl mylib/mylib.rtm

build/bin/mylib/mylib.rtm: src/mylib/mylib.rts
	realtalkc --debug mylib/mylib.rts

clean:
	rm -rf build
