open pro.c
gcc -o pro pro.c `pkg-config --cflags --libs gtk+-3.0`
./pro

