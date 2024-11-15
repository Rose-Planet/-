#include <iostream>
#include <ctime>
#include <algorithm>
#include <windows.h>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <sstream>

using namespace std;

clock_t start_time, end_time;

// 将随机生成的数组写入文件，文件名为 "random_array_n.txt"
void writeRandomArrayToFile(int* a, int n) {
    stringstream filename;
    filename << "random_array_" << n << ".txt";  // 文件名格式为 "random_array_n.txt"

    ofstream outFile(filename.str(), ios::out); // 打开文件
    if (outFile.is_open()) {
        for (int i = 0; i < n; i++) {
            outFile << a[i] << " "; // 写入每个随机生成的元素
        }
        outFile << endl; // 写入换行符
        outFile.close(); // 关闭文件
        cout << "生成的随机数组已写入文件 " << filename.str() << endl;
    } else {
        cout << "无法打开文件写入数据!" << endl;
    }
}

// 生成n个随机数并返回数组
void generateRandomArray(int* a, int n) {
    for (int i = 0; i < n; i++) {
        a[i] = rand(); // 生成随机数
    }
}

// 合并函数，用于合并两个子数组
void merge(int* a, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // 创建临时数组
    int* L = new int[n1];
    int* R = new int[n2];

    // 复制数据到临时数组
    for (int i = 0; i < n1; i++) L[i] = a[left + i];
    for (int i = 0; i < n2; i++) R[i] = a[mid + 1 + i];

    int i = 0, j = 0, k = left;

    // 合并两个临时数组
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            a[k] = L[i];
            i++;
        } else {
            a[k] = R[j];
            j++;
        }
        k++;
    }

    // 将剩余的元素复制到原数组
    while (i < n1) {
        a[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        a[k] = R[j];
        j++;
        k++;
    }

    // 释放临时数组内存
    delete[] L;
    delete[] R;
}

// 递归合并排序函数
void mergeSortRecursive(int* a, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSortRecursive(a, left, mid);   // 排序左半部分
        mergeSortRecursive(a, mid + 1, right); // 排序右半部分
        merge(a, left, mid, right);  // 合并两个有序部分
    }
}

// 快速排序的分区操作
int partition(int* a, int low, int high) {
    int pivot = a[high];  // 选择最后一个元素作为基准
    int i = low - 1;  // i 是较小元素的索引

    for (int j = low; j <= high - 1; j++) {
        if (a[j] < pivot) {
            i++;  // 增加较小元素的索引
            swap(a[i], a[j]);  // 交换
        }
    }
    swap(a[i + 1], a[high]);  // 将基准元素放到正确位置
    return i + 1;  // 返回基准的索引
}

// 快速排序递归函数
void quickSortRecursive(int* a, int low, int high) {
    if (low < high) {
        int pi = partition(a, low, high);  // 获取基准的索引
        quickSortRecursive(a, low, pi - 1);  // 排序基准左边
        quickSortRecursive(a, pi + 1, high);  // 排序基准右边
    }
}

// 将排序时间结果写入CSV文件
void writeResultsToCSV(string filename, int test_size, double times[5], string sortType) {
    ofstream outFile(filename, ios::app);  // 追加模式写入文件
    if (outFile.is_open()) {
        // 将每次排序时间写入CSV文件
        outFile << test_size;  // 数据量
        for (int i = 0; i < 5; i++) {
            outFile << "," << fixed << setprecision(4) << times[i];  // 排序版本的时间
        }
        outFile << endl;  // 换行
        outFile.close();  // 关闭文件
    } else {
        cout << "无法打开文件写入结果!" << endl;
    }
}

// 主函数
int main() {
    int* a; // 使用指针来动态分配内存
    srand(time(NULL)); // 随机数种子

    // 定义测试数据量
    int test_sizes[] = {5000, 10000, 20000, 40000, 80000};

    // 创建并初始化CSV文件，写入表头
    ofstream outFile("merge_sort_results.csv");
    if (outFile.is_open()) {
        outFile << "Data Size,Test 1,Test 2,Test 3,Test 4,Test 5\n";  // CSV 表头
        outFile.close();
    } else {
        cout << "无法创建CSV文件!" << endl;
        return 1;
    }

    // 创建并初始化CSV文件，写入表头
    ofstream outFileQuickSort("quick_sort_results.csv");
    if (outFileQuickSort.is_open()) {
        outFileQuickSort << "Data Size,Test 1,Test 2,Test 3,Test 4,Test 5\n";  // CSV 表头
        outFileQuickSort.close();
    } else {
        cout << "无法创建CSV文件!" << endl;
        return 1;
    }

    // 遍历每个测试数据量
    for (int i = 0; i < 5; i++) {
        int n = test_sizes[i]; // 当前测试数据量
        cout << "正在测试数据量: " << n << endl;

        // 动态分配数组空间
        a = new int[n];

        // 生成n个随机数并将其存入数组a[]
        generateRandomArray(a, n);

        // 将生成的随机数写入文件，文件名包含数据量n
        writeRandomArrayToFile(a, n);

        // 保存五次的排序时间
        double timesMerge[5];
        double timesQuick[5];

        // 五次测试
        for (int j = 0; j < 5; j++) {
            // 合并排序测试
            start_time = clock(); // 设置开始时间
            mergeSortRecursive(a, 0, n - 1); // 调用递归合并排序函数
            end_time = clock(); // 设置结束时间
            double merge_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
            timesMerge[j] = merge_time;

            // 快速排序测试
            start_time = clock(); // 设置开始时间
            quickSortRecursive(a, 0, n - 1); // 调用递归快速排序函数
            end_time = clock(); // 设置结束时间
            double quick_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
            timesQuick[j] = quick_time;

            // 为了便于调试，避免每次测试之间的冲突，稍微暂停
            Sleep(500);  // 暂停500毫秒
        }

        // 将合并排序结果写入CSV文件
        writeResultsToCSV("merge_sort_results.csv", n, timesMerge, "Merge Sort");

        // 将快速排序结果写入CSV文件
        writeResultsToCSV("quick_sort_results.csv", n, timesQuick, "Quick Sort");

        // 释放动态分配的内存
        delete[] a;

        cout << endl; // 分隔不同的数据量测试
    }

    cout << "所有测试完成，结果已保存至 'merge_sort_results.csv' 和 'quick_sort_results.csv' 文件中。" << endl;

    return 0;
}
