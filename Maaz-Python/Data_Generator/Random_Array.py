import numpy as np
import sys
from itertools import chain


def Generate_Random_int_array(numbers):
    probs_100 = np.array([np.sin(np.radians(i * 2 / 101 * 90)) for i in range(101)])
    probs_1000 = np.array([np.cos(np.radians(i / 901 * 90)) for i in range(901)])

    probs_100 = np.array([prob / probs_100.sum() / 2 for prob in probs_100])
    probs_1000 = np.array([prob / probs_1000.sum() / 2 for prob in probs_1000])

    print(probs_100.sum(), probs_1000.sum())
    print(probs_100.size, probs_1000.size)

    probs = probs_100.tolist() + probs_1000.tolist()
    return np.random.choice(a=range(1002), p=probs, size=numbers)


def Generate_Random_float_array(numbers, post_point, pre_point):
    total_size = 10 ** (post_point + pre_point) + 1

    size_100 = 10 ** (post_point + pre_point - 1) + 1
    size_1000 = total_size - size_100

    probs_100_arr = np.sin(np.radians(np.linspace(0, 90, size_100)))
    probs_1000_arr = np.cos(np.radians(np.linspace(0, 90, size_1000)))

    probs_100_sum = probs_100_arr.sum()
    probs_1000_sum = probs_1000_arr.sum()

    normalized_probs_gen = chain(
        (prob / probs_100_sum / 2 for prob in probs_100_arr),
        (prob / probs_1000_sum / 2 for prob in probs_1000_arr),
    )

    p_array = np.fromiter(normalized_probs_gen, dtype=float)

    print(p_array.size, total_size)

    int_array = np.random.choice(a=range(total_size), p=p_array, size=numbers)
    return np.array([i / (10**post_point) for i in int_array], dtype=float)


# random_array = Generate_Random_int_array(12)
random_array = Generate_Random_float_array(12, 3, 3)
print(random_array.tolist())
