#include <iostream>
#include <vector>
#include <random> 

void print_matrix(const std::vector<std::vector<bool>>& matrix){
    if (matrix.empty()) return;
    std::cout << "МАТРИЦА СМЕЖНОСТИ:\n";
    for (size_t i = 0; i < matrix.size(); ++i){
        for (size_t j = 0; j < matrix.size(); ++j){
            std::cout << matrix[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}


std::vector<std::vector<bool>> gen_matrix_s(std::mt19937& gen){
    std::uniform_int_distribution<int> distrib(2, 10); 
    int size = distrib(gen);
    std::vector<std::vector<bool>> matrix(size, std::vector<bool>(size, false));

    for (size_t i = 0; i < size; ++i){
        for (size_t j = 0; j < size; ++j){
            int a = distrib(gen);
            if (i != 0 && j < i) {
                if (matrix[j][i] == true) matrix[i][j] = true;
                else matrix[i][j] = false;
            } else {
                if (i == j) matrix[i][j] = false;
                else matrix[i][j] = static_cast<bool>(a%2);
            }
        }
    }
    return matrix;
}

size_t size_i(const std::vector<std::vector<bool>>& matrix){
    return matrix[0].size();
}

size_t size_s(const std::vector<std::vector<bool>>& matrix){
    size_t size = 0;
    if (matrix.empty() || matrix.size() == 1) return size;
    for (size_t i = 0; i < matrix.size()-1; ++i){
        for (size_t j = i+1; j < matrix.size(); ++j){
            size += matrix[i][j];
        }
    }
    return size;
}

uint8_t sum_e(const std::vector<bool>& v){
    uint8_t sum = 0;
    for (size_t i = 0; i < v.size(); ++i){
        sum += v[i];
    }
    return sum;
}

void type_v(const std::vector<std::vector<bool>>& matrix){
    if (matrix.empty()) return;
    uint8_t sum = 0;
    uint8_t s = 0;

    std::cout << "ПОИСК ОСОБЕННЫХ ВЕРШИН\n";
    for (size_t i = 0; i < matrix.size(); ++i){
        sum = sum_e(matrix[i]);
        if (sum == 0){
            ++s;
            std::cout << "вершина " << i+1 << " ИЗОЛИРОВАНА\n";
        } else if (sum == 1) {
            ++s;
            std::cout << "вершина " << i+1 << " КОНЦЕВАЯ\n";
        } else if (sum == matrix.size()-1) {
            ++s;    
            std::cout << "вершина " << i+1 << " ДОМИНИРУЩАЯ\n";
        }
    }    
    if (s == 0) std::cout << "ВЕРШИНЫ НЕ НАЙДЕНЫ :(\n";
}

std::vector<std::vector<bool>> gen_matrix_i(size_t g_cnt, size_t e_cnt, const std::vector<std::vector<bool>>& matrix_s){
    std::vector<std::vector<bool>> matrix_i(g_cnt, std::vector<bool>(e_cnt, 0));
    
    uint8_t cnt = 0;
    for (size_t i = 0; i < matrix_s.size()-1; ++i){
        for (size_t j = i+1; j < matrix_s.size(); ++j){
            if (matrix_s[i][j]) {
                matrix_i[i][cnt] = true;
                matrix_i[j][cnt] = true;
                ++cnt;    
            }
        }
    }

    return matrix_i;
}

void print_matrix_i(const std::vector<std::vector<bool>>& matrix){
    if (matrix.empty()) return;
    size_t n = matrix.size();
    size_t m = matrix[0].size();
    std::cout << "\nМАТРИЦА ИНЦИНДЕНТНОСТИ\n";
    std::cout << "КОЛИЧЕСТВО ВЕРШИН: " << n << "\n";
    std::cout << "КОЛИЧЕСТВО РЕБЕР: " << m << "\n";

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << "\n";
    }
}




int main(){
    std::random_device rd;
    std::mt19937 gen(rd());

    // task1
    std::vector<std::vector<bool>> g_matrix_s = gen_matrix_s(gen);
    size_t e_cnt = size_s(g_matrix_s);
    size_t g_cnt = g_matrix_s.size();
    std::cout << "КОЛИЧЕСТВО ВЕРШИН: " << g_cnt << "\n";
    std::cout << "РАЗМЕР ГРАФА: " << e_cnt << "\n";
    print_matrix(g_matrix_s);
    type_v(g_matrix_s);

    //task2
    std::vector<std::vector<bool>> g_matrix_i = gen_matrix_i(g_cnt, e_cnt, g_matrix_s);
    print_matrix_i(g_matrix_i);
    std::cout << "\nРАЗМЕР ПО МАТРИЦЕ ИНЦИНДЕНТНОСТИ:\n" << size_i(g_matrix_i) << "\n";
    type_v(g_matrix_i); 
}









