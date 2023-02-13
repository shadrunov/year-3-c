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
} row;

// manually add rows to table
void create_table(row *pointer)
{
    pointer[0] = (struct row){0, 1, 1, {0, 0, 0, 0}};
    pointer[1] = (struct row){0, 2, 2, {0, 0, 0, 0}};
    pointer[2] = (struct row){0, 5, 5, {0, 0, 0, 0}};
    pointer[3] = (struct row){0, 10, 0, {1, 0, 0, 0}};
    pointer[4] = (struct row){1, 1, 2, {0, 0, 0, 0}};
    pointer[5] = (struct row){1, 2, 3, {0, 0, 0, 0}};
    pointer[6] = (struct row){1, 5, 6, {0, 0, 0, 0}};
    pointer[7] = (struct row){1, 10, 0, {0, 1, 0, 0}};
    pointer[8] = (struct row){2, 1, 3, {0, 0, 0, 0}};
    pointer[9] = (struct row){2, 2, 4, {0, 0, 0, 0}};
    pointer[10] = (struct row){2, 5, 7, {0, 0, 0, 0}};
    pointer[11] = (struct row){2, 10, 0, {1, 1, 0, 0}};
    pointer[12] = (struct row){3, 1, 4, {0, 0, 0, 0}};
    pointer[13] = (struct row){3, 2, 5, {0, 0, 0, 0}};
    pointer[14] = (struct row){3, 5, 8, {0, 0, 0, 0}};
    pointer[15] = (struct row){3, 10, 0, {0, 0, 1, 0}};
    pointer[16] = (struct row){4, 1, 5, {0, 0, 0, 0}};
    pointer[17] = (struct row){4, 2, 6, {0, 0, 0, 0}};
    pointer[18] = (struct row){4, 5, 0, {0, 0, 0, 0}};
    pointer[19] = (struct row){4, 10, 0, {0, 0, 0, 1}};
    pointer[20] = (struct row){5, 1, 6, {0, 0, 0, 0}};
    pointer[21] = (struct row){5, 2, 7, {0, 0, 0, 0}};
    pointer[22] = (struct row){5, 5, 0, {1, 0, 0, 0}};
    pointer[23] = (struct row){5, 10, 0, {1, 0, 0, 1}};
    pointer[24] = (struct row){6, 1, 7, {0, 0, 0, 0}};
    pointer[25] = (struct row){6, 2, 8, {0, 0, 0, 0}};
    pointer[26] = (struct row){6, 5, 0, {0, 1, 0, 0}};
    pointer[27] = (struct row){6, 10, 0, {0, 1, 0, 1}};
    pointer[28] = (struct row){7, 1, 8, {0, 0, 0, 0}};
    pointer[29] = (struct row){7, 2, 0, {1, 0, 0, 0}};
    pointer[30] = (struct row){7, 5, 0, {1, 1, 0, 0}};
    pointer[31] = (struct row){7, 10, 0, {1, 1, 0, 1}};
    pointer[32] = (struct row){8, 1, 0, {0, 0, 0, 0}};
    pointer[33] = (struct row){8, 2, 0, {1, 0, 0, 0}};
    pointer[34] = (struct row){8, 5, 0, {0, 0, 1, 0}};
    pointer[35] = (struct row){8, 10, 0, {0, 0, 1, 1}};
}

int main()
{
    // init FSM with precalculated values
    int states_num = 36;
    row *fsm = malloc(sizeof(row) * states_num);
    create_table(fsm);

    // start a machine
    printf("Price is 9 \n\n");

    int cur_state = 0;
    int input;

    while (1)
    {
        // new iteration
        printf("\nCurrent state: %d\n", cur_state);
        printf("Enter coin (1, 2, 5 or 10)? \n");
        scanf("%d", &input);

        if (input != 1 && input != 2 && input != 5 && input != 10)
            continue;  // wrong coin

        int i = 0;
        while (fsm[i].state != cur_state || fsm[i].insert != input)
            i++;  // find current row in table

        if (fsm[i].next_state == 0)  // the machine returned to S0
        {
            // so we give a drink!
            printf("Here's your drink! \n");
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
