.PHONY: clean

build/bin/myapp.rte: /usr/local/share/realtalk/stdlib/build/bin/std.rtl vendor/myvendor/mylib/build/bin/mylib.rtl ../mylib2/build/bin/mylib2.rtl build/bin/myapp/mycomponent.rtm
	realtalkl --import=../mylib2 --import=/usr/local/share/realtalk/stdlib myapp.rte std.rtl mylib.rtl mylib2.rtl myapp/mycomponent.rtm

build/bin/myapp/mycomponent.rtm: /usr/local/share/realtalk/stdlib/src/std/io.rts vendor/myvendor/mylib/src/mylib/mylib.rts ../mylib2/src/mylib2/mylib2.rts src/myapp/mycomponent.rts
	realtalkc --import=../mylib2 --import=/usr/local/share/realtalk/stdlib myapp/mycomponent.rts

vendor/myvendor/mylib/build/bin/mylib.rtl:
	$(MAKE) -C vendor/myvendor/mylib

../mylib2/build/bin/mylib2.rtl:
	$(MAKE) -C ../mylib2

clean:
	rm -rf build
	$(MAKE) -C vendor/myvendor/mylib clean
	$(MAKE) -C ../mylib2 clean
