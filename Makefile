all:
	gcc main.c -o bin -Wl,--export-dynamic `pkg-config --libs --cflags gtk+-3.0 webkit2gtk-4.0`
run:
	LIBGL_DRI3_DISABLE=1 ./bin
