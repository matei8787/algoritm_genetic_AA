#pragma once
#include "cromozom.hpp"

Cromozom::Cromozom(vector<bool> biti)
{
    this -> biti = biti;
}
double Cromozom::convert_bits(double st, double dr)
{
    int plm = 0;
    for ( int i = 0 ; i < biti.size() ; i++ )
    {
        plm += (int)biti[i] * (1<<i);
    }
    double ans = plm * ((dr - st) / ((1<<biti.size()) - 1)) + st;
    return ans;
}
void Cromozom::ch_bit(int poz)
{
    biti[poz] = !biti[poz];
}
vector<bool> Cromozom::get_biti()
{
    return biti;
}