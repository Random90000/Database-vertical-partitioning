#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <unistd.h>

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
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << cohesion_measure; 
    std::ofstream out ("size_"+std::to_string(rows)+"*"+std::to_string(cols)+"_cohesion_"+stream.str(), std::ofstream::out);
    out << rows << " " << cols << "\n";
    Matrix m(rows,cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (positions.find(std::make_pair(i,j)) != positions.end())
            {
                out << 1 << " ";
                m[j][i] = 1;
            }
            else
            {
                out << 0 << " ";
                m[j][i] = 0;
            }
        }
        out << "\n";
    }
    return m;
}

int main(){
    std::ofstream out("algorithm_speed");    
    for (float c : {0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8})
    {
        for (int s : {40,45,50,55})
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
    return 0;
}