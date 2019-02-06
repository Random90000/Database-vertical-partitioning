#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <unistd.h>
#include <ctime>

#include "measures.hpp"
#include "matrix.hpp"
#include "bab11.hpp"

Matrix generate_random(int rows, int cols, float cohesion_measure)
{
    std::set<std::pair<int,int>> positions;
    int                          non_zero_count = cohesion_measure*rows*cols;
    for (int i = 0; i < non_zero_count; i++)
    {
        int pos_i = rand()%rows;
        int pos_j = rand()%cols;
        while(positions.find(std::make_pair(pos_i,pos_j)) != positions.end())
        {
            pos_i = rand()%rows;
            pos_j = rand()%cols;
        }
        positions.insert(std::make_pair(pos_i,pos_j));
    }
    Matrix m(rows,cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (positions.find(std::make_pair(i,j)) != positions.end())
            {
                m[j][i] = 1;
            }
            else
            {
                m[j][i] = 0;
            }
        }
    }
    return m;
}

int main(){
    std::ofstream out_speed("algorithm_speed", std::ios::out | std::ios::app);
    std::string parameters_directory = "";
    std::cin >> parameters_directory;
    std::ifstream in(parameters_directory);    
    std::cout << "is_open: " << in.is_open() << "\n"; 
    float cohesion = 0;
    int   rows = 0;
    int   cols = 0;
    while(in >> cohesion >> rows >> cols)
    {
        Matrix m = generate_random(rows,cols,cohesion);
        auto start = std::chrono::steady_clock::now();
        std::time_t t = std::time(nullptr);
        std::cout << "size: " << rows << "*" << cols << " cohesion: " << cohesion << " started: " << std::ctime(&t);
        std::cout.flush();
        auto sol = Bab11(m);
        auto end = std::chrono::steady_clock::now();
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << cohesion; 
        std::ofstream out_data("size_"+std::to_string(rows)+"*"+std::to_string(cols)+"_cohesion_"+stream.str(),std::ios::out | std::ios::app);
        out_data << rows << " " << cols << "\n" << m << "\n\n" << "clusters :\n";
        out_data.flush();
        if (sol.clusters.size() == 0)
        {
            std::cout << " time: > 2 hours\n";
            out_speed << "size: " << rows << "*" << cols << " cohesion: " << cohesion << " time: > 2 hours\n";
            out_speed.flush();
            break;
        }
        else
        {
            std::cout << " time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "\n";
            out_speed << "size: " << rows << "*" << cols << " cohesion: " << cohesion << " time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "\n";
            out_speed.flush();
            for (auto i : sol.clusters)
            {
                out_data << i << "\n";
                out_data.flush();
            }
        }
        out_data.close();
    }
    /*
    for (float c : {0.7,0.8})//0.2 85-100 //0.8 60 //0.7 95-100
    {
        for (int s : {65,70,75,80,85,90,95,100})
        {
            Matrix m = generate_random(s,20,c);
            auto start = std::chrono::steady_clock::now();
            auto sol = Bab11(m);
            auto end = std::chrono::steady_clock::now();
            for (auto i : sol.clusters)
            {
                std::cout << i << "\n";
            }
            if (sol.clusters.size() == 0)
            {
                std::cout << "size: " << s << " cohesion: " << c << " time: > 2 hours\n";
                out << "size: " << s << " cohesion: " << c << " time: > 2 hours\n";
                out.flush();
                break;
            }
            else
            {
                std::cout << "size: " << s << " cohesion: " << c << " time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "\n";
                out << "size: " << s << " cohesion: " << c << " time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "\n";
                out.flush();
            }    
        }
    }
    */   
    return 0;
}