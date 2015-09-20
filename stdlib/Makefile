.PHONY: clean

build/bin/std.rtl: build/bin/std/io.rtm
	realtalkl --debug --output-type=lib std.rtl std/io.rtm

build/bin/std/io.rtm: src/std/io.rts
	realtalkc --debug std/io.rts

clean:
	rm -rf build
