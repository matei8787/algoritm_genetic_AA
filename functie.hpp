#pragma once
#include <vector>
#include <iostream>

class Functie
{
    std::vector<double> coeff; ///v[0] + x * v[1] + x^2 * v[2] + ...
    std::pair<double, double> domeniu;
public:
    Functie(){}
    void set_domeniu(std::pair<double, double> p);
    void add_coeff(double c);
    double get_coeff(int poz);
    std::pair<double, double> get_domeniu();
    double eval(double x);
    void afisare(std::ostream& out);
};