#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Node defination

typedef struct Node {
    int x;
    int y;
    struct Node* next;
} Node;

// Create a new Node

Node* createNode() {
    Node* newNode = (Node*)malloc(sizeof(Node));
    return newNode;
}

// Print List 

void printList(Node* head) {
    printf("\n");
    while(head != NULL) {
        printf(" (%d, %d) ", head->x, head->y);
        head = head->next;
    }
    printf("\n");
}

// Print the board

void displayBoard(char** board, int m, int n) {
    for(int i = 0; i <= m; i++) printf("##");

    for(int i = 0; i < m; i++) {
        printf("\n#");
        for(int j = 0; j < n; j++) {
            printf("%c ", board[i][j]);
        }
        printf("#");
    }
    printf("\n");
    for(int i = 0; i <= m; i++) printf("##");
}

// Check if present on snake or not. 0 - No, 1 - Yes.

int checkOnSnake(Node* head, int x, int y) {
    while(head != NULL) {
        if(head->x == x && head->y == y) {
            return 1;
        }
        head = head->next;
    }
    return 0;
}

// Generate Fruit

void generateFruit(char** board, int m, int n, Node* head) {
    time_t t;
    srand((unsigned) time(&t));
    int x = 0;
    int y = 0;
    do {
       x = rand() % m;
       y = rand() % n; 
        
    }while(checkOnSnake(head, x, y));
    board[x][y] = 'O';
}

// Initalize the board.

void initBoard(char** board, int m, int n) {
    for(int i = 0 ; i < m ; i++) {
        *(board + i) = (char*)calloc(n, sizeof(char));
    }
    for(int i = 0 ; i < m ; i++) {
        for(int j = 0 ; j < n ; j++) {
            board[i][j] = ' ';
        }
    }
}

//Intialize the snake

void initSnake(char** board, Node* head, int length) {
    head->x = 0;
    head->y = 0;
    board[0][0] = '@';
    for(int i = 1 ; i < length; i++) {
        head->next = createNode();
        head = head-> next;
        head->x = 0;
        head->y = i;
        if(i == length -1)
            board[0][i] = '*';
        else
            board[0][i] = '=';
    }
}

// Remove last node

void removeLastNode(Node* head, char** board) {
    while(head->next->next != NULL) {
        head= head->next;
    }
    board[head->next->x][head->next->y] = ' ';
    board[head->x][head->y] = '*';
    head->next = NULL;
}

// Move helper

void helper(char** board, Node* head, int x1 , int y1) {
    board[head->x][head->y] = '=';
    Node* newNode = createNode();
    newNode->x = head->x;
    newNode->y = head->y;
    newNode->next = head-> next;
    head->next = newNode;
    head->x = x1;
    head->y = y1;
}

// Move the snake

int moveSnake(char** board, int m, int n, char move, Node* head) {
    int x = 0 , y = 0;
    switch(move) {
        case 'w':
            x = head->x == 0 ? m - 1 : head->x - 1;
            y = head->y;
            break;
        case 'a':
            x = head->x;
            y = head->y == 0 ? n - 1 : head->y - 1;
            break;
        case 's':
            x = (head->x + 1) % m;
            y = head->y;
            break;
        case 'd':
            x = head->x;
            y = (head->y + 1) % m;
            break;
        default:
            return 0;               
    }
    if(x == head->next->x && y == head->next->y) return 0;    
    helper(board,head, x ,y);
    int score =  board[x][y] == 'O' ? 1 : 0;
    if(!score) removeLastNode(head, board);
    else generateFruit(board, m, n, head);
    board[x][y] = '@';
    if(checkOnSnake(head->next, x, y)) return -1;
    return score;
}

// Driver code

int main()
{
    char restart = 'n'; 
    do{
        system("clear");
        int m = 15, n = 15;
        int score = 0;
        char** board = (char**)calloc(m, sizeof(char*)); // Create Board..
        initBoard(board, m, n);    
        Node* head = createNode();
        initSnake(board, head, 4);
        generateFruit(board, m, n, head);
        char move = 'A';
        do {
            printf("\n@ -head, * - tail\n");
            printf("\nBoundaires are safe! They teleport you :P\n\n");
            displayBoard(board, m, n);
            //printList(head);
            printf("\n\nw - Up, a - Left, d - Right, s - Down, q - Quit\n");
            scanf(" %c", &move);
            system("clear");
            if(move != 'q') {
                int res = moveSnake(board, m, n, move, head);
                if(res == -1) {
                    printf("Yikes! You Ate Yourself!\n");
                    break;
                }
                score += res;
            }
        }while(move != 'q');
        if(move == 'q') printf("You Quit!\n");
        printf("Your score is : %d ", score);

        printf("\n\nWant to Play Again? press r to restart!\n");
        scanf(" %c", &restart);
    }while(restart == 'r');
    

    return 0;
}
