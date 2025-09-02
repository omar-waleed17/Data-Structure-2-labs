#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

milliseconds calcTime(void (*sorting_function)(vector<int>&), vector<int> arr) {
    auto start = high_resolution_clock::now();
    sorting_function(arr);
    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start);
}


void selection_sort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min]) {
                min = j;
            }
        }
        swap(arr[min], arr[i]);
    }
}


void insertion_sort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int j;
        int temp = arr[i];
        for (j = i; j > 0 && temp < arr[j - 1]; j--) {
            arr[j] = arr[j - 1];
        }
        arr[j] = temp;
    }
}


void bubble_sort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}


void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> leftArr(n1), rightArr(n2);

    for (int i = 0; i < n1; i++) leftArr[i] = arr[left + i];
    for (int i = 0; i < n2; i++) rightArr[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) arr[k++] = leftArr[i++];
        else arr[k++] = rightArr[j++];
    }
    while (i < n1) arr[k++] = leftArr[i++];
    while (j < n2) arr[k++] = rightArr[j++];
}


void merge_sort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void merge_sort_wrapper(vector<int>& arr) {
    merge_sort(arr, 0, arr.size() - 1);
}

int partition_(vector<int>& arr, int low, int high) {
    int pivot_index = low + rand() % (high - low + 1);
    swap(arr[pivot_index], arr[high]);
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            swap(arr[++i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quick_sort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int piv_index = partition_(arr, low, high);
        quick_sort(arr, low, piv_index - 1);
        quick_sort(arr, piv_index + 1, high);
    }
}

void quick_sort_wrapper(vector<int>& arr) {
    quick_sort(arr, 0, arr.size() - 1);
}


int leftChild(int i) { return 2 * i + 1; }
int rightChild(int i) { return 2 * i + 2; }

void heapify(vector<int>& arr, int i, int heap_len) {
    int left = leftChild(i);
    int right = rightChild(i);
    int max = i;

    if (left < heap_len && arr[max] < arr[left]) max = left;
    if (right < heap_len && arr[max] < arr[right]) max = right;

    if (max != i) {
        swap(arr[max], arr[i]);
        heapify(arr, max, heap_len);
    }
}

void buildMaxHeap(vector<int>& arr) {
    for (int i = arr.size() / 2 - 1; i >= 0; i--) {
        heapify(arr, i, arr.size());
    }
}

void heap_sort(vector<int>& arr) {
    buildMaxHeap(arr);
    for (int heap_len = arr.size() - 1; heap_len > 0; heap_len--) {
        swap(arr[0], arr[heap_len]);
        heapify(arr, 0, heap_len);
    }
}

vector<int> array_generator(int size) {
    int min_val = 1, max_val = 100000;
    vector<int> arr(size);
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < size; i++) {
        arr[i] = min_val + rand() % (max_val - min_val + 1);
    }
    return arr;
}

int main() {
    int size;
    cout << "Please input size of your array, minimum 500: ";
    cin >> size;

    if (size < 500) {
        cout << "Array size must be at least 500." << endl;
        return 1;
    }

    vector<int> arr = array_generator(size);
    milliseconds times[6];

    thread t1([&]() { times[0] = calcTime(bubble_sort, arr); });
    thread t2([&]() { times[1] = calcTime(insertion_sort, arr); });
    thread t3([&]() { times[2] = calcTime(selection_sort, arr); });
    thread t4([&]() { times[3] = calcTime(merge_sort_wrapper, arr); });
    thread t5([&]() { times[4] = calcTime(quick_sort_wrapper, arr); });
    thread t6([&]() { times[5] = calcTime(heap_sort, arr); });

    t1.join(); t2.join(); t3.join(); t4.join(); t5.join(); t6.join();

    cout << "Bubble Sort took " << times[0].count() << " ms\n";
    cout << "Insertion Sort took " << times[1].count() << " ms\n";
    cout << "Selection Sort took " << times[2].count() << " ms\n";
    cout << "Merge Sort took " << times[3].count() << " ms\n";
    cout << "Quick Sort took " << times[4].count() << " ms\n";
    cout << "Heap Sort took " << times[5].count() << " ms\n";

    return 0;
}
