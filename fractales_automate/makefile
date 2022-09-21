FLAGS =-o3 -lm #-Wall #-Wextra #-fsanitize=leak -fsanitize=address -std=gnu11 

# /usr/bin/mpicc 

fractal_mpi: parallelized_fractal.o lea_png.o libattopng.o create_matrix.o
	/usr/bin/mpicc $^ $(FLAGS) -o $@

parity: libattopng.o lea_png.o create_matrix.o parity.o
	gcc $^ $(FLAGS) -o $@

parity_mpi: libattopng.o lea_png.o create_matrix.o parallelized_cellular_automate.o
	/usr/bin/mpicc $^ $(FLAGS) -o $@
	
GOL_mpi: libattopng.o lea_png.o create_matrix.o parallelized_GOL_optimized.o
	/usr/bin/mpicc $^ $(FLAGS) -o $@

pairimpair: create_vector.o pair_impair.o
	gcc $^ $(FLAGS) -o $@

tri_fusion: create_vector.o tri_fusion.o
	gcc $^ $(FLAGS) -o $@
	
fractal: fractal.o lea_png.o libattopng.o create_matrix.o 
	gcc $^ $(FLAGS) -o $@

fractal_comp: fractal_with_complex.o lea_png.o libattopng.o create_matrix.o 
	gcc $^ $(FLAGS) -o $@

merge_sort: parallelized_merge_sort.o 
	/usr/bin/mpicc $^ $(FLAGS) -o $@

%.gif: video/%_0.png
	$(eval IMGS := $(shell ls video/$(basename $@)_*.png | sort -V | xargs))
	mkdir -p ./video/tmp
	@for f in $(notdir $(IMGS)); do convert ./video/$$f ./video/tmp/$$f; done
	convert -delay 0 -loop 0 $(shell ls video/tmp/$(basename $@)_*.png | sort -V | xargs) $@
	rm -rf ./video/tmp


%.o: %.c
	/usr/bin/mpicc -c $^ -o $@

clean:
	rm -rvf *.o parity 
