Simple 2048 in C
================

This is a simple 2048 written in C.

Features
--------

*Multiple undo (Rotate history).
*Customizable map size using constant.
*Customizable percentage of getting 4 using constant.

Functions
---------

### `int have_move(int map[MAP_SIZE][MAP_SIZE])`

Check if the map have space or mergable cells.
Return 1 if yes, 0 if no.

### `void copy_map(int new_map[MAP_SIZE][MAP_SIZE], int old_map[MAP_SIZE][MAP_SIZE])`

Copy the second map to the first one.

### `int compare_map(int new_map[MAP_SIZE][MAP_SIZE], int old_map[MAP_SIZE][MAP_SIZE])`

Compare the maps.
Return 1 if same, 0 if not.

### `void generate_cell(int map[MAP_SIZE][MAP_SIZE])`

Generate a 2 or 4 on a empty cell of the map.
A full map will lead to infinity loop.

### `void do_merge(int array[MAP_SIZE])`

Remove all spaces and merge once. Add spaces to the end.

### `void do_command(int map[MAP_SIZE][MAP_SIZE], int cmd)`

Transversal the map by cmd. Hand lines to do_merge and replace the old map.

### `void print_map(int step, int map[MAP_SIZE][MAP_SIZE])`

Print the map with step number.

### `int get_command()`

Get valid command.
Return command code in integer.

License
-------

[MIT](LICENSE)