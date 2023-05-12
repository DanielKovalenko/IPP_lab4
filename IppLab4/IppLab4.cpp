#include <iostream>
#include <vector>
#include <omp.h>

int main()
{
    const int n = 10;
    const int m = 10;
    std::vector<std::vector<double>> matrix(n, std::vector<double>(m));
    std::vector<double> avg(m);

    // Fill matrix with random values
#pragma omp parallel for collapse(2) //об'єднує два вкладені цикли в один і розподіляє їх ітерації між потоками
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrix[i][j] = (rand() % 100) + 1;
        }
    }

    // Output matrix
    std::cout << " Original Matrix:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    // Compute average of each row and accumulate into the column sums
#pragma omp parallel for
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
#pragma omp atomic
            avg[j] += matrix[i][j] / n;
        }
    }
    std::cout << "\n";

    // Add column averages to each element of the corresponding column
#pragma omp parallel for
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++) {
            matrix[i][j] += avg[j];
        }
    }

    // Output updated matrix
    std::cout << "Updated matrix:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";


    // Output results
    for (int j = 0; j < m; j++) {
        std::cout << "Average of column " << j << ": " << avg[j] << std::endl;
    }

    return 0;
}