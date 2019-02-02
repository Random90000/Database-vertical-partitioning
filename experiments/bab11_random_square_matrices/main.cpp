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

Matrix generate_random(int size, float cohesion_measure)
{
    std::set<std::pair<int,int>> positions;
    int                          non_zero_count = cohesion_measure*size*size;
    for (int i = 0; i < non_zero_count; i++)
    {
        int pos_i = rand()%size;
        int pos_j = rand()%size;
        while(positions.find(std::make_pair(pos_i,pos_j)) != positions.end())
        {
            pos_i = rand()%size;
            pos_j = rand()%size;
        }
        positions.insert(std::make_pair(pos_i,pos_j));
    }
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << cohesion_measure; 
    std::ofstream out ("size_"+std::to_string(size)+"_cohesion_"+stream.str(), std::ofstream::out);
    out << size << "\n";
    Matrix m(size,size);
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
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
    for (float c : {0.7,0.8})
    {
        for (int s : {5,10,15,20,25,30,35})
        {
            Matrix m = generate_random(s,c);
            auto start = std::chrono::steady_clock::now();
            auto sol = Bab11(m);
            auto end = std::chrono::steady_clock::now();
            for (auto i : sol.clusters)
            {
                std::cout << i << "\n";
            }
            if (sol.clusters.size() == 0)
            {
                std::cout << "size: " << s << " cohesion: " << c << " time: > 2 hours";
                out << "size: " << s << " cohesion: " << c << " time: > 2 hours";
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