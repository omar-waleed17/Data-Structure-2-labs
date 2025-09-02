
#include <iostream>
#include <ctime>  
#include <vector>
#include <chrono>
using namespace std::chrono;
using namespace std;
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
void print_Array(int arr[], int n)
{
	for (int i = 0; i < n; i++)
	{
		cout << arr[i] << ",";
	}
}
vector<long long> time_calculator(vector<int> arr) {
	vector<long long> times(3); 

	// Bubble Sort
	vector<int> temp = arr;
	clock_t start = clock();
	bubble_sort(temp);
	clock_t end = clock();
	times[0] = (long long)(end - start) * 1000 / CLOCKS_PER_SEC;

	// Insertion Sort
	temp = arr;
	start = clock();
	insertion_sort(temp);
	end = clock();
	times[1] = (long long)(end - start) * 1000 / CLOCKS_PER_SEC;

	// Selection Sort
	temp = arr;
	start = clock();
	selection_sort(temp);
	end = clock();
	times[2] = (long long)(end - start) * 1000 / CLOCKS_PER_SEC;

	return times;
}
vector<int> array_generator(int size) 
{
	int min_val = 1;
	int max_val = 100000;
	vector<int> arr(size);
	srand(time(0));  
	for (int i = 0; i < size; i++) {
		arr[i] = min_val + rand() % (max_val - min_val + 1);
	}
	return arr;
}

int main()
{
	int size;
	cout << "please input size of your array, minimum 500";
	cin >> size;
	vector<int> arr = array_generator(size);

	vector<long long> times = time_calculator(arr);

	cout << "Bubble Sort took " << times[0] << " ms\n";
	cout << "Insertion Sort took " << times[1] << " ms\n";
	cout << "Selection Sort took " << times[2] << " ms\n";

	return 0;
}










