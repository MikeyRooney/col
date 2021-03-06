#include <stdio.h>
#include <stdlib.h>

#include "grid.h"

static FILE *data_file;

// First writes the current iteration as well as the simulation timestamp.
// TODO: radius and mass will stay constant so can save space by writing them 
// once at the start of the binary file
static void save_sphere_state_to_file(uint64_t iteration_num, double time_elapsed) {
	fwrite(&iteration_num, sizeof(uint64_t), 1, data_file);
	fwrite(&time_elapsed, sizeof(double), 1, data_file);
	int i;
	for (i = 0; i < NUM_SPHERES; i++) {
		fwrite(&spheres[i].vel.x, sizeof(double), 1, data_file);
		fwrite(&spheres[i].vel.y, sizeof(double), 1, data_file);
		fwrite(&spheres[i].vel.z, sizeof(double), 1, data_file);
		fwrite(&spheres[i].pos.x, sizeof(double), 1, data_file);
		fwrite(&spheres[i].pos.y, sizeof(double), 1, data_file);
		fwrite(&spheres[i].pos.z, sizeof(double), 1, data_file);
		fwrite(&spheres[i].radius, sizeof(double), 1, data_file);
		fwrite(&spheres[i].radius, sizeof(double), 1, data_file);
	}
}

// First write the grid's dimensions.
// Then write the initial state of the spheres.
// The iteration number and the time elapsed are 0 as nothing has
// happened yet.
// TODO: probably need an id for each sphere
static void init_binary_file() {
	data_file = fopen("data.bin", "wb");
	fwrite(&grid->x_start, sizeof(double), 1, data_file);
	fwrite(&grid->x_end, sizeof(double), 1, data_file);
	fwrite(&grid->y_start, sizeof(double), 1, data_file);
	fwrite(&grid->y_end, sizeof(double), 1, data_file);
	fwrite(&grid->z_start, sizeof(double), 1, data_file);
	fwrite(&grid->z_end, sizeof(double), 1, data_file);
	save_sphere_state_to_file(0, 0.0);
}

void simulation_init() {
	init_grid();
	init_binary_file();
}

void simulation_run() {
	double time_elapsed = 0;
	int i;
	for (i = 1; i < 10; i++) { // start at 1 as 0 is iteration num for the initial state
		time_elapsed += update_grid();
		save_sphere_state_to_file(i, time_elapsed);
	}
}

void simulation_cleanup() {
	fclose(data_file);
	free(grid);
}