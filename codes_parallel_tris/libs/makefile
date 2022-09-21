FLAGS =-o3
test_lea: test_lea.o libattopng.o lea_png.o
	gcc $^ -o $@

%.o: %.c
	gcc -c $^ -o $@

clean:
	rm -rvf *.o test_*.png test_lea