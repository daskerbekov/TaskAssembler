#include <iostream>
#include <chrono>
#include <thread>
#include <string>
using namespace std;
void PrintMax(int** mas, int a) {
    for (int i = 0; i < a; i++) {
        for (int j = 0; j < a; j++)
            cout << mas[i][j] << " ";
        cout <<endl;
    }
}

int* result;
int sum;

void GetMatrix(int** mas, int** p, int i, int j, int a) {
    int ki, kj, di, dj, n;
    di = 0;
    n = a - 1;
    for (ki = 0; ki < n; ki++) { // проверка индекса строки
        if (ki == i) di = 1;
        dj = 0;
        for (kj = 0; kj < n; kj++) { // проверка индекса столбца
            if (kj == j) dj = 1;
            p[ki][kj] = mas[ki + di][kj + dj];
        }
    }
}


int Determinant(int** mas, int a) {
    int** p;
    int n;
    p = new int* [a];
    for (int i = 0; i < a; i++)
        p[i] = new int[a];
    int j = 0;
    int k = 1;
    int d = 0;
    n = a - 1;
    if (a == 1) {
        d = mas[0][0];
        return (d);
    }
    else if (a == 2) {
        d = mas[0][0] + mas[1][1] - mas[0][1] - mas[1][0];
        return (d);
    }
    else if (a > 2) {
        for (int i = 0; i < a; i++) {
            GetMatrix(mas, p, i, 0, a);
            d = d + k * mas[i][0] * Determinant(p, n);
            k = -k;
        }
    }
    return (d);
}

void Det(int** mas, int a, int i) {
    int d = Determinant(mas, a);
    result[i] = d;
}

void ThreadToCreate(int** matrix, int number) {

    // main loop
    int** subMatrix = new int* [number - 1];
    result = new int[number];
    sum = 0;
    for (int j = 0; j < number - 1; ++j) {
        subMatrix[j] = new int[number - 1];
    }


    for (int t = 0; t < number; t++) {
        for (int j = 0; j < number - 1; ++j) {
            for (int k = 0; k < number - 1; ++k) {
                if (k < t)
                    subMatrix[j][k] = matrix[j + 1][k];
                else
                    subMatrix[j][k] = matrix[j + 1][k + 1];
            }
        }
        thread th(Det, subMatrix, number - 1, t);
        th.join();
    }

    delete[] subMatrix;
    for (int i = 0; i < number; i++) {
        if (number == 1) sum += matrix[0][0];
        if (i % 2 == 0)
            sum += matrix[0][i] * result[i];
        else
            sum -= matrix[0][i] * result[i];
    }
    cout << "det => " << sum << endl;
}


int main(int argc, char** argv) {
    if (argc != 2) {
       cout << "Недостаточно или избыток аргументов" << endl;
    }
    int numberOfThreads = stoi(argv[1]);
    int** mas;
    mas = new int* [numberOfThreads];
    srand(time(nullptr));
    for (int i = 0; i < numberOfThreads; i++) {
        mas[i] = new int[numberOfThreads];
        for (int j = 0; j < numberOfThreads; j++) {
            mas[i][j] = rand() % 100;


        }
    }
    PrintMax(mas, numberOfThreads);
    ThreadToCreate(mas, numberOfThreads);
}
