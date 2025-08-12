import numpy as np


def Generate_Random_int_array(numbers):
    probs_100 = np.array([np.sin(np.radians(i * 2 / 101 * 90)) for i in range(101)])
    probs_1000 = np.array([np.cos(np.radians(i / 901 * 90)) for i in range(901)])

    probs_100 = np.array([prob / probs_100.sum() / 2 for prob in probs_100])
    probs_1000 = np.array([prob / probs_1000.sum() / 2 for prob in probs_1000])

    print(probs_100.sum(), probs_1000.sum())
    print(probs_100.size, probs_1000.size)

    probs = probs_100.tolist() + probs_1000.tolist()
    return np.random.choice(a=range(1002), p=probs, size=numbers)


def Generate_Random_float_array(numbers, post_point):
    # probs_100 = np.array(
    #     [
    #         np.sin(np.radians(i * 2 / (10 ** (post_point + 2) + 1) * 90))
    #         for i in range(10 ** (post_point) + 1)
    #     ],
    #     dtype=float,
    # )
    # probs_1000 = np.array(
    #     [
    #         np.cos(np.radians(i / (10 ** (post_point + 2) + 1) * 90))
    #         for i in range(10 ** (post_point**2) - 10**(post_point))
    #     ],
    #     dtype=float,
    # )
    probs_100 = np.sin(np.radians(np.linspace(0, 90, 10 ** (post_point) + 1)))
    probs_1000 = np.cos(
        np.radians(np.linspace(0, 90, 10 ** (post_point + 2) - 10**post_point))
    )

    probs_100 = np.array([prob / probs_100.sum() / 2 for prob in probs_100])
    probs_1000 = np.array([prob / probs_1000.sum() / 2 for prob in probs_1000])

    probs = np.hstack([probs_100, probs_1000])
    print(probs.size, 10 ** (post_point + 2) + 1)

    int_array = np.random.choice(
        a=range(10 ** (post_point + 2) + 1), p=probs, size=numbers
    )
    return np.array([float(i / (10**post_point)) for i in int_array], dtype=float)


# random_array = Generate_Random_int_array(12)
random_array = Generate_Random_float_array(12, 3)
print(random_array.tolist())
