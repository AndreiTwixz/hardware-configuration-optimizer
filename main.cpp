#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

class Echipament_electronic {
protected:
    float pret;
    std::string nume;
public:
    Echipament_electronic(float p, std::string n) : pret(p), nume(n) {
    }

    virtual ~Echipament_electronic() {
    }

    virtual void afisare() const = 0;

    float getPret() const {
        return pret;
    }

    std::string getNume() const {
        return nume;
    }
};

class Placa_baza : public Echipament_electronic {
    std::string socket;
public:
    Placa_baza(float p, std::string n, std::string s) : Echipament_electronic(p, n), socket(s) {
    }

    void afisare() const override {
        std::cout << "placa_baza, " << nume << ", " << socket << ", " << pret << std::endl;
    }

    std::string getSocket() const {
        return socket;
    }
};

class CPU : public Echipament_electronic {
    std::string socket;
    int frecventa;
public:
    CPU(float p, std::string n, std::string s, int f) : Echipament_electronic(p, n), socket(s), frecventa(f) {
    }

    void afisare() const override {
        std::cout << "cpu, " << nume << ", " << socket << ", " << frecventa << ", " << pret << std::endl;
    }

    std::string getSocket() const {
        return socket;
    }

    float getCalitate() const {
        return (float)frecventa;
    }

    float getRaport() const {
        return (float)frecventa / pret;
    }
};

class GPU : public Echipament_electronic {
    int memorie;
public:
    GPU(float p, std::string n, int m) : Echipament_electronic(p, n), memorie(m) {
    }

    void afisare() const override {
        std::cout << "gpu, " << nume << ", " << memorie << ", " << pret << std::endl;
    }

    float getCalitate() const {
        return (float)memorie;
    }

    float getRaport() const {
        return (float)memorie / pret;
    }
};

class Stocare : public Echipament_electronic {
    float memorie;
public:
    Stocare(float p, std::string n, float m) : Echipament_electronic(p, n), memorie(m) {
    }

    void afisare() const override {
        std::cout << "stocare, " << nume << ", " << memorie << ", " << pret << std::endl;
    }

    float getCalitate() const {
        return memorie;
    }

    float getRaport() const {
        return memorie / pret;
    }
};

class PC {
public:
    Placa_baza* mb;
    CPU* cpu;
    GPU* gpu;
    Stocare* stocare;

    PC() : mb(nullptr), cpu(nullptr), gpu(nullptr), stocare(nullptr) {
    }

    void afisareConfiguratie() const {
        mb->afisare();
        cpu->afisare();
        gpu->afisare();
        stocare->afisare();
        std::cout << (mb->getPret() + cpu->getPret() + gpu->getPret() + stocare->getPret()) << std::endl;
    }
};

class Manager {
    Placa_baza* mb = nullptr;
    std::vector<CPU*> cpus;
    std::vector<GPU*> gpus;
    std::vector<Stocare*> stocari;

public:
    ~Manager() {
        delete mb;
        
        for (auto x : cpus) {
            delete x;
        }
        
        for (auto x : gpus) {
            delete x;
        }
        
        for (auto x : stocari) {
            delete x;
        }
    }

    void citire() {
        int n;
        std::cin >> n;
        
        for (int i = 0; i < n; i++) {
            float p;
            std::string num;
            std::string t;
            
            std::cin >> p >> num >> t;
            
            if (t == "placa_baza") {
                std::string s;
                std::cin >> s;
                mb = new Placa_baza(p, num, s);
            } else if (t == "cpu") {
                std::string s;
                int f;
                std::cin >> s >> f;
                cpus.push_back(new CPU(p, num, s, f));
            } else if (t == "gpu") {
                int m;
                std::cin >> m;
                gpus.push_back(new GPU(p, num, m));
            } else if (t == "stocare") {
                float m;
                std::cin >> m;
                stocari.push_back(new Stocare(p, num, m));
            }
        }
    }

    void afisare() {
        if (mb) {
            mb->afisare();
        }
        
        for (auto x : cpus) {
            x->afisare();
        }
        
        for (auto x : gpus) {
            x->afisare();
        }
        
        for (auto x : stocari) {
            x->afisare();
        }
    }

    void configurare_manuala() {
        std::string nc;
        std::string ng;
        std::string ns;
        
        std::cin >> nc >> ng >> ns;
        
        PC pc;
        pc.mb = mb;
        
        for (auto x : cpus) {
            if (x->getNume() == nc) {
                pc.cpu = x;
            }
        }
        
        for (auto x : gpus) {
            if (x->getNume() == ng) {
                pc.gpu = x;
            }
        }
        
        for (auto x : stocari) {
            if (x->getNume() == ns) {
                pc.stocare = x;
            }
        }
        
        if (pc.mb->getSocket() != pc.cpu->getSocket()) {
            std::cout << "Placa de baza este incompatibila cu CPU ales." << std::endl;
            exit(0);
        }
        
        pc.afisareConfiguratie();
    }

    void configurare_optima() {
        int buget;
        std::cin >> buget;
        
        std::vector<CPU*> compatibili;
        
        for (auto x : cpus) {
            if (x->getSocket() == mb->getSocket()) {
                compatibili.push_back(x);
            }
        }

        auto sortG = [](GPU* a, GPU* b) {
            return a->getRaport() < b->getRaport();
        };
        auto sortC = [](CPU* a, CPU* b) {
            return a->getRaport() < b->getRaport();
        };
        auto sortS = [](Stocare* a, Stocare* b) {
            return a->getRaport() < b->getRaport();
        };

        std::sort(gpus.begin(), gpus.end(), sortG);
        std::sort(compatibili.begin(), compatibili.end(), sortC);
        std::sort(stocari.begin(), stocari.end(), sortS);

        int iG = 0;
        int iC = 0;
        int iS = 0;

        for (int i = 1; i < gpus.size(); i++) {
            if (gpus[i]->getPret() < gpus[iG]->getPret()) {
                iG = i;
            }
        }
        
        for (int i = 1; i < compatibili.size(); i++) {
            if (compatibili[i]->getPret() < compatibili[iC]->getPret()) {
                iC = i;
            }
        }
        
        for (int i = 1; i < stocari.size(); i++) {
            if (stocari[i]->getPret() < stocari[iS]->getPret()) {
                iS = i;
            }
        }

        while (true) {
            bool up = false;
            float curent = mb->getPret() + gpus[iG]->getPret() + compatibili[iC]->getPret() + stocari[iS]->getPret();
            
            for (int i = 0; i < gpus.size(); i++) {
                if (gpus[i]->getRaport() > gpus[iG]->getRaport()) {
                    if (curent - gpus[iG]->getPret() + gpus[i]->getPret() <= buget) {
                        iG = i;
                        up = true;
                        break;
                    }
                }
            }
            
            if (!up) {
                for (int i = 0; i < compatibili.size(); i++) {
                    if (compatibili[i]->getRaport() > compatibili[iC]->getRaport()) {
                        if (curent - compatibili[iC]->getPret() + compatibili[i]->getPret() <= buget) {
                            iC = i;
                            up = true;
                            break;
                        }
                    }
                }
            }
            
            if (!up) {
                for (int i = 0; i < stocari.size(); i++) {
                    if (stocari[i]->getRaport() > stocari[iS]->getRaport()) {
                        if (curent - stocari[iS]->getPret() + stocari[i]->getPret() <= buget) {
                            iS = i;
                            up = true;
                            break;
                        }
                    }
                }
            }
            
            if (!up) {
                break;
            }
        }
        
        PC pc;
        pc.mb = mb;
        pc.gpu = gpus[iG];
        pc.cpu = compatibili[iC];
        pc.stocare = stocari[iS];
        
        pc.afisareConfiguratie();
    }
};

int main() {
    Manager m;
    std::string cmd;
    
    while (std::cin >> cmd) {
        if (cmd == "iesire") {
            break;
        }
        
        if (cmd == "citire") {
            m.citire();
        } else if (cmd == "afisare") {
            m.afisare();
        } else if (cmd == "configurare_manuala") {
            m.configurare_manuala();
        } else if (cmd == "configurare_optima") {
            m.configurare_optima();
        }
    }
    
    return 0;
}