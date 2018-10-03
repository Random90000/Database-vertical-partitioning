#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

using namespace testing;

class BinString
{
public:
    BinString();
    BinString(const std::string str);
    ~BinString(){}

    std::string   get_binary();
    unsigned long get_decimal();
    size_t        get_size();
    int           get_base();

    void          push(int number);
    void          set(size_t idx, int number);
    int           get(size_t idx);

    void          inc();

private:
    std::string value;
};

BinString::BinString()
{
    value = "0";
}

BinString::BinString(const std::string str)
{
    value = str;
}

std::string BinString::get_binary()
{
    return this->value;
}

size_t BinString::get_size()
{
    return this->value.size();
}

void BinString::push(int number)
{
    if (number == 1)
    {
        this->value.push_back('1');
    }
    if (number == 0)
    {
        this->value.push_back('0');
    }
}

void BinString::set(size_t idx, int number)
{
    if (number == 1)
    {
        this->value[idx] = '1';
    }
    if (number == 0)
    {
        this->value[idx] = '0';
    }
}

int BinString::get(size_t idx)
{
    return (this->value[idx] == '1') ? 1 : 0;
}

void BinString::inc ()
{
    if (value[value.size() - 1] == '0')
    {
        value[value.size() - 1] = '1';
    }
    else
    {
        for (std::string::reverse_iterator it = value.rbegin(); it != value.rend(); it++)
        {
            if ((it == value.rend() - 1) && (*it == '1'))
            {
                *it = '0';
                value.insert(value.begin(), '1');
            }
            else if (*it == '1')
            {
                *it = '0';
            }
            else
            {
                *it = '1';
                break;
            }
        }
    }
}
