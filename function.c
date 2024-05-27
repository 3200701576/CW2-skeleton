#include "maze.h"

/**
 * @brief Initialise a maze object - allocate memory and set attributes
 * @param maze pointer to the maze to be initialised
 * @param height height to allocate
 * @param width width to allocate
 * @return int 0 on success, 1 on fail  // 返回1？？？？？？？
 */
int create_maze(maze *maze, int height, int width)
{

    maze->height = height;
    maze->width = width;
    maze->map = (char **)malloc(height * sizeof(char *));

    if (maze->map == NULL)
    {
        return 1; // 内存分配失败
    }

    for (int i = 0; i < height; i++)
    {
        maze->map[i] = (char *)malloc(width * sizeof(char)); // 为每一行分配空间

        if (maze->map[i] == NULL)
        {
            // 如果某行分配失败，释放已分配的内存并返回1
            for (int j = 0; j < i; j++)
            {
                free(maze->map[j]);
            }
            free(maze->map);
            return 1;
        }
    }

    return 0;
}

/**
 * @brief Free the memory allocated to the maze struct
 *
 * @param maze the pointer to the struct to free
 */
void free_maze(maze *maze)
{
    for (int i = 0; i < maze->height; i++)
    {
        free(maze->map[i]);
    }
    free(maze->map);
}

/**
 * @brief Validate and return the width of the mazefile
 *
 * @param file the file pointer to check
 * @return int 0 for error, or a valid width (5-100)  规则是不是要改
 */
int get_width(FILE *file)
{
    // int width;
    // // Read the width from the file
    //
    // printf("width %d\n", width);
    // return width;
    fseek(file, 0, SEEK_SET);
    int width = 0;
    char c;
    while ((c = fgetc(file)) != EOF)
    {
        if (c != '\n')
        {
            width++;
        }
        else
        {
            break;
        }
    }

    return width;
}

/**
 * @brief Validate and return the height of the mazefile
 *
 * @param file the file pointer to check
 * @return int 0 for error, or a valid height (5-100)
 */
int get_height(FILE *file)
{
    fseek(file, 0, SEEK_SET);
    int height = 0;
    // int good_line = 0;
    char last_char;
    char c;
    while ((c = fgetc(file)) != EOF)
    {
        if (c == '\n' && last_char != '\n')
        {
            height++;
        }
        last_char = c;
    }
    if (last_char != '\n')
    {
        height++;
    }
    return height;
}

/**
 * @brief read in a maze file into a struct
 *
 * @param maze Maze struct to be used
 * @param file Maze file pointer
 * @return int 0 on success, 1 on fail
 */
int read_maze(maze *maze, FILE *file)
{
    fseek(file, 0, SEEK_SET);
    char c;
    int i, j;
    int has_s = 0;
    int has_e = 0;

    for (i = 0; i < maze->height; i++)
    {
        for (j = 0; j < MAX_DIM; j++)
        {
            if (fscanf(file, "%c", &maze->map[i][j]) != 1)
            {
                if (feof(file))
                {
                    break;
                }
                else
                {
                    return 1; // Error reading the maze
                }
            }

            if (maze->map[i][j] == 'S')
            {
                has_s += 1;
                maze->start.x = j;
                maze->start.y = i;
            }
            else if (maze->map[i][j] == 'E')
            {
                has_e += 1;
                maze->end.x = j;
                maze->end.y = i;
            }
            else if (maze->map[i][j] == '\n')
            {
                if (j == (maze->width))
                {
                    break;
                }
                else
                {
                    // printf("maze %d\n", j);
                    printf("333");
                    return 1;
                }
            }
            else if (maze->map[i][j] != ' ' && maze->map[i][j] != '#')
            {
                // printf("%c", c);
                printf("222222");
                return 1; // Invalid character in the maze
            }
        }
    }
    if (fscanf(file, "%c", &maze->map[i][j]) == 1)
    {
        return 1;
    }

    if (has_e != 1 || has_s != 1)
    {
        printf("44444");
        return 1;
    }
    return 0; // Success
}

/**
 * @brief Prints the maze out - code provided to ensure correct formatting
 *
 * @param maze pointer to maze to print
 * @param player the current player location
 */
void print_maze(maze *maze, coord *player)
{
    // make sure we have a leading newline..
    printf("\n");
    for (int i = 0; i < maze->height; i++)
    {
        for (int j = 0; j < maze->width; j++)
        {
            // decide whether player is on maze spot or not
            if (player->x == j && player->y == i)
            {
                printf("X");
            }
            else
            {
                printf("%c", maze->map[i][j]);
            }
        }
        // end each row with a newline.
        printf("\n");
    }
    printf("\n");
}

void player_location(coord *player)
{
    printf("%d,%d\n", player->x, player->y);
}

int is_wall(maze *maze, coord *player)
{
    if (maze->map[player->y][player->x] == '#')
    {
        printf("Hit the wall\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Validates and performs a movement in a given direction
 *
 * @param maze Maze struct
 * @param player The player's current position
 * @param direction The desired direction to move in
 */
int move(maze *maze, coord *player, char direction)
{

    // if (length != 1)
    // {
    //     return 1;
    // }
    direction = toupper(direction);
    // 根据方向更新玩家的坐标
    switch (direction)
    {
    case 'W': // 向上

        if (player->y > 0)
        {
            player->y -= 1;
            if (is_wall(maze, player) != 0)
            {
                player->y += 1;
            }
        }
        else
        {
            printf("Movement out\n");
        }
        break;
    case 'S': // 向下
        if (player->y < (maze->height - 1))
        {
            player->y += 1;
            if (is_wall(maze, player) != 0)
            {
                player->y -= 1;
            }
        }
        else
        {
            printf("Movement out\n");
        }
        break;

    case 'A':
        if (player->x > 0)
        {
            player->x -= 1;
            if (is_wall(maze, player) != 0)
            {
                player->x += 1;
            }
        }
        else
        {
            printf("Movement out\n");
        }
        break;
    case 'D':
        if (player->x < (maze->width - 1))
        {
            player->x += 1;
            if (is_wall(maze, player) != 0)
            {
                player->x -= 1;
            }
        }
        else
        {
            printf("Movement out\n");
        }
        break;
    case 'M':
        print_maze(maze, player);
        break;
    case 'X':
        player_location(player);
        break;
    case 'Q':
        return 1;
    default:
        printf("Invalid input\n");
        // return 1;
    }
    return 0;
}

/**
 * @brief Check whether the player has won and return a pseudo-boolean
 *
 * @param maze current maze
 * @param player player position
 * @return int 0 for false, 1 for true
 */
int has_won(maze *maze, coord *player)
{
    return (player->x == maze->end.x && player->y == maze->end.y);
}