#include "functie.cpp"
#include "cromozom.cpp"
#include <math.h>


namespace Global
{
    Functie f;
    vector<Cromozom> cromozomi;
    int nr_populatie, nr_zec_prec, nr_etape;
    double pcorss, pmutatie;
    int calc_sz()
    {
        int ans = (int)log2((f.get_domeniu().second - f.get_domeniu().first) * 10) * nr_zec_prec;
        return ans;
    }
}