"""
В файле записан ряд целых чисел, разделённых пробелом. Программа должна считать имя файла из первого
аргумента командной строки и рассчитать сумму квадратов записанных в файл чисел. Для расчёта суммы квадратов
программа должна создать N дочерних процессов (N передаётся вторым аргументом командной строки) и передать
каждому из них часть полученных чисел. Каждый из дочерних процессов должен рассчитать сумму квадратов
переданных ему чисел и вернуть её родительскому. Родительский процесс должен просуммировать полученные от
дочерних числа и вывести на консоль итоговую сумму. Если исходный файл не существует, или в нём записано менее
2 чисел, следует вывести соответствующее сообщение для пользователя и завершить работу программы.

"""


import sys
import time
import threading

mutex = threading.Lock()
globalResult = 0


def print_help():
    """Prints help message to console"""
    print("\nUsage: main <path> <N>")
    print("\tpath — file to read")
    print("\tN — fork number")
    return 1


def calc(args):
    global globalResult
    total = 0
    threadNumber = args[0]
    numberCount = args[1]
    print(f"\t [Thread {threadNumber}] started")
    print(f"\t [Thread {threadNumber}] numberCount: {numberCount}")

    for i in range(2, numberCount + 2):
        # calc sum of squares
        total += args[i] ** 2

    print(f"\t [Thread {threadNumber}] calculated result: {total}")

    # update global result
    mutex.acquire()
    globalResult += total
    print(f"\t [Thread {threadNumber}] new globalResult: {globalResult}")
    mutex.release()
    # time.sleep(500)
    print(f"\t [Thread {threadNumber}] finished")


def main():
    # check number of arguments
    if len(sys.argv) != 3:
        return print_help()

    # get amount of numbers and file path
    input_path: str = sys.argv[1]
    N: int = int(sys.argv[2])

    # print input values
    print("Path to file: %s" % input_path)
    print("Thread number: %d" % N)

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
        print("Too many threads. New fork number: %d" % N)

    # calculate division between processes
    n: int = M // N
    n_last: int = n + M % N

    print("Each thread gets: %d numbers" % n)
    print("Last thread gets: %d numbers" % n_last)

    # create files for each process and fork
    threadArray = []
    for i in range(N):
        # write numbers to output file
        n_effective: int = n_last if i == N - 1 else n
        subset = input_file[n * i : n * i + n_effective]
        subset = [i, len(subset)] + subset

        # start thread
        threadArray.append(threading.Thread(target=calc, args=(subset,)))
        threadArray[i].start()

    for i in range(N):
        threadArray[i].join()

    print("\nFinal Result: %d" % globalResult)
    return 0


main()
