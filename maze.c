#include "maze.h"

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
