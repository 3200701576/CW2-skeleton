#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
// #include "maze.h"

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
 * @param this pointer to the maze to be initialised
 * @param height height to allocate
 * @param width width to allocate
 * @return int 0 on success, 1 on fail  // 返回1？？？？？？？
 */
int create_maze(maze *this, int height, int width)
{

    this->height = height;
    this->width = width;
    this->map = (char **)malloc(height * sizeof(char *));

    if (this->map == NULL)
    {
        return 1; // 内存分配失败
    }

    for (int i = 0; i < height; i++)
    {
        this->map[i] = (char *)malloc(width * sizeof(char)); // 为每一行分配空间

        if (this->map[i] == NULL)
        {
            // 如果某行分配失败，释放已分配的内存并返回1
            for (int j = 0; j < i; j++)
            {
                free(this->map[j]);
            }
            free(this->map);
            return 1;
        }
    }

    return 0;
}

/**
 * @brief Free the memory allocated to the maze struct
 *
 * @param this the pointer to the struct to free
 */
void free_maze(maze *this)
{
    for (int i = 0; i < this->height; i++)
    {
        free(this->map[i]);
    }
    free(this->map);
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
 * @param this Maze struct to be used
 * @param file Maze file pointer
 * @return int 0 on success, 1 on fail
 */
int read_maze(maze *this, FILE *file)
{
    fseek(file, 0, SEEK_SET);
    char c;
    int i, j;
    int has_s = 0;
    int has_e = 0;

    for (i = 0; i < this->height; i++)
    {
        for (j = 0; j < MAX_DIM; j++)
        {
            if (fscanf(file, "%c", &this->map[i][j]) != 1)
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

            if (this->map[i][j] == 'S')
            {
                has_s += 1;
                this->start.x = j;
                this->start.y = i;
            }
            else if (this->map[i][j] == 'E')
            {
                has_e += 1;
                this->end.x = j;
                this->end.y = i;
            }
            else if (this->map[i][j] == '\n')
            {
                if (j == (this->width))
                {
                    break;
                }
                else
                {
                    // printf("this %d\n", j);
                    printf("333");
                    return 1;
                }
            }
            else if (this->map[i][j] != ' ' && this->map[i][j] != '#')
            {
                // printf("%c", c);
                printf("222222");
                return 1; // Invalid character in the maze
            }
        }
    }
    if (fscanf(file, "%c", &this->map[i][j]) == 1)
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
 * @param this pointer to maze to print
 * @param player the current player location
 */
void print_maze(maze *this, coord *player)
{
    // make sure we have a leading newline..
    printf("\n");
    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width; j++)
        {
            // decide whether player is on this spot or not
            if (player->x == j && player->y == i)
            {
                printf("X");
            }
            else
            {
                printf("%c", this->map[i][j]);
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

int is_wall(maze *this, coord *player)
{
    if (this->map[player->y][player->x] == '#')
    {
        printf("Hit the wall");
        return 1;
    }
    return 0;
}

/**
 * @brief Validates and performs a movement in a given direction
 *
 * @param this Maze struct
 * @param player The player's current position
 * @param direction The desired direction to move in
 */
int move(maze *this, coord *player, char direction)
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
            if (is_wall(this, player) != 0)
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
        if (player->y < (this->height - 1))
        {
            player->y += 1;
            if (is_wall(this, player) != 0)
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
            if (is_wall(this, player) != 0)
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
        if (player->x < (this->width - 1))
        {
            player->x += 1;
            if (is_wall(this, player) != 0)
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
        print_maze(this, player);
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
 * @param this current maze
 * @param player player position
 * @return int 0 for false, 1 for true
 */
int has_won(maze *this, coord *player)
{
    return (player->x == this->end.x && player->y == this->end.y);
}

int main(int argc, char *argv[])
{

    // check args
    if (argc != 2)
    {
        printf("Error: Incorrect number of input parameters.\n");
        return EXIT_ARG_ERROR;
    }
    // set up some useful variables (you can rename or remove these if you want)
    FILE *f;

    maze Maze;
    // 判断文件是否能打开
    if (f == NULL)
    {
        printf("Error: Unable to open file.\n");
        return EXIT_FILE_ERROR;
    }
    else
    {
        f = fopen(argv[1], "r");
    }

    int width = get_width(f);
    int height = get_height(f);
    // int width = 15;
    // int height = 9;

    if (width < MIN_DIM || width > MAX_DIM || height < MIN_DIM || height > MAX_DIM)
    {
        fclose(f);
        printf("Maze is wrong.");
        return EXIT_MAZE_ERROR;
    }

    printf("width:  %d\n", width);
    printf("height : %d\n", height);

    // open and validate mazefile
    if (create_maze(&Maze, height, width) != 0)
    {
        printf("Error: Maze creation failed.\n");
        fclose(f);
        free_maze(&Maze);
        return 100;
    }

    // read in mazefile to struct
    if (read_maze(&Maze, f) != 0)
    {
        printf("Error: Maze is wrong.\n");
        fclose(f);
        free_maze(&Maze);
        return EXIT_MAZE_ERROR;
    }

    coord player;
    player.x = Maze.start.x;
    player.y = Maze.start.y;

    // maze game loop
    while (!has_won(&Maze, &player))
    {
        printf("Enter movement (WASD): ");
        char movement;
        scanf(" %c", &movement);
        // 读取并丢弃换行符
        char c = getchar();
        if (c != '\n') // 如果下一个字符不是换行符，说明输入了多个字符
        {
            printf("Error: Too many characters. Please enter only one character (WASD).\n");
            continue;
        }
        // printf("ddd %s\n", movement);
        if (move(&Maze, &player, movement) != 0)
        {
            break;
        };
    }

    if (has_won(&Maze, &player))
    {
        printf("Congratulations! You've won!\n");
    }

    fclose(f);
    free_maze(&Maze);

    return EXIT_SUCCESS;
}