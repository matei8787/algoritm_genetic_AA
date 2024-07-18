#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <time.h>
#include "global.cpp"

void citeste_functie(istream& in, Functie& fun)
{
    pair<double, double> p;
    in>>p.first>>p.second;
    fun.set_domeniu(p);
    int nrcoeff;
    in>>nrcoeff;
    for ( int i = 1 ; i <= nrcoeff ; i++ )
    {
        double nr;
        in>>nr;
        fun.add_coeff(nr);
    }
}
void citire(istream& in)
{
    in>>Global::nr_populatie;
    citeste_functie(in, Global::f);
    in>>Global::nr_zec_prec>>Global::pcorss>>Global::pmutatie>>Global::nr_etape;
}

void afis_global(ostream& out)
{
    out<<"Nr populatie: "<<Global::nr_populatie<<'\n';
    Global::f.afisare(out);
    out<<"Precizie: "<<Global::nr_zec_prec<<"\nProbabilitate crossover: "<<Global::pcorss
    <<"\nProbabilitate mutatie: "<<Global::pmutatie<<"\nNumarul de etape: "<<Global::nr_etape<<'\n';
}

vector<bool> random_bit_string(int size)
{
    vector<bool> ans;
    for ( int i = 1 ; i <= size ; i++ )
    {
        int aux = rand() % 10;
        if ( aux >= 5 )
            ans.push_back(1);
        else
            ans.push_back(0);
    }
    return ans;
}

void genereaza_cromozomi()
{
    int marime = Global::calc_sz();
    for ( int i = 1 ; i <= Global::nr_populatie ; i++ )
    {
         Cromozom c(random_bit_string(marime));
         Global::cromozomi.push_back(c);
    }
}

double get_functie_cromozom(int poz)
{
    return Global::f.eval(Global::cromozomi[poz].convert_bits(Global::f.get_domeniu().first, Global::f.get_domeniu().second));   
}

int get_maximul()
{
    double ansmax = -1;
    int poz = -1;
    for ( int i = 0 ; i < Global::cromozomi.size() ; i++ )
    {
        double temp = get_functie_cromozom(i);
        if ( ansmax < temp )
        {
            ansmax = temp;
            poz = i;
        }
    }
    return poz;
}

void afis_cromozomi(ostream& out, vector<Cromozom>& cromozomi)
{

    for ( int i = 0 ; i < cromozomi.size() ; i++ )
    {
        out<<i + 1<<" : ";
        for ( bool bit : cromozomi[i].get_biti() )
        {
            out<<bit;
        }
        double numarul = cromozomi[i].convert_bits(Global::f.get_domeniu().first, Global::f.get_domeniu().second);
        out<<"   x = "<<numarul;
        out<<"   f = "<<Global::f.eval(numarul);
        out<<'\n';
    }
}

double get_suma_cromozomi()
{
    double ans = 0;
    for ( int i = 0 ; i < Global::cromozomi.size() ; i++ )
    {
        ans += get_functie_cromozom(i);
    }
    return ans;
}

vector<double> afis_prob_selectie(ostream& out, bool afisez)
{
    double sum = get_suma_cromozomi();
    vector<double> probabilitati;
    if ( afisez )
        out<<"Probabilitati de selectie\n";
    for ( int i = 0 ; i < Global::cromozomi.size() ; i++ )
    {
        probabilitati.push_back(get_functie_cromozom(i)/sum);
        if ( afisez )
            out<<"cromozomul "<<i+1<<" probabilitate "<<(probabilitati[probabilitati.size()-1])<<'\n';
    }
    return probabilitati;
}

vector<double> make_sir_prob_selectie(vector<double> probabilitati)
{
    double s = 0;
    vector<double> ans;
    for ( int i = 0 ; i < probabilitati.size() ; i++ )
    {
        ans.push_back(s);
        s += probabilitati[i];
    }
    ans.push_back(s);
    return ans;
}
void afis_vector_d(ostream& out, vector<double> &v)
{
    for ( int i = 0 ; i < v.size() ; i++ )
    {
        out<<v[i]<<" ";
    }
    out<<'\n';
}

int get_indice_cromozom(double probabilitate, vector<double>& sir_prob)
{
    int st = 0, dr = sir_prob.size();
    while ( st < dr )
    {
        int mij = ( st + dr ) / 2;
        if ( sir_prob[mij-1] <= probabilitate && probabilitate <= sir_prob[mij] )
        {
            return mij;
        }
        if ( sir_prob[mij-1] > probabilitate )
        {
            dr = mij - 1;
        }
        else
        {
            st = mij + 1;
        }
    }
    if ( st == 0 )
        return 1;
    if ( st >= Global::cromozomi.size() )
        return Global::cromozomi.size() - 1;
    return st;
}

double fRand(double fMin = 0, double fMax = 1)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

int selecteaza_random(vector<double>& sir_prob, bool afisez)
{
    double u = fRand();
    if ( afisez )
    cout<<"u="<<u<<" ";
    return get_indice_cromozom(u, sir_prob);
}

vector<Cromozom> selecteaza_cromozomi(vector<double>& sir_prob, bool afisez)
{
    vector<Cromozom> ans;
    for ( int i = 1 ; i <= Global::nr_populatie - 1 ; i++ )
    {
        int idx = selecteaza_random(sir_prob, afisez);
        if ( afisez )
            cout<<"selectam cromozomul "<<idx<<'\n';
        ans.push_back(Global::cromozomi[idx-1]);
    }
    ans.push_back(Global::cromozomi[get_maximul()]);
    return ans;
}

vector<pair<int, int>> selecteaza_crossover(ostream& out, vector<Cromozom>& vc, bool afisez)
{
    vector< pair<int, int> > ans;
    vector<int> temp;
    for ( int i = 0 ; i < vc.size() ; i++ )
    {
        if ( afisez )
        out<<i+1<<" : ";
        for ( bool bit : vc[i].get_biti() )
        {
            if ( afisez )
                out<<bit;
        }
        double u = fRand();
        if ( afisez )
            out<<"u=  "<<u;
        if ( u < Global::pcorss )
        {
            if ( afisez )
                out<<"<"<<Global::pcorss<<" participa";
            temp.push_back(i);
        }
        if ( afisez )
            out<<'\n';
    }
    auto rng = default_random_engine{};
    shuffle(temp.begin(), temp.end(), rng);
    for ( int i = 0 ; i < temp.size() - 1 ; i += 2 )
    {
        ans.push_back(make_pair(temp[i], temp[i+1]));
    }
    return ans;
}

pair<Cromozom, Cromozom> combina(Cromozom a, Cromozom b, int punct)
{
    vector<bool> va, vb;
    for ( int i = 0 ; i < punct ; i++ )
    {
        va.push_back(b.get_biti()[i]);
        vb.push_back(a.get_biti()[i]);
    }
    for ( int i = punct ; i < a.get_biti().size() ; i++ )
    {
        va.push_back(a.get_biti()[i]);
        vb.push_back(b.get_biti()[i]);
    }
    Cromozom auxa(va);
    Cromozom auxb(vb);
    pair<Cromozom, Cromozom> ans(auxa, auxb);
    return ans;
}

void cross(ostream& out, vector<pair<int, int>> care_cu_care, vector<Cromozom>& vc, bool afisez)
{
    for ( int i =  0 ; i < care_cu_care.size() ; i++ )
    {
        pair<int, int> p = care_cu_care[i];
        int punct_de_rupere = rand() % Global::calc_sz();
        if ( afisez ){
            out<<"Recombinare dintre cromozomul "<<p.first + 1<<" cu cromozomul "<<p.second + 1<<":\n";
            for ( bool bit : vc[p.first].get_biti() )
            {
                out<<bit;
            }
            out<<" ";
            for ( bool bit : vc[p.second].get_biti() )
            {
                out<<bit;
            }
            out<<"  punct  "<<punct_de_rupere<<'\n';
        }
        pair<Cromozom, Cromozom> pcc = combina(vc[p.first], vc[p.second], punct_de_rupere);
        if ( afisez )
        {
            out<<"Rezultat\t";
            for ( bool bit : pcc.first.get_biti() )
            {
                out<<bit;
            }
            out<<" ";
            for ( bool bit : pcc.second.get_biti() )
            {
                out<<bit;
            }
            out<<'\n';
        }
        vc[p.first] = pcc.first;
        vc[p.second] = pcc.second;
    }
}

void afis_dupa_cross(ostream& out, vector<Cromozom>& vc)
{
    out<<"Dupa recombinare:\n";
    afis_cromozomi(out, vc);
    out<<'\n';
}

void muteaza(ostream& out, vector<Cromozom>& vc, bool afisez)
{
    if ( afisez )
    {
        out<<"Probabilitate de mutatie pentru fiecare gena "<<Global::pmutatie<<'\n';
    }
    vector<int> care;
    for ( int i = 0 ; i < vc.size() ; i++ )
    {
        bool am_pus = false;
        for ( int j = 0 ; j < vc[i].get_biti().size() ; j++ )
        {
            double u = fRand();
            if ( u < Global::pmutatie )
            {
                if ( !am_pus )
                {
                    care.push_back(i);
                    am_pus = true;
                }
                vc[i].ch_bit(j);
            }
        }
    }
    if ( afisez )
    {
        if ( care.size() > 0 )
            cout<<"Au fost modificati cromozomii:\n";
        for ( int i = 0 ; i < care.size() ; i++ )
        {
            cout<<care[i] + 1<<'\n';
        }
    }
}
void afis_dupa_mutatie(ostream& out, vector<Cromozom>& vc, bool afisez)
{
    out<<"Dupa mutatie:\n";
    afis_cromozomi(out, vc);
}

void schimba_cromozomii(vector<Cromozom>& nou)
{
    Global::cromozomi.clear();
    Global::cromozomi.resize(0);
    for ( int i = 0 ; i < nou.size() ; i++ )
    {
        Global::cromozomi.push_back(nou[i]);
    }
}

void etapa1()
{
    afis_cromozomi(cout, Global::cromozomi);
    vector<double> sir_prob = make_sir_prob_selectie(afis_prob_selectie(cout, true));
    cout<<"Intervale probabilitati selectie: \n";
    afis_vector_d(cout, sir_prob);
    vector<Cromozom> aux = selecteaza_cromozomi(sir_prob, true);
    cout<<"Dupa selectie:\n";
    afis_cromozomi(cout, aux);
    vector<pair<int, int>> selectare_cross = selecteaza_crossover(cout, aux, true);
    cross(cout, selectare_cross, aux, true);
    afis_dupa_cross(cout, aux);
    muteaza(cout, aux, true);
    afis_dupa_mutatie(cout, aux, true);
    schimba_cromozomii(aux);
}



void afis_dupa_etapa()
{
    cout<<"Maximul: ";
    double ansmax = -1;
    double suma = 0;
    for ( int i = 0 ; i < Global::cromozomi.size() ; i++ )
    {
        double temp = get_functie_cromozom(i);
        ansmax = max(ansmax, temp);
        suma += temp;
    }
    cout<<ansmax<<"\nAvg: "<<(suma/Global::nr_populatie)<<'\n';
}

void etapa()
{
    vector<double> sir_prob = make_sir_prob_selectie(afis_prob_selectie(cout, false));
    vector<Cromozom> aux = selecteaza_cromozomi(sir_prob, false);
    vector<pair<int, int>> selectare_cross = selecteaza_crossover(cout, aux, false);
    cross(cout, selectare_cross, aux, false);
    muteaza(cout, aux, false);
    schimba_cromozomii(aux);
    afis_dupa_etapa();
}

int main()
{
    srand(time(NULL));
    citire(cin);
    genereaza_cromozomi();
    etapa1();
    cout<<'\n';
    for ( int i = 2 ; i <= Global::nr_etape ; i++ )
    {
        etapa();
    }
}