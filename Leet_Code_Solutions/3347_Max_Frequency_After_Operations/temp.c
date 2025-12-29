
static inline void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

static int partition(int arr[], int low, int high) {
    int pivot = arr[low];
    int left = low + 1, right = high;

    do {
        while (left <= high && arr[left] <= pivot)
            left++;
        while (right > low && arr[right] > pivot)
            right--;

        if (left < right)
            swap(arr + left, arr + right);
    } while (left < right);

    swap(arr + low, arr + right);

    return right;
}

static void Quick_Sort(int arr[], int low, int high) {
    if (low > high)
        return;

    if (low < high) {
        int partition_index = partition(arr, low, high);

        if (partition_index > low)
            Quick_Sort(arr, low, partition_index - 1);
        if (partition_index + 1 < high)
            Quick_Sort(arr, partition_index + 1, high);
    }
}

static int check_array(int* arr, int* freq_arr, int* points, const int p_size,
                       const int k, const int restore_numOperations,
                       int* max_freq_index) {
    int min_numOpers = restore_numOperations;
    int numOperations = restore_numOperations;
    int min_numOpers_max_freq_index = numOperations;

    for (int p = 0; p < p_size; p++) {
        const int point = points[p];
        const int restore_point_val = arr[point];
        int i, max_freq_i = point;

        printf("numOperations = %i for arr[point=%i] = %i\n", numOperations,
               point, arr[point]);

        for (i = point - 1; i >= 0 && arr[point] - arr[i] <= k; i--) {
            numOperations -= freq_arr[i];

            if (freq_arr[i] > freq_arr[max_freq_i])
                max_freq_i = i;

            // printf("arr[i=%i] = %i; freq_arr[i=%i] = %i; numOperations =
            // %i\n",
            //        i, arr[i], i, freq_arr[i], numOperations);
            if (numOperations < 1) {
                // *max_freq_index = max_freq_i;
                // return 0;
                break;
            }
        }

        if (numOperations < min_numOpers)
            min_numOpers = numOperations >= 0 ? numOperations : 0;

        if (numOperations > 0 && i >= 0) {
            arr[point] = arr[i + 1];
            // numOperations--;
            // arr[point] -= k;

            // printf("\narr[point=%i] = %i\n", point, arr[point]);

            // printf("Here arr[i is %i] is %i\n", i, arr[i]);
            for (i;
                 i >= 0 && abs(arr[point] - arr[i]) <= k && numOperations > 0;
                 i--) {
                numOperations -= freq_arr[i];

                if (freq_arr[i] > freq_arr[max_freq_i])
                    max_freq_i = i;

                // printf(
                //     "arr[i=%i] = %i; numOperations = %i; min_numOpers =
                //     %i\n", i, arr[i], numOperations, min_numOpers);
            }

            arr[point] = restore_point_val;
            // putchar('\n');

            for (i; i >= 0 && abs(arr[point] - arr[i]) > k &&
                    abs(arr[point] - arr[i]) <= 2 * k && numOperations > 1;
                 i--) {
                numOperations -= freq_arr[i];

                if (freq_arr[i] > freq_arr[max_freq_i])
                    max_freq_i = i;

                // printf(
                //     "arr[point=%i] = %i; arr[i=%i] = %i; numOperations =
                //     %i\n", point, arr[point], i, arr[i], numOperations);
            }

            if (numOperations < min_numOpers)
                min_numOpers = numOperations >= 0 ? numOperations : 0;
        }

        printf("max_freq_index = %i; max_freq_i = %i\n", *max_freq_index,
               max_freq_i);

        printf("min_numOpers = %i; min_numOpers_max_freq_index = %i\n", min_numOpers,
               min_numOpers_max_freq_index);

        if (min_numOpers == min_numOpers_max_freq_index &&
            freq_arr[max_freq_i] > freq_arr[*max_freq_index]) {
            *max_freq_index = max_freq_i;
            printf("setting min_numOpers_max_freq_index=%i = min_numOpers=%i\n", min_numOpers_max_freq_index, min_numOpers);
            min_numOpers_max_freq_index = min_numOpers;
        } else if (min_numOpers < min_numOpers_max_freq_index && max_freq_i != *max_freq_index) {
            *max_freq_index = max_freq_i;
            printf("setting min_numOpers_max_freq_index=%i = min_numOpers=%i\n", min_numOpers_max_freq_index, min_numOpers);
            min_numOpers_max_freq_index = min_numOpers;
        }

        numOperations = restore_numOperations;

        printf("arr[point=%i] = %i; min_numOpers = %i\n\n", point, arr[point],
               min_numOpers);
    }

    // printf("\nmin_numOpers = %i\n\n", min_numOpers);
    return min_numOpers;
}

int maxFrequency(int* nums, int numsSize, int k, int numOperations) {
    int val_size = 0, max_freq_i = 0, result, zero_operations;
    int* values = (int*)malloc(numsSize * sizeof(int) * 4);
    int* freqs = values + numsSize;
    int* max_freq = freqs + numsSize;
    int* indices = max_freq + numsSize;
    freqs[0] = 1;
    max_freq[0] = 0;

    // if (values)
    //     printf("Heap Array Allocated!!!\n");
    // else
    //     printf("Heap Array Allocation failed!!!\n");
    // fflush(stdout);

    Quick_Sort(nums, 0, numsSize - 1);

    // putchar('{');
    // for (int i = 0; i < numsSize; i++)
    //     printf("%i%s", nums[i], i < numsSize - 1 ? ", " : "}\n\n");

    for (int i = 0; i < numsSize; i++) {
        int j = i + 1;
        while (j < numsSize && nums[j] == nums[i])
            j++;

        values[val_size] = nums[i];
        freqs[val_size] = j - i;

        if (freqs[max_freq[0]] < freqs[val_size]) {
            max_freq[0] = val_size;
            max_freq_i = 1;
        } else if (freqs[max_freq[0]] == freqs[val_size]) {
            max_freq[max_freq_i] = val_size;
            max_freq_i++;
        }

        val_size++;
        i = j - 1;
    }

    putchar('{');
    for (int i = 0; i < val_size; i++)
        printf("%i%s", values[i], i < val_size - 1 ? ", " : "}\n");

    putchar('{');
    for (int i = 0; i < val_size; i++)
        printf("%i%s", freqs[i], i < val_size - 1 ? ", " : "}\n");

    // putchar('{');
    // for (int i = 0; i < max_freq_i; i++)
    //     printf("%i%s", max_freq[i], i < max_freq_i - 1 ? ", " : "}\n");

    if (numOperations < 1)
        return freqs[max_freq[0]];

    for (int i = 0; i < val_size; i++)
        indices[i] = i;

    // printf("indices = {");
    // for (int i = 0; i < val_size; i++)
    //     printf("%i%s", indices[i], i < val_size - 1 ? ", " : "}\n");

    max_freq_i = 0;
    zero_operations =
        // check_array(values, freqs, max_freq, max_freq_i, k, numOperations);
        check_array(values, freqs, indices, val_size, k, numOperations,
                    &max_freq_i);

    result = freqs[max_freq_i] + numOperations - zero_operations;
    // result = freqs[max_freq[0]] + numOperations - zero_operations;

    // printf("\nzero_operations = %i\n", zero_operations);
    printf("freqs[max_freq_i=%i]=%i + numOperations=%i - zero_operations=%i = "
           "result=%i\n",
           max_freq_i, freqs[max_freq_i], numOperations, zero_operations,
           result);

    free(values);
    return result <= numsSize ? result : numsSize;
}
