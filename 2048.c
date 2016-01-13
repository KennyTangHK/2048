#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
Only variable with const keyword can be use in array declaration
EG
const int CONSTANT = 5;
int array[CONSTANT]; will work.
int variable = 5;
int array[variable]; will not work.
*/
const int HISTORY_SIZE = 50; /* Too big will lead to compile error. */
const int MAP_SIZE = 4;

const int PERCENTAGE_OF_GETTING_4 = 20;

const int LEFT = 1;
const int RIGHT = 2;
const int UP = 3;
const int DOWN = 4;
const int UNDO = 5;
const int EXIT = 6;

/* See vertially, all index 0 means LEFT, index 1 means RIGHT ... */
const int init_r[4] = {0, 0, 0, MAP_SIZE - 1}; /* We start at this point, then */
const int init_c[4] = {0, MAP_SIZE - 1, 0, 0};
const int look_r[4] = {0, 0, 1, -1}; /* look into this position to remove empty or merge, */
const int look_c[4] = {1, -1, 0, 0};
const int move_r[4] = {1, 1, 0, 0}; /* after merging a line, go to this position */
const int move_c[4] = {0, 0, 1, 1};

/*
int have_move(int map[MAP_SIZE][MAP_SIZE])
Check if the map have space or mergable cells.
Return 1 if yes, 0 if no.
*/
int have_move(int map[MAP_SIZE][MAP_SIZE]) {
	int i, j;

	for (i = 0; i < MAP_SIZE; i++) {
		for (j = 0; j < MAP_SIZE; j++) {
			if (map[i][j] == 0 || (i+1 < MAP_SIZE && map[i][j] == map[i+1][j]) || (j+1 < MAP_SIZE && map[i][j] == map[i][j+1])) {
				return 1;
			}
		}
	}

	return 0;
}

/*
void copy_map(int new_map[MAP_SIZE][MAP_SIZE], int old_map[MAP_SIZE][MAP_SIZE])
Copy the second map to the first one.
*/
void copy_map(int new_map[MAP_SIZE][MAP_SIZE], int old_map[MAP_SIZE][MAP_SIZE]) {
	int i, j;
	for (i = 0; i < MAP_SIZE; i++) {
		for (j = 0; j < MAP_SIZE; j++) {
			new_map[i][j] = old_map[i][j];
		}
	}
}

/*
int compare_map(int new_map[MAP_SIZE][MAP_SIZE], int old_map[MAP_SIZE][MAP_SIZE])
Compare the maps.
Return 1 if same, 0 if not.
*/
int compare_map(int new_map[MAP_SIZE][MAP_SIZE], int old_map[MAP_SIZE][MAP_SIZE]) {
	int i, j;
	for (i = 0; i < MAP_SIZE; i++) {
		for (j = 0; j < MAP_SIZE; j++) {
			if (new_map[i][j] != old_map[i][j]) return 0;
		}
	}
	return 1;
}

/*
void generate_cell(int map[MAP_SIZE][MAP_SIZE])
Generate a 2 or 4 on a empty cell of the map.
A full map will lead to infinity loop.
*/
void generate_cell(int map[MAP_SIZE][MAP_SIZE]) {
	int rand_r, rand_c, rand_num = rand() % 100;

	do {
		rand_r = rand() % MAP_SIZE;
		rand_c = rand() % MAP_SIZE;
	} while (map[rand_r][rand_c] != 0);

	if (rand_num < PERCENTAGE_OF_GETTING_4) {
		map[rand_r][rand_c] = 4;
	} else {
		map[rand_r][rand_c] = 2;
	}
}

/*
void do_merge(int array[MAP_SIZE])
Remove all spaces and merge once. Add spaces to the end.
*/
void do_merge(int array[MAP_SIZE]) {
	int i, count1 = 0, count2 = 0;

	/* Remove spaces and get the new length count1 */
	for (i = 0; i < MAP_SIZE; i++) {
		if (array[i] > 0) array[count1++] = array[i];
	}

	/* Merge the lines */
	for (i = 0; i < count1 - 1; i++) {
		if (array[i] == array[i+1]) {
			array[i] *= 2;
			array[i+1] = 0;
		}
	}

	/* Remove spaces again and get the new length count2 */
	for (i = 0; i < count1; i++) {
		if (array[i] > 0) array[count2++] = array[i];
	}

	/* Add space to the end */
	for (i = count2; i < MAP_SIZE; i++) {
		array[i] = 0;
	}
}

/*
void do_command(int map[MAP_SIZE][MAP_SIZE], int cmd)
Transversal the map by cmd. Hand lines to do_merge and replace the old map.
*/
void do_command(int map[MAP_SIZE][MAP_SIZE], int cmd) {
	if (cmd == LEFT) cmd = 0;
	if (cmd == RIGHT) cmd = 1;
	if (cmd == UP) cmd = 2;
	if (cmd == DOWN) cmd = 3;

	int line[MAP_SIZE];
	int i, j, k, l, count;

	/* We start at this point, then */
	i = init_r[cmd];
	j = init_c[cmd];

	while (i >= 0 && i < MAP_SIZE && j >= 0 && j < MAP_SIZE) {
		k = i;
		l = j;
		count = 0;

		/* look into this position, put it into line */
		while (k >= 0 && k < MAP_SIZE && l >= 0 && l < MAP_SIZE) {
			line[count++] = map[k][l];
			k += look_r[cmd];
			l += look_c[cmd];
		}

		/* merge it */
		do_merge(line);

		/* Restore to the start of this line */
		k = i;
		l = j;
		count = 0;

		/* replace the line by the merged line */
		while (k >= 0 && k < MAP_SIZE && l >= 0 && l < MAP_SIZE) {
			map[k][l] = line[count++];
			k += look_r[cmd];
			l += look_c[cmd];
		}

		/* after merging a line, go to this position */
		i += move_r[cmd];
		j += move_c[cmd];
	}
}

void print_map(int step, int map[MAP_SIZE][MAP_SIZE]) {
	int i, j;

	printf("\nSTEP %d\n", step);

	for (i = 0; i < MAP_SIZE; i++) {
		for (j = 0; j < MAP_SIZE; j++) {
			printf("|-----");
		}
		printf("|\n");

		for (j = 0; j < MAP_SIZE; j++) {
			printf("|%5d", map[i][j]);
		}
		printf("|\n");
	}

	for (i = 0; i < MAP_SIZE; i++) {
		printf("|-----");
	}
	printf("|\n\n");
}

int get_command() {
	int cmd;
	while (1) {
		printf("Command\n[%d] LEFT, [%d] RIGHT, [%d] UP, [%d] DOWN, [%d] UNDO, [%d] EXIT\n", LEFT, RIGHT, UP, DOWN, UNDO, EXIT);
		scanf("%d", &cmd);

		if (cmd == LEFT || cmd == RIGHT || cmd == UP || cmd == DOWN || cmd == UNDO || cmd == EXIT) {
			return cmd;
		}

		printf("[Error] Invaild command.\n");
	}
}

int main() {
	srand(time(NULL));

	int map_stack[HISTORY_SIZE][MAP_SIZE][MAP_SIZE];
	int temp_map[MAP_SIZE][MAP_SIZE]; /* to check cmd with no change */
	int i, j, step = 0, oldest_step = 0; /* the current step and the oldest step that can be undo to */
	int supress = 0; /* set to 1 will stop generating new cell */

	for (i = 0; i < MAP_SIZE; i++) {
		for (j = 0; j < MAP_SIZE; j++) {
			map_stack[step][i][j] = 0;
		}
	}

	while (1) {
		if (supress == 0) {
			generate_cell(map_stack[step % HISTORY_SIZE]);
			supress = 0;
		}

		/* Copy the map to a new layer, the old one will be history */
		copy_map(map_stack[(step+1) % HISTORY_SIZE], map_stack[step % HISTORY_SIZE]);
		step++;

		/* Maintain the oldest_step */
		if (step - HISTORY_SIZE > oldest_step) oldest_step = step - HISTORY_SIZE;
		if (oldest_step < 0) oldest_step = 0;

		print_map(step, map_stack[step % HISTORY_SIZE]);

		if (!have_move(map_stack[step % HISTORY_SIZE])) {
			printf("No more move at step %d.\n", step);
			break;
		}

		int cmd = get_command();

		switch (cmd) {
			case LEFT: case RIGHT: case UP: case DOWN: /* if (cmd == LEFT || cmd == RIGHT ...) { */
				copy_map(temp_map, map_stack[step % HISTORY_SIZE]);
				do_command(map_stack[step % HISTORY_SIZE], cmd);

				if (compare_map(temp_map, map_stack[step % HISTORY_SIZE]) == 1) {
					printf("[Warn] Command made no different.\n");
					step--;
					supress = 1;
				}

				break; /* break switch, not while */
			case UNDO: /* if (cmd == UNDO) { */
				if (step - 2 < oldest_step) {
					printf("[Error] No more history (Max %d).\n", HISTORY_SIZE);
					step -= 1;
				} else {
					step -= 2;
				}
				supress = 1;

				break; /* break switch, not while */
		}

		if (cmd == EXIT) break;
	}

	printf("Goodbye.\n");

	return 0;
}