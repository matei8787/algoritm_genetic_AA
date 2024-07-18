#pragma once
#include "functie.hpp"

void Functie::add_coeff(double c)
{
    coeff.push_back(c);
}
void Functie::set_domeniu(std::pair<double, double> p)
{
    domeniu = p;
}
double Functie::get_coeff(int poz)
{
    return coeff[poz];
}
std::pair<double, double> Functie::get_domeniu()
{
    return domeniu;
}

double Functie::eval(double x)
{
    double ans = 0;
    double mul = 1;
    for ( int i = 0 ; i < coeff.size() ; i++ )
    {
        ans += coeff[i] * mul;
        mul *= x;
    }
    return ans;
}

void Functie::afisare(std::ostream& out)
{
    out<<"Definita pe intervalul ["<<domeniu.first<<", "<<domeniu.second<<"]\n";
    out<<coeff[0];
    for ( int i = 1 ; i < coeff.size() ; i++ )
    {
        if ( coeff[i] >= 0 )
            out<<" +";
        out<<coeff[i]<<"*x^"<<i;
    }
    out<<'\n';
}