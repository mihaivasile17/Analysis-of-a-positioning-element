#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <limits>

int main()
{
    // Declarațiile variabilelor
    float dimensiune;
     do {
         std::cout<<"\nIntroduceti dimensiunea  0 daca vreti sa opriti programul "<<std::endl;
    std::cout << "\nIntroduceti dimensiunea:  ";
    std::cin >> dimensiune;
    if (dimensiune == 0 ) break;
    while (dimensiune >3150 || dimensiune<0)  {
        std::cout << "\nIntroduceti dimensiuni mai mici decat 3150, dar mai mari ca 0" << std::endl;
        std::cout << "\nIntroduceti dimensiunea:  ";
        std::cin >> dimensiune;

    }
         if (dimensiune == 0 ) break;
    float toleranta;
    float As;
    float Ai;
    float as;
    float ai;
    float D1;
    float D2;
    float dim_rotunjita;
    float dmin,dmax,Dmin,Dmax; 
    float Jmax, Jmin;

    // Dmax,Dmin => As, Ai , Dmin = Dnom + Ai , Dmax = Dnom + As  ; dmax,dmin => as, ai , dmin = dnom + ai , dmax = dnom + as
    // Jmin = Ai - as, Jmax = As - ai;

    std::cout << "\nAbaterea inferioara pentru alezaj este: ";
    std::cin >> Ai;
    std::cout << "\nAbaterea superioara pentru alezaj este: ";
    std::cin >> As;
    std::cout<<std::endl;
    toleranta = As - Ai;
    std::cout << "Toleranta este  " << toleranta<<std::endl;
    std::cout << "\nAbaterea inferioara pentru arbore este: ";
    std::cin >> ai;
    std::cout << "\nAbaterea superioara pentru arbore este: ";
    std::cin >> as;

    Dmin = dimensiune + Ai;
    Dmax = dimensiune + As;
    dmin = dimensiune + ai;
    dmax = dimensiune + as;
    Jmin = Ai - as;
    Jmax = As - ai;
    
    // Deschideți fișierul CSV pentru a citi prima linie
    std::ifstream file("tabelIT.csv");

    if (!file.is_open()) {
        std::cerr << "Eroare la deschiderea fisierului!" << std::endl;

        return 1;
    }

    // Ignorați prima linie cu titlurile coloanelor
    std::string line;
    std::getline(file, line);

    // Citirea valorilor de pe prima linie (fără primele două coloane)
    std::istringstream first_line_stream(line);
    std::vector<double> scara_it;
    for (int i = 0; i < 2; ++i) {
        std::getline(first_line_stream, line, ';'); // Ignora primele doua coloane
    }
    while (std::getline(first_line_stream, line, ';')) {
        double valoare;
        std::istringstream val_line(line);
        val_line >> valoare;
        scara_it.push_back(valoare);
    }

    // Închideți fișierul după ce ați citit prima linie
    file.close();

    // Deschideți fișierul CSV pentru a continua cu restul codului
    file.open("tabelIT.csv");

    if (!file.is_open()) {
        std::cerr << "Eroare la deschiderea fisierului!" << std::endl;
        return 1;
    }

    // Ignorați prima linie cu titlurile coloanelor
    std::getline(file, line);

    // Citirea valorilor din fișier
    std::vector<double> val_lim_sup;
    std::vector<std::vector<double>> vector_valori;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        double dim_sup;
        iss >> dim_sup;

        val_lim_sup.push_back(dim_sup);

        // Citirea vectorului de valori de pe acea linie (fără a citi din nou dim_sup)
        std::vector<double> v_valori;
        double valoare;

        // Sărim primele două coloane
        for (int i = 0; i < 2; ++i) {
            std::getline(iss, line, ';');
        }

        // Citirea valorilor începând cu a treia coloană
        while (std::getline(iss, line, ';')) {
            std::istringstream val_line(line);
            val_line >> valoare;
            v_valori.push_back(valoare);
        }

        vector_valori.push_back(v_valori);
    }

    // Închideți fișierul după ce ați citit valorile pentru a determina dim_rotunjita
    file.close();

    // Rotunjirea dimensiunii la următoarea valoare din val_lim_sup
    auto it = std::lower_bound(val_lim_sup.begin(), val_lim_sup.end(), dimensiune);
    if (dimensiune > 2500 && dimensiune <= 3150) {
        dim_rotunjita = 3150.0;
        D1 = 2500.0;
        D2 = dim_rotunjita;
    } else {
        // Determinarea dim_rotunjita
        dim_rotunjita = (it != val_lim_sup.end()) ? *it : 0.0;
        auto index_dim_rotunjita = std::distance(val_lim_sup.begin(), it);
        if (dim_rotunjita == val_lim_sup[0]) {
            D2 = dim_rotunjita;
            D1 = 0;
        } else if (dim_rotunjita != val_lim_sup[0]) {
            D2 = dim_rotunjita;
            D1 = val_lim_sup[index_dim_rotunjita - 1];
        }
    }
    // Deschiderea fișierului pentru a căuta valorile corespunzătoare coloanei a doua
    file.open("tabelIT.csv");

    if (!file.is_open()) {
        std::cerr << "Eroare la deschiderea fisierului!" << std::endl;
        return 1;
    }

    // Ignorați prima linie cu titlurile coloanelor
    std::getline(file, line);

    // Găsirea indexului corespunzător valorii rotunjite
    auto index = std::distance(val_lim_sup.begin(), it);

    // Resetează cursorul la începutul fișierului
    file.clear();
    file.seekg(0);

    // Sărim peste liniile până la linia cu indexul corespunzător
    for (size_t i = 0; i <= index; ++i) {
        std::getline(file, line);
    }

    // Citim linia anterioară (linia corespunzătoare dim_rotunjita - 1)
    std::getline(file, line);

    // Calcularea diferenței minime și actualizarea tol_dif
    double tol_dif = std::numeric_limits<double>::infinity();
    double val_rotunjita;
    int index_valoare_rotunjita = 0;

    for (int i = 0; i < vector_valori[index - 1].size(); ++i) {
        double valoare = vector_valori[index - 1][i];
        double diferenta = std::abs(valoare - toleranta);

        if (diferenta < tol_dif) {
            tol_dif = diferenta;
            val_rotunjita = valoare;
            index_valoare_rotunjita = i;
        }
    }

    // Obțineți elementul corespunzător din scara_it
    double treapta_de_toleranta = scara_it[index_valoare_rotunjita];

    if (treapta_de_toleranta < 5) 
        {
        std::cout << "Nu se calculeaza pentru trepte de toleranta mai mici decat treapta IT-5";
        } 
    else 
        {
        std::cout << "\nD1 = " << D1;
        std::cout << "\nD2 = " << D2;
        std::cout<<std::endl;
        }

    // Afișarea valorii val_rotunjita
    std::cout << "Valoarea rotunjita: " << val_rotunjita << std::endl;

    // Afișarea valorii treapta_de_toleranta
    std::cout << "Treapta de toleranta corespunzatoare este IT-" << treapta_de_toleranta << std::endl;

    std::cout<<std::endl;
    std::cout << "Dmax: " << Dmax << std::endl;
    std::cout << "Dmin: " << Dmin << std::endl;
    std::cout << "dmax: " << dmax << std::endl;
    std::cout << "dmin: " << dmin << std::endl;

    std::cout << "Jocul maxim este: " << Jmax << std::endl;
    std::cout << "Jocul minim este: " << Jmin << std::endl;

        } while (dimensiune<=3150);

}