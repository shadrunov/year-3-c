"""
В файле записан ряд целых чисел, разделённых пробелом. Программа должна считать имя файла из первого
аргумента командной строки и рассчитать сумму квадратов записанных в файл чисел. Для расчёта суммы квадратов
программа должна создать N дочерних процессов (N передаётся вторым аргументом командной строки) и передать
каждому из них часть полученных чисел. Каждый из дочерних процессов должен рассчитать сумму квадратов
переданных ему чисел и вернуть её родительскому. Родительский процесс должен просуммировать полученные от
дочерних числа и вывести на консоль итоговую сумму. Если исходный файл не существует, или в нём записано менее
2 чисел, следует вывести соответствующее сообщение для пользователя и завершить работу программы.

"""


import os
import sys


def print_help():
    """Prints help message to console"""
    print("\nUsage: main <path> <N>")
    print("\tpath — file to read")
    print("\tN — fork number")
    return 1


def main():
    # check number of arguments
    if len(sys.argv) != 3:
        return print_help()

    # get amount of numbers and file path
    input_path: str = sys.argv[1]
    N: int = int(sys.argv[2])

    # print input values
    print("Path to file: %s" % input_path)
    print("Fork number: %d" % N)

    # open file to read
    with open(input_path, "r") as f:
        input_file = f.read().strip().split(" ")
        input_file = list(map(int, input_file))

    M = len(input_file)
    print("Input size: %d" % M)

    # if too few numbers
    if M < 2:
        print("Too few numbers (M must be greater than 2)")
        return print_help()

    # if too many forks
    if N > M // 2:
        N = M // 2
        print("Too many forks. New fork number: %d" % N)

    # calculate division between processes
    n: int = M // N
    n_last: int = n + M % N

    print("Each proc gets: %d numbers" % n)
    print("Last proc gets: %d numbers" % n_last)

    # create files for each process and fork
    for i in range(N):
        # construct name of output file
        output_path = "_%d_input.txt" % i

        # open file to write
        with open(output_path, "w") as output_file:
            # write numbers to output file
            n_effective: int = n_last if i == N - 1 else n
            subset = input_file[n * i : n * i + n_effective]
            strings = list(map(str, subset))
            output_file.write(" ".join(strings))

        # start process
        pid = os.fork()

        if not pid:  # forked
            print("Forked proc %d with output_path %s" % (os.getpid(), output_path))
            os.execvp("python", ["python", "calc.py", output_path, str(n_effective)])

    # stop process to see zombies
    input()

    # calc results
    sum: int = 0
    for i in range(N):
        child, code = os.wait()

        # construct name of output file
        result_path = "_%d_result.txt" % child

        # open result file to read
        with open(result_path, "r") as result_file:
            # read result from file
            sum += int(result_file.read().strip())

    print("\nFinal Result: %d" % sum)
    return 0


main()
