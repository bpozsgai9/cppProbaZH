#include <iostream>
#include <string>
#include <cassert>
#include <utility>

using namespace std;

/////////////////////////
//Ide dolgozz!!

class Jatek {
private:
    string nev;
    unsigned tarhely_igeny;
    unsigned firmware_igeny;

public:
    Jatek(const string nev, unsigned int tarhely_igeny, unsigned int firmware_igeny) :
    nev(nev), tarhely_igeny(tarhely_igeny), firmware_igeny(firmware_igeny) {}

    Jatek() :nev(""), tarhely_igeny(0), firmware_igeny(0) {}

    const string &get_nev() const {
        return nev;
    }

    unsigned int get_tarhely_igeny() const {
        return tarhely_igeny;
    }

    unsigned int get_firmware_igeny() const {
        return firmware_igeny;
    }

    bool operator == (const Jatek& j) const {

        if (this->nev == j.get_nev() &&
            this->firmware_igeny == j.get_firmware_igeny() &&
            this->tarhely_igeny == j.get_tarhely_igeny()) {

            return true;

        } else {

            return false;
        }
    }
};

//sajat hiba
class Hiba : public exception {
private:
    int hibakod;

public:
    Hiba(int hibakod) : hibakod(hibakod) {}

    int get_hibakod() const {
        return hibakod;
    }
};

class Jatekplatform {
private:
    unsigned max_tarhely;
    unsigned felhasznalt_tarhely;
    Jatek* jatekok;
    unsigned jatek_szam;
    unsigned firmware;

public:
    //konstruktor
    Jatekplatform(unsigned int max_tarhely, unsigned int firmware) :
                  max_tarhely(max_tarhely),
                  firmware(firmware),
                  felhasznalt_tarhely(0),
                  jatekok(nullptr),
                  jatek_szam(0) {}

    Jatekplatform(const Jatekplatform& jatekplatform) {
        this-> max_tarhely = jatekplatform.max_tarhely;
        this->firmware = jatekplatform.firmware;
        this->felhasznalt_tarhely = jatekplatform.felhasznalt_tarhely;

        this->jatek_szam = jatekplatform.jatek_szam;
        this->jatekok = new Jatek[this->jatek_szam];

        for (int i = 0; i < this->jatek_szam; ++i) {
            this->jatekok[i] = jatekplatform.jatekok[i];
        }
    }

    //destruktor
    virtual ~Jatekplatform() {
        if (this->jatekok != nullptr) {
            delete []jatekok;
        }
    }

    Jatek *get_jatekok() const {
        return jatekok;
    }

    unsigned int get_jatek_szam() const {
        return jatek_szam;
    }

    unsigned int get_firmware() const {
        return firmware;
    }

    void set_jatekok(Jatek *jatekok) {
        Jatekplatform::jatekok = jatekok;
    }

    void set_jatek_szam(unsigned int jatek_szam) {
        Jatekplatform::jatek_szam = jatek_szam;
    }

    void set_firmware(unsigned int firmware) {
        Jatekplatform::firmware = firmware;
    }

    //prefix, ++i
    Jatekplatform& operator++ () {
        this->firmware++;
        return *this;
    }

    //postfix, i++
    Jatekplatform operator++ (int) {
        Jatekplatform local = *this;
        operator++();
        return local;
    }

    //+=
    virtual Jatekplatform& operator += (const Jatek& j) {

        for (int i = 0; i < this->jatek_szam; ++i) {
            if (this->jatekok[i] == j) {
                throw Hiba(1);
            }
        }

        if (this->max_tarhely - this->felhasznalt_tarhely < j.get_tarhely_igeny()) {
            throw Hiba(2);
        }

        if (this->firmware < j.get_firmware_igeny()) {
            throw Hiba(3);
        }

        Jatek* ujJatekTomb = new Jatek[this->jatek_szam + 1];

        for (int i = 0; i < jatek_szam; ++i) {
            ujJatekTomb[i] = this->jatekok[i];
        }
        ujJatekTomb[jatek_szam] = j;
        this->jatek_szam++;
        this->felhasznalt_tarhely += j.get_tarhely_igeny();

        //kell a delete az elemek törlése miatt
        delete []this->jatekok;
        this->jatekok = ujJatekTomb;

        return *this;
    }

    //ertekadas
    Jatekplatform& operator = (const Jatekplatform& jatekplatform) {
        if (this == &jatekplatform) {
            return *this;
        }

        this-> max_tarhely = jatekplatform.max_tarhely;
        this->firmware = jatekplatform.firmware;
        this->felhasznalt_tarhely = jatekplatform.felhasznalt_tarhely;

        this->jatek_szam = jatekplatform.jatek_szam;
        delete[] this->jatekok;
        this->jatekok = new Jatek[this->jatek_szam];

        for (int i = 0; i < this->jatek_szam; ++i) {
            this->jatekok[i] = jatekplatform.jatekok[i];
        }

        return *this;
    }
};

class Playstation : public Jatekplatform {
private:
    bool pro;

public:
    bool get_pro() const {
        return pro;
    }

    Playstation(unsigned int max_tarhely, unsigned int firmware, bool pro) :
    Jatekplatform(max_tarhely, firmware), pro(pro) {}

    Playstation& operator += (const Jatek& jatek) override {
        if (!this->pro) {
            Jatekplatform::operator+=(jatek);
        } else {
            try {
                Jatekplatform::operator+=(jatek);
            } catch (Hiba& hiba) {
                if (hiba.get_hibakod() == 3) {
                    this->set_firmware(jatek.get_firmware_igeny());
                    Jatekplatform::operator+=(jatek);
                } else {
                    throw hiba;
                }
            }
        }
        return *this;
    }
};


////////////////////////

//=== Teszteles bekapcsolasa kikommentezessel
//#define TEST_jatek1
//#define TEST_jatek2
//#define TEST_egyenloseg
//#define TEST_jatekplatform
//#define TEST_hiba
//#define TEST_operator_plusz
//#define TEST_copy_ctor
//#define TEST_ertekadas_operator
//#define TEST_playstation
//#define TEST_operator_plusz_ps
//=== Teszteles bekapcsolas vege


/*
Készíts egy Jatek osztályt szoftveres játékok reprezentálására. Adattagok:
- nev: a játék neve (string)
- tarhely_igeny: a játék mérete (unsigned)
- firmware_igeny: a firmware szoftver verzió (unsigned)

Rendelkezzen default konstruktorral is, mely a sztringet üresen hagyja a két unsigned értéket pedig 0-ra állítja be.
*/
void test_jatek1(){
  #if defined TEST_jatek1 && !defined TEST_BIRO
  const Jatek j;
  assert(j.get_firmware_igeny() == 0);
  assert(j.get_tarhely_igeny() == 0);
  #endif
}

/*
Rendelkezzen egy paraméteres konstruktorral, amely egy sztring és két unsigned paramétert vár és inicializálja velük a három adattagot. 
A paraméterek sorrendje: név, tárhely, firmware
*/
void test_jatek2(){
  #if defined TEST_jatek2 && !defined TEST_BIRO
  const Jatek j("Uncharted", 100, 2);
  assert(j.get_firmware_igeny() == 2);
  assert(j.get_tarhely_igeny() == 100);
  #endif
}

/*
Valósítsd meg a == operátort a Jatek osztályra. Akkor adjon vissza igazat, ha mindhárom adattag megegyezik, egyébként hamisat. 
*/
void test_egyenloseg(){
  #if defined TEST_egyenloseg && !defined TEST_BIRO
  const Jatek j1("Uncharted", 120, 1), j2("Uncharted", 110, 1);
  bool result = j1 == j2;
  assert(!result);
  #endif
}


/*
Készíts egy Jatekplatform nevű osztályt! Adattagok:

- max_tarhely: Tárhelyének mérete & unsigned &
- felhasznalt_tarhely: Felhasznált tárhely & unsigned &
- jatekok: Installált játékok listája & Jatek* &
- jatek_szam: Telepített játékok száma & unsigned &
- firmware: Firmware verzió & unsigned &


A játékplatform egy dinamikusan változó méretű tömbben tárolja az aktuálisan telepített játékok listáját (jatekok adattag) 
A tömb kezdteben üres (nullptr) és amint egy játék telepítésre kerül meg kell növelni a tömb méretét eggyel.

Legyen az osztálynak egy paraméteres konstruktora, mely két unsigned értéket vár és inicializálja vele a max_tarhely és firmware értékeket. 
A tömb kezdetben üres, a jatek_szam és felhasznalt_tarhely értelemszerűen nulla.


Legyen az osztálynak destruktora amely felszabadítja a jatekok tömböt, ha nem üres.
*/
void test_jatekplatform(){
  #if defined TEST_jatekplatform && !defined TEST_BIRO
  const Jatekplatform jp(100, 3);
  assert(jp.get_firmware() == 3);
  assert(jp.get_jatek_szam() == 0);
  assert(jp.get_jatekok() == nullptr);
  #endif
}

/*
Valósítsd meg a prefix és postfix ++ operátort a Jatekplatform osztály számára. Az operátorok firmware upgrade-t végeznek, vagyis eggyel növelik a firmware adattag értékét.
*/
void test_prefix_postfix(){
  #if defined TEST_prefix_postfix && !defined TEST_BIRO
  Jatekplatform jp(200, 2);
  jp++;
  assert(jp.get_firmware() == 3);
  ++jp;
  assert(jp.get_firmware() == 4);
  #endif
}

/*
Készíts egy Hiba nevű osztály, mely publikusan öröklődik az std::exception osztályból. Ez az osztály lesz dobva bármilyen hiba esetén. Egy darab adattagja van:

- hibakod: a hibeüzenet (int)

Készíts neki egy konstruktort, amely egy int-et vár a hibakod beállítására.
*/
void test_hiba(){
  #if defined TEST_hiba && !defined TEST_BIRO
  const Hiba h(5);
  assert(h.get_hibakod() == 5);
  #endif
}


/*
Valósítsd meg a Jatekplatform osztályra a += operátort úgy, hogy paramétere egy Jatek-ra mutató konstans referencia legyen. 
Az operátor feladata, hogy megvizsgálja, telepíthető-e a játék és ha igen akkor megnöveli a jatekok tömb méretét eggyel és eltárolja az új játékot. 
A telepíthetőséghez ezeket kell vizsgálni (ebben a sorrendben):
- Először ellenőrizze szerepel-e már az a játék a telepített játékok listájában. Ha igen, legyen egy Hiba kivétel dobva 1-es hibakóddal.
- Ha még nincs telepítve akkor a tárhelyet kell ellenőrizni. Ha nincs elég hely a játékot telepíteni egy Hibát kell dobni 2-es hibakóddal.
- Végül ellenőrizni kell, hogy a platform firmware verziója nagyobb-e vagy egyenlő, mint a játék által elvárt verzió. 
Ha kisebb, akkor a játék sajnos megint nem telepíthető. Legyen kivétel dobva 3-as hibakóddal.
*/
void test_operator_plusz(){
  #if defined TEST_operator_plusz && !defined TEST_BIRO
  Jatekplatform jp(500, 2);
  const Jatek j1("Uncharted", 120, 1), j2("Uncharted", 110, 1), j3("Half life 3", 200, 4);
  jp+=j1;
  jp+=j2;
  assert(jp.get_jatek_szam() == 2);
  assert(jp.get_jatekok()[1].get_tarhely_igeny() == 110);
  try{
    jp+=j3;
    assert(false);
  }catch(const Hiba& h){
    assert(h.get_hibakod() == 3);
  }
  #endif
}

/*
Valósítsd meg a Jatekplatform osztály másoló konstruktorát odafigyelve a dinamikus adattag megfelelő kezelésére.
*/
void test_copy_ctor(){
  #if defined TEST_copy_ctor && !defined TEST_BIRO
  Jatekplatform jp(500, 2);
  const Jatek j1("Uncharted", 120, 1), j2("Uncharted", 110, 1);
  jp+=j1;
  jp+=j2;
  {
    Jatekplatform jp2(jp);
    assert(jp2.get_jatek_szam() == 2);
    assert(jp2.get_jatekok()[1].get_tarhely_igeny() == 110);
  }
  assert(jp.get_jatek_szam() == 2);
  assert(jp.get_jatekok()[1].get_tarhely_igeny() == 110);
  #endif
}

/*
Valósítsd meg a Jatekplatform osztály értékadás operátorát (=) odafigyelve a dinamikus adattag megfelelő kezelésére.
*/
void test_ertekadas_operator(){
  #if defined TEST_ertekadas_operator && !defined TEST_BIRO
  Jatekplatform jp2(300,4);
  const Jatek j3("Last of us", 70, 3);
  jp2+=j3;
  assert(jp2.get_jatek_szam() == 1);
  assert(jp2.get_jatekok()[0].get_tarhely_igeny() == 70);
  {
    Jatekplatform jp(500, 2);
    const Jatek j1("Uncharted", 120, 1), j2("Uncharted", 110, 1);
    jp+=j1;
    jp+=j2;
    jp = jp2;
    assert(jp.get_jatek_szam() == 1);
    assert(jp.get_jatekok()[0].get_tarhely_igeny() == 70);
  }
  assert(jp2.get_jatek_szam() == 1);
  assert(jp2.get_jatekok()[0].get_tarhely_igeny() == 70); 
  #endif
}

/*
Készíts egy Playstation osztályt, amely publikusan öröklődik a Jatekplatform osztályból. Új, privát láthatóságú adattagja:

- pro: PS pro vagy sima (bool)

Legyen két unsigned és egy bool paramétert váró konstruktora, 
amely közül az első paraméter a tárhelyet állítja be, a második a firmware verziót, a harmadik pedig azt, hogy pro-e a playstation.
*/
void test_playstation(){
  #if defined TEST_playstation && !defined TEST_BIRO
  const Playstation ps(10000, 8, true);
  assert(ps.get_pro());
  assert(ps.get_firmware() == 8);
  assert(ps.get_jatek_szam() == 0);
  #endif
}


/*
Definiáld felül a +=Jatek operátort a Playstation osztályra! A += viselkedése nem pro playstation esetén megegyezik az ősével. 
Ha azonban a pro adattag értéke igaz, akkor a Playstation intelligens telepítésre képes. 
Ez azt jelenti, hogy ha a telepítés kizárólag firmware verzió különbség miatt nem sikerülne (ez az egyetlen hiba, hely van elég, nincs a játék még telepítve), 
akkor frissítse fel a firmware verziót arra a szintre, amit a játék elvár és utána telepítse a játékot. Működjön az operátor polimorfikusan is!
*/
void test_operator_plusz_ps(){
  #if defined TEST_operator_plusz_ps && !defined TEST_BIRO
  Jatekplatform * jp = new Playstation(10000, 8, true);
  const Jatek j3("Last of us", 70, 10);
  (*jp) += j3;
  assert(jp->get_jatek_szam() == 1);
  assert(jp->get_firmware() == 10);
  #endif
}

int main() {

  test_jatek1();
  test_jatek2();
  test_egyenloseg();
  test_jatekplatform();
  test_hiba();
  test_operator_plusz();
  test_copy_ctor();
  test_ertekadas_operator();
  test_playstation();
  test_operator_plusz_ps();

  return 0;
}
