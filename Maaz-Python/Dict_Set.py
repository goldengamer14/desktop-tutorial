import time
import numpy as np

print("\n")

# Initialize global variables
list_time = 0.000000
dict_time = 0.000000
set_time = 0.000000
tuple_time = 0.000000
max_num_iterations = 10**6


# Define function to print the results
def print_results():
    print(f"Dict: in range = {max_num_iterations}, Time = {dict_time:.8f} seconds")
    print(f"Set: in range = {max_num_iterations}, Time = {set_time:.8f} seconds")
    print(f"List: in range = {max_num_iterations}, Time = {list_time:.8f} seconds")
    print(f"Tuple: in range = {max_num_iterations}, Time = {tuple_time:.8f} seconds")
    print(
        f"Set is {'faster' if set_time < dict_time else 'slower'} than Dict: x{dict_time / set_time if set_time < dict_time else set_time / dict_time:.2f}x"
    )
    print(
        f"Dict is {'faster' if dict_time < list_time else 'slower'} than List: x{dict_time / list_time if list_time < dict_time else list_time / dict_time:.2f}x"
    )
    print(
        f"Set is {'faster' if set_time < list_time else 'slower'} than List: x{set_time / list_time if list_time < set_time else list_time / set_time:.2f}x"
    )
    print(
        f"List is {'faster' if list_time < tuple_time else 'slower'} than Tuple: x{tuple_time / list_time if list_time < tuple_time else list_time / tuple_time:.2f}x"
    )
    print(
        f"Dict is {'faster' if dict_time < tuple_time else 'slower'} than Tuple: x{tuple_time / dict_time if dict_time < tuple_time else dict_time / tuple_time:.2f}x"
    )
    print(
        f"Set is {'faster' if set_time < tuple_time else 'slower'} than Tuple: x{tuple_time / set_time if set_time < tuple_time else set_time / tuple_time:.2f}x"
    )


# Define function to measure search time
def measure_search_time(container, value):
    start_time = time.perf_counter()
    result = value in container
    end_time = time.perf_counter()
    return end_time - start_time


# Define function to measure creation time for a list
def measure_list_creation_time(num_elements):
    start_time = time.perf_counter()
    the_list = [i for i in range(num_elements)]
    end_time = time.perf_counter()
    return end_time - start_time


# Define function to measure creation time for a tuple
def measure_tuple_creation_time(num_elements):
    start_time = time.perf_counter()
    the_tuple = tuple(i for i in range(num_elements))
    end_time = time.perf_counter()
    return end_time - start_time


# Define function to measure creation time for a dictionary
def measure_dict_creation_time(num_elements):
    start_time = time.perf_counter()
    the_dict = {i: i for i in range(num_elements)}
    end_time = time.perf_counter()
    return end_time - start_time


# Define function to measure creation time for a set
def measure_set_creation_time(num_elements):
    start_time = time.perf_counter()
    the_set = {i for i in range(num_elements)}
    end_time = time.perf_counter()
    return end_time - start_time


# Execution of search time measurement
while True:
    the_list = range(0, max_num_iterations)
    the_tuple = tuple(ele for ele in the_list)  # tuple(the_list)
    the_dict = {ele: ele for ele in the_list}
    the_set = {ele for ele in the_list}  # set(the_list)
    find_num = np.random.randint(0, max_num_iterations)

    list_time = measure_search_time(the_list, find_num)
    tuple_time = measure_search_time(the_tuple, find_num)
    set_time = measure_search_time(the_set, find_num)
    dict_time = measure_search_time(the_dict, find_num)
    max_num_iterations *= 10
    if not (
        tuple_time == 0.000000
        or list_time == 0.000000
        or dict_time == 0.000000
        or set_time == 0.000000
    ):
        break

print_results()

print("\n")

# Reset max_num_iterations for creation time measurement
max_num_iterations = 10**6

# Execution of creation time measurement
while True:
    list_time = measure_list_creation_time(max_num_iterations)
    tuple_time = measure_tuple_creation_time(max_num_iterations)
    dict_time = measure_dict_creation_time(max_num_iterations)
    set_time = measure_set_creation_time(max_num_iterations)
    max_num_iterations *= 10
    if not (
        tuple_time == 0.000000
        or list_time == 0.000000
        or dict_time == 0.000000
        or set_time == 0.000000
    ):
        break

print_results()

print("\n")
