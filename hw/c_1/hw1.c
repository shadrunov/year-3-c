// HW 1, Shadrunov Aleksei

/* Название аэропорта и список из трёх символьных
значений – его обозначения на латинице (например, SVO
или DME).
Шифрование: к каждому байту применяется операцию
побитового исключающего ИЛИ с двоичным числом
01010101. Расшифровывание: аналогично
шифрованию.

how to run:
./hw1 -e file.bin
./hw1 -d file.bin

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

/**
 * LUT to convert unsigned char to string
 */
static char *encoding_bytes[] =
    {"00000000", "00000001", "00000010", "00000011", "00000100", "00000101", "00000110", "00000111",
     "00001000", "00001001", "00001010", "00001011", "00001100", "00001101", "00001110", "00001111",
     "00010000", "00010001", "00010010", "00010011", "00010100", "00010101", "00010110", "00010111",
     "00011000", "00011001", "00011010", "00011011", "00011100", "00011101", "00011110", "00011111",
     "00100000", "00100001", "00100010", "00100011", "00100100", "00100101", "00100110", "00100111",
     "00101000", "00101001", "00101010", "00101011", "00101100", "00101101", "00101110", "00101111",
     "00110000", "00110001", "00110010", "00110011", "00110100", "00110101", "00110110", "00110111",
     "00111000", "00111001", "00111010", "00111011", "00111100", "00111101", "00111110", "00111111",
     "01000000", "01000001", "01000010", "01000011", "01000100", "01000101", "01000110", "01000111",
     "01001000", "01001001", "01001010", "01001011", "01001100", "01001101", "01001110", "01001111",
     "01010000", "01010001", "01010010", "01010011", "01010100", "01010101", "01010110", "01010111",
     "01011000", "01011001", "01011010", "01011011", "01011100", "01011101", "01011110", "01011111",
     "01100000", "01100001", "01100010", "01100011", "01100100", "01100101", "01100110", "01100111",
     "01101000", "01101001", "01101010", "01101011", "01101100", "01101101", "01101110", "01101111",
     "01110000", "01110001", "01110010", "01110011", "01110100", "01110101", "01110110", "01110111",
     "01111000", "01111001", "01111010", "01111011", "01111100", "01111101", "01111110", "01111111",
     "10000000", "10000001", "10000010", "10000011", "10000100", "10000101", "10000110", "10000111",
     "10001000", "10001001", "10001010", "10001011", "10001100", "10001101", "10001110", "10001111",
     "10010000", "10010001", "10010010", "10010011", "10010100", "10010101", "10010110", "10010111",
     "10011000", "10011001", "10011010", "10011011", "10011100", "10011101", "10011110", "10011111",
     "10100000", "10100001", "10100010", "10100011", "10100100", "10100101", "10100110", "10100111",
     "10101000", "10101001", "10101010", "10101011", "10101100", "10101101", "10101110", "10101111",
     "10110000", "10110001", "10110010", "10110011", "10110100", "10110101", "10110110", "10110111",
     "10111000", "10111001", "10111010", "10111011", "10111100", "10111101", "10111110", "10111111",
     "11000000", "11000001", "11000010", "11000011", "11000100", "11000101", "11000110", "11000111",
     "11001000", "11001001", "11001010", "11001011", "11001100", "11001101", "11001110", "11001111",
     "11010000", "11010001", "11010010", "11010011", "11010100", "11010101", "11010110", "11010111",
     "11011000", "11011001", "11011010", "11011011", "11011100", "11011101", "11011110", "11011111",
     "11100000", "11100001", "11100010", "11100011", "11100100", "11100101", "11100110", "11100111",
     "11101000", "11101001", "11101010", "11101011", "11101100", "11101101", "11101110", "11101111",
     "11110000", "11110001", "11110010", "11110011", "11110100", "11110101", "11110110", "11110111",
     "11111000", "11111001", "11111010", "11111011", "11111100", "11111101", "11111110", "11111111"};

/**
 * Converts string with binary representation of char to char
 */
char str2char(char *input)
{
    char res = 0;
    if (input[7] == '1') res += 1;
    if (input[6] == '1') res += 2;
    if (input[5] == '1') res += 4;
    if (input[4] == '1') res += 8;
    if (input[3] == '1') res += 16;
    if (input[2] == '1') res += 32;
    if (input[1] == '1') res += 64;
    if (input[0] == '1') res += 128;
    return res;
}

/**
 * Prints help message to console
 */
void print_help()
{
    puts("usage: hw1 -e | -d [-t] <path> \n\n");
    puts("Encode to file:");
    puts("    -e <path> \t encode data and write to file specified in path");
    puts("Decode from file:");
    puts("    -d <path> \t read data from file and decode it \n");
    puts("Encode to text file:");
    puts("    -et <path> \t encode data and write to text file");
    puts("Decode from text file:");
    puts("    -dt <path> \t read data from text file and decode \n");
}

/**
 * Struct to store data about Airport
 */
struct AirportEntry
{
    char name[128];
    char letters[3];
};

/**
 * Union to serialise data from structure
 */
union AirportUnion
{
    struct AirportEntry ent;
    char arr[sizeof(struct AirportEntry) / sizeof(char)];
};

/**
 * XOR given byte with 01010101
 */
char xor_byte(char input)
{
    if (sizeof(input) != 1)
    {
        puts("ValueError: input must be 1 byte long.");
        exit(-1);
    }
    return input ^ 0b01010101;
    // return input;
}

/**
 * Copy bytes from input array to output array and apply operation function to each byte
 */
int process(char *input, char *output, size_t input_s, size_t output_s, char(operation)(char))
{
    if (input_s > output_s)
        return 1; // Indexerror

    for (int i = 0; i < input_s; i++)
        output[i] = xor_byte(input[i]); // copy byte after xor_byte

    return 0;
}

/**
 * Reads data from keyboard, returns union AirportUnion
 * user input safety: gets line of any length, truncates after 127 characters
 */
union AirportUnion get_data_from_user()
{
    union AirportUnion un;
    printf("Enter the name of the airport (for example, \"Vnukovo\") [max 127]: ");

    // get full line
    size_t input_size = sizeof(un.ent.name);
    char *init_char = malloc(input_size * sizeof(char));
    input_size = getline(&init_char, &input_size, stdin);

    // truncate the rest of input if any
    if (input_size > sizeof(un.ent.name))
        input_size = sizeof(un.ent.name);
    for (int i = 0; i < input_size - 1; i++)
        un.ent.name[i] = init_char[i];
    un.ent.name[input_size - 1] = '\0';
    free(init_char);

    // input three letters
    for (int i = 0; i < 3; i++)
    {
        printf("Enter the %d letter of the airport code (for example, letter from \"VKO\") [max 1]: ", i + 1);
        scanf(" %c", &un.ent.letters[i]);
    }

    return un;
}

int main(int argc, char **argv)
{
    // check number of arguments
    if ((argc != 3) && (argc != 4))
    {
        print_help();
        return -1;
    }

    // get mode
    bool encode;
    bool set_encode = false;
    bool text = false;
    int option;
    while ((option = getopt(argc, argv, "det")) != -1)
    {
        switch (option)
        {
        case 'd':
            puts("Decoding...");
            encode = false;
            set_encode = true;
            break;
        case 'e':
            puts("Encoding...");
            encode = true;
            set_encode = true;
            break;
        case 't':
            puts("Using text file");
            text = true;
            break;
        default:
            print_help();
            return -1;
        }
    }

    // e or d must be provided
    if (!set_encode)
    {
        print_help();
        return -1;
    }
    // get file path
    char *path = argv[argc - 1];
    printf("File path: %s \n\n", path);

    if (encode)
    {
        // get data from user
        union AirportUnion un = get_data_from_user();

        // output buffer
        char *output = malloc(sizeof(un) * sizeof(char));

        // write output to buffer
        if (process(un.arr, output, sizeof(un), sizeof(un), xor_byte))
        {
            puts("Error occurred during the process. Indexerror: size of input must not be greater than size of output");
            free(output);
            return -1;
        }

        // write buffer to file
        if (text)
        {
            FILE *ft = fopen(path, "w+");
            if (!ft) // error opening file
            {
                puts("AllocationError: can't open file");
                free(output);
                return -1;
            }
            // write every char as string from LUT
            for (int i = 0; i < sizeof(un); i++)
                fputs(encoding_bytes[(unsigned char)output[i]], ft);

            fclose(ft);
        }
        else
        {
            FILE *fp = fopen(path, "wb");
            if (!fp) // error opening file
            {
                puts("AllocationError: can't open file");
                free(output);
                return -1;
            }
            // write bytes directly
            fwrite(output, sizeof(char), sizeof(un), fp);
            fclose(fp);
        }

        free(output);
    }
    else // decode
    {
        // create empty structure
        union AirportUnion un;
        char *input = malloc(sizeof(un) * sizeof(char));

        // get encrypted data from file
        if (access(path, F_OK))
        {
            puts("AllocationError: can't open file, not found");
            free(input);
            return -1;
        }

        if (text)
        {
            FILE *ft = fopen(path, "r");
            if (!ft) // error opening file
            {
                puts("AllocationError: can't open file");
                free(input);
                return -1;
            }
            // read file by chunks of 8 bits
            size_t chunksize = 8;
            char buffer[chunksize + 1];

            for (int i = 0; i < sizeof(un); i++)
            {
                fseek(ft, SEEK_SET, i * chunksize);          // find position (each iteration +8 bits)
                fread(&buffer, sizeof(char), chunksize, ft); // read 8 symbols to buffer
                buffer[chunksize] = '\0';                    // terminator
                // printf("%s \n", buffer);
                // printf("%u ", (unsigned char)str2char(buffer));
                input[i] = str2char(buffer); // convert string in buffer to char
            }
            fclose(ft);
        }
        else
        {
            FILE *fp = fopen(path, "rb");
            if (!fp) // error opening file
            {
                puts("AllocationError: can't open file");
                free(input);
                return -1;
            }
            fread(input, sizeof(char), sizeof(un), fp);
            fclose(fp);
        }
        // decrypt data
        if (process(input, un.arr, sizeof(un), sizeof(un), xor_byte))
        {
            puts("Error occurred during the process. Indexerror: size of input must not be greater than size of output");
            free(input);
            return -1;
        }

        printf("Airport name: %s \n", un.ent.name);
        printf("Airport code: %s \n", un.ent.letters);

        free(input);
    }

    return 0;
}
