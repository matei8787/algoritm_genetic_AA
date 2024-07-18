#pragma once
#include <vector>
using namespace std;
class Cromozom
{
    vector<bool> biti;
public:
    Cromozom(){}
    Cromozom(vector<bool> biti);
    void ch_bit(int poz);
    vector<bool> get_biti();
    double convert_bits(double st, double dr);
};