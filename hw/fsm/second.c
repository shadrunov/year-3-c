#include <stdio.h>
#include <stdlib.h>

/*
 * single row of FSM:
 * contains current state, input (a coin),
 * next state and output (the change)
 */
typedef struct row
{
    int state;      // states from 0 to 8
    int insert;     // an inserted coin (1, 2, 5, 10)
    int next_state; // state after coin
    int change[4];  // should a machine return 1, 2, 2x2 or 5 coin of change
    int item;       // an item to give (1 or 2)
} row;

// manually add rows to table
void create_table(row *pointer)
{
    pointer[0] = (struct row){0, 1, 10, {0, 0, 0, 0}, 0};
    pointer[1] = (struct row){0, 2, 20, {0, 0, 0, 0}, 0};
    pointer[2] = (struct row){10, 1, 11, {0, 0, 0, 0}, 0};
    pointer[3] = (struct row){10, 2, 12, {0, 0, 0, 0}, 0};
    pointer[4] = (struct row){10, 5, 15, {0, 0, 0, 0}, 0};
    pointer[5] = (struct row){10, 10, 0, {1, 0, 0, 0}, 1};
    pointer[6] = (struct row){11, 1, 12, {0, 0, 0, 0}, 0};
    pointer[7] = (struct row){11, 2, 13, {0, 0, 0, 0}, 0};
    pointer[8] = (struct row){11, 5, 16, {0, 0, 0, 0}, 0};
    pointer[9] = (struct row){11, 10, 0, {0, 1, 0, 0}, 1};
    pointer[10] = (struct row){12, 1, 13, {0, 0, 0, 0}, 0};
    pointer[11] = (struct row){12, 2, 14, {0, 0, 0, 0}, 0};
    pointer[12] = (struct row){12, 5, 17, {0, 0, 0, 0}, 0};
    pointer[13] = (struct row){12, 10, 0, {1, 1, 0, 0}, 1};
    pointer[14] = (struct row){13, 1, 14, {0, 0, 0, 0}, 0};
    pointer[15] = (struct row){13, 2, 15, {0, 0, 0, 0}, 0};
    pointer[16] = (struct row){13, 5, 18, {0, 0, 0, 0}, 0};
    pointer[17] = (struct row){13, 10, 0, {0, 0, 1, 0}, 1};
    pointer[18] = (struct row){14, 1, 15, {0, 0, 0, 0}, 0};
    pointer[19] = (struct row){14, 2, 16, {0, 0, 0, 0}, 0};
    pointer[20] = (struct row){14, 5, 0, {0, 0, 0, 0}, 1};
    pointer[21] = (struct row){14, 10, 14, {0, 0, 0, 1}, 0};
    pointer[22] = (struct row){15, 1, 16, {0, 0, 0, 0}, 0};
    pointer[23] = (struct row){15, 2, 17, {0, 0, 0, 0}, 0};
    pointer[24] = (struct row){15, 5, 0, {1, 0, 0, 0}, 1};
    pointer[25] = (struct row){15, 10, 0, {1, 0, 0, 1}, 1};
    pointer[26] = (struct row){16, 1, 17, {0, 0, 0, 0}, 0};
    pointer[27] = (struct row){16, 2, 18, {0, 0, 0, 0}, 0};
    pointer[28] = (struct row){16, 5, 0, {0, 1, 0, 0}, 1};
    pointer[29] = (struct row){16, 10, 0, {0, 1, 0, 1}, 1};
    pointer[30] = (struct row){17, 1, 18, {0, 0, 0, 0}, 0};
    pointer[31] = (struct row){17, 2, 0, {1, 0, 0, 0}, 1};
    pointer[32] = (struct row){17, 5, 0, {1, 1, 0, 0}, 1};
    pointer[33] = (struct row){17, 10, 0, {1, 1, 0, 1}, 1};
    pointer[34] = (struct row){18, 1, 0, {0, 0, 0, 0}, 1};
    pointer[35] = (struct row){18, 2, 0, {1, 0, 0, 0}, 1};
    pointer[36] = (struct row){18, 5, 0, {0, 0, 1, 0}, 1};
    pointer[37] = (struct row){18, 10, 0, {0, 0, 1, 1}, 1};
    pointer[38] = (struct row){20, 1, 21, {0, 0, 0, 0}, 0};
    pointer[39] = (struct row){20, 2, 22, {0, 0, 0, 0}, 0};
    pointer[40] = (struct row){20, 5, 0, {0, 1, 0, 0}, 2};
    pointer[41] = (struct row){20, 10, 0, {0, 1, 0, 1}, 2};
    pointer[42] = (struct row){21, 1, 22, {0, 0, 0, 0}, 0};
    pointer[43] = (struct row){21, 2, 0, {0, 0, 0, 0}, 2};
    pointer[44] = (struct row){21, 5, 0, {1, 1, 0, 0}, 2};
    pointer[45] = (struct row){21, 10, 0, {1, 1, 0, 1}, 2};
    pointer[46] = (struct row){22, 1, 0, {0, 0, 0, 0}, 2};
    pointer[47] = (struct row){22, 2, 0, {1, 0, 0, 0}, 2};
    pointer[48] = (struct row){22, 5, 0, {0, 0, 1, 0}, 2};
    pointer[49] = (struct row){22, 10, 0, {1, 1, 1, 1}, 2};
}

int main()
{
    // init FSM with precalculated values
    int states_num = 50;
    row *fsm = malloc(sizeof(row) * states_num);
    create_table(fsm);

    // start a machine
    printf("Price of first item:   9 \n");
    printf("Price of second item:  3 \n\n");

    int cur_state = 0;
    int input;

    while (1)
    {
        // new iteration
        printf("\nCurrent state: %d\n", cur_state);
        if (cur_state == 0)
            printf("Select item (1 or 2)? \n");
        else
            printf("Enter coin (1, 2, 5 or 10)? \n");
        scanf("%d", &input);

        if (cur_state == 0 && input != 1 && input != 2)
            continue; // wrong item

        if (cur_state != 0 && input != 1 && input != 2 && input != 5 && input != 10)
            continue; // wrong coin

        int i = 0;
        while (fsm[i].state != cur_state || fsm[i].insert != input)
            i++; // find current row in table

        if (fsm[i].next_state == 0) // the machine returned to S0
        {
            // so we give a drink!
            printf("Here's your item #%d! \n", fsm[i].item);
            printf("And change: \n");
            printf(" - 1 ruble:  %d \n", fsm[i].change[0]);
            printf(" - 2 rubles: %d \n", fsm[i].change[1]);
            printf(" - 4 rubles: %d \n", fsm[i].change[2]);
            printf(" - 5 rubles: %d \n\n", fsm[i].change[3]);
        }

        // go to next state
        printf("going to state %d \n", fsm[i].next_state);
        cur_state = fsm[i].next_state;
    }

    return 0;
}
