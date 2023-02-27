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
import time


def print_help():
    """Prints help message to console"""
    print("\nUsage: calc <path> <N>")
    print("\tpath — file to read")
    print("\tN — amount of numbers in input file")
    return 1


def main():
    # print PID
    pid: int = os.getpid()
    print("\n[Child %d] \t New proc" % pid)

    # get byte numbers and file path
    input_path: str = sys.argv[1]
    n: int = int(sys.argv[2].strip())

    # print input values
    print("[Child %d] \t Path to file with numbers: %s" % (pid, input_path))
    print("[Child %d] \t Input length: %d" % (pid, n))

    # open file to read
    with open(input_path, "r") as f:
        input_file = f.read().strip().split(" ")
        input_file = list(map(int, input_file))

    # perform calculations
    Sum: int = sum([i**2 for i in input_file])

    print("[Child %d] \t Result: %d" % (pid, Sum))

    # construct name of output file
    output_file = "_%d_result.txt" % pid

    with open(output_file, "w") as f:
        f.write(str(Sum))

    time.sleep(5)
    return 0


main()
