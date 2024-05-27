#ifndef MAZE_H
#define MAZE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// defines for max and min permitted dimensions
#define MAX_DIM 100
#define MIN_DIM 5

// defines for the required autograder exit codes
#define EXIT_SUCCESS 0
#define EXIT_ARG_ERROR 1
#define EXIT_FILE_ERROR 2
#define EXIT_MAZE_ERROR 3

typedef struct __Coord
{
    int x;
    int y;
} coord;

typedef struct __Maze
{
    char **map;
    int height;
    int width;
    coord start;
    coord end;
} maze;

/**
 * @brief Initialise a maze object - allocate memory and set attributes
 * @param maze pointer to the maze to be initialised
 * @param height height to allocate
 * @param width width to allocate
 * @return int 0 on success, 1 on fail  // 返回1？？？？？？？
 */
int create_maze(maze *maze, int height, int width);

/**
 * @brief Free the memory allocated to the maze struct
 *
 * @param maze the pointer to the struct to free
 */
void free_maze(maze *maze);

/**
 * @brief Validate and return the width of the mazefile
 *
 * @param file the file pointer to check
 * @return int 0 for error, or a valid width (5-100)  规则是不是要改
 */
int get_width(FILE *file);

/**
 * @brief Validate and return the height of the mazefile
 *
 * @param file the file pointer to check
 * @return int 0 for error, or a valid height (5-100)
 */
int get_height(FILE *file);

/**
 * @brief read in a maze file into a struct
 *
 * @param maze Maze struct to be used
 * @param file Maze file pointer
 * @return int 0 on success, 1 on fail
 */
int read_maze(maze *maze, FILE *file);

/**
 * @brief Prints the maze out - code provided to ensure correct formatting
 *
 * @param maze pointer to maze to print
 * @param player the current player location
 */
void print_maze(maze *maze, coord *player);

void player_location(coord *player);

int is_wall(maze *maze, coord *player);

/**
 * @brief Validates and performs a movement in a given direction
 *
 * @param maze Maze struct
 * @param player The player's current position
 * @param direction The desired direction to move in
 */
int move(maze *maze, coord *player, char direction);

/**
 * @brief Check whether the player has won and return a pseudo-boolean
 *
 * @param maze current maze
 * @param player player position
 * @return int 0 for false, 1 for true
 */
int has_won(maze *maze, coord *player);

#endif