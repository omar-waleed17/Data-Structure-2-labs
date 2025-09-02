#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

int getRandomInRange(int low, int high) {
    return low + rand() % (high - low + 1);
}

void merge(int array[], int low, int mid, int high) {
    int size_left = mid - low + 1;
    int size_right = high - mid;

    vector<int> leftArr(size_left);
    vector<int> rightArr(size_right);

    for (int i = 0; i < size_left; i++)
        leftArr[i] = array[low + i];
    for (int i = 0; i < size_right; i++)
        rightArr[i] = array[mid + 1 + i];

    int i = 0, j = 0, k = low;
    while (i < size_left && j < size_right) {
        if (leftArr[i] <= rightArr[j]) {
            array[k++] = leftArr[i++];
        }
        else {
            array[k++] = rightArr[j++];
        }
    }

    while (i < size_left) {
        array[k++] = leftArr[i++];
    }

    while (j < size_right) {
        array[k++] = rightArr[j++];
    }
}

void insertion_sort(int arr[], int low, int high) {
    for (int i = low + 1; i <= high; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= low && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void merge_sort(int arr[], int low, int high, int threshold) {
    if ((high - low + 1) <= threshold) {
        insertion_sort(arr, low, high);
        return;
    }

    int mid = low + (high - low) / 2;
    merge_sort(arr, low, mid, threshold);
    merge_sort(arr, mid + 1, high, threshold);
    merge(arr, low, mid, high);
}

int partition_(int* arr, int first, int last) {
    int s1 = first;
    int piv_index = getRandomInRange(first, last);
    swap(arr[piv_index], arr[first]);
    int pivot = arr[first];

    for (int i = first + 1; i <= last; i++)
        if (arr[i] < pivot)
            swap(arr[i], arr[++s1]);

    swap(arr[first], arr[s1]);
    return s1;
}

int kth_smallest(int* arr, int first, int last, int kth) {
    if (kth <= 0 || kth > last - first + 1)
        return -1;

    int f = first;
    int l = last;
    int piv_index = -1;

    while (l > f) {
        piv_index = partition_(arr, f, l);
        if (piv_index + 1 < kth + first)
            f = piv_index + 1;
        else if (piv_index + 1 > kth + first)
            l = piv_index - 1;
        else
            return arr[piv_index];
    }
    return arr[l];
}

int main() {
    srand(time(0));

    int size;
    cout << "Enter number of elements: ";
    cin >> size;

    int* arr = new int[size];

    cout << "Generated array: ";
    for (int i = 0; i < size; i++) {
        arr[i] = getRandomInRange(1, 100);
        cout << arr[i] << " ";
    }
    cout << endl;

    int kth, threshold;
    cout << "Enter k (1 to " << size << "): ";
    cin >> kth;
    cout << "Enter threshold: ";
    cin >> threshold;

    int* arr_copy = new int[size];
    copy(arr, arr + size, arr_copy);

    int kth_element = kth_smallest(arr_copy, 0, size - 1, kth);

    if (kth_element != -1)
        cout << kth << "th smallest element is " << kth_element << endl;
    else
        cout << "Invalid value of k!" << endl;

    merge_sort(arr, 0, size - 1, threshold);

    cout << "Sorted array: ";
    for (int i = 0; i < size; i++)
        cout << arr[i] << " ";
    cout << endl;

    delete[] arr;
    delete[] arr_copy;
    return 0;
}
