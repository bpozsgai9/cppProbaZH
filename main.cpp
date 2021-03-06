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

        //kell a delete az elemek t??rl??se miatt
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
K??sz??ts egy Jatek oszt??lyt szoftveres j??t??kok reprezent??l??s??ra. Adattagok:
- nev: a j??t??k neve (string)
- tarhely_igeny: a j??t??k m??rete (unsigned)
- firmware_igeny: a firmware szoftver verzi?? (unsigned)

Rendelkezzen default konstruktorral is, mely a sztringet ??resen hagyja a k??t unsigned ??rt??ket pedig 0-ra ??ll??tja be.
*/
void test_jatek1(){
  #if defined TEST_jatek1 && !defined TEST_BIRO
  const Jatek j;
  assert(j.get_firmware_igeny() == 0);
  assert(j.get_tarhely_igeny() == 0);
  #endif
}

/*
Rendelkezzen egy param??teres konstruktorral, amely egy sztring ??s k??t unsigned param??tert v??r ??s inicializ??lja vel??k a h??rom adattagot. 
A param??terek sorrendje: n??v, t??rhely, firmware
*/
void test_jatek2(){
  #if defined TEST_jatek2 && !defined TEST_BIRO
  const Jatek j("Uncharted", 100, 2);
  assert(j.get_firmware_igeny() == 2);
  assert(j.get_tarhely_igeny() == 100);
  #endif
}

/*
Val??s??tsd meg a == oper??tort a Jatek oszt??lyra. Akkor adjon vissza igazat, ha mindh??rom adattag megegyezik, egy??bk??nt hamisat. 
*/
void test_egyenloseg(){
  #if defined TEST_egyenloseg && !defined TEST_BIRO
  const Jatek j1("Uncharted", 120, 1), j2("Uncharted", 110, 1);
  bool result = j1 == j2;
  assert(!result);
  #endif
}


/*
K??sz??ts egy Jatekplatform nev?? oszt??lyt! Adattagok:

- max_tarhely: T??rhely??nek m??rete & unsigned &
- felhasznalt_tarhely: Felhaszn??lt t??rhely & unsigned &
- jatekok: Install??lt j??t??kok list??ja & Jatek* &
- jatek_szam: Telep??tett j??t??kok sz??ma & unsigned &
- firmware: Firmware verzi?? & unsigned &


A j??t??kplatform egy dinamikusan v??ltoz?? m??ret?? t??mbben t??rolja az aktu??lisan telep??tett j??t??kok list??j??t (jatekok adattag) 
A t??mb kezdteben ??res (nullptr) ??s amint egy j??t??k telep??t??sre ker??l meg kell n??velni a t??mb m??ret??t eggyel.

Legyen az oszt??lynak egy param??teres konstruktora, mely k??t unsigned ??rt??ket v??r ??s inicializ??lja vele a max_tarhely ??s firmware ??rt??keket. 
A t??mb kezdetben ??res, a jatek_szam ??s felhasznalt_tarhely ??rtelemszer??en nulla.


Legyen az oszt??lynak destruktora amely felszabad??tja a jatekok t??mb??t, ha nem ??res.
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
Val??s??tsd meg a prefix ??s postfix ++ oper??tort a Jatekplatform oszt??ly sz??m??ra. Az oper??torok firmware upgrade-t v??geznek, vagyis eggyel n??velik a firmware adattag ??rt??k??t.
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
K??sz??ts egy Hiba nev?? oszt??ly, mely publikusan ??r??kl??dik az std::exception oszt??lyb??l. Ez az oszt??ly lesz dobva b??rmilyen hiba eset??n. Egy darab adattagja van:

- hibakod: a hibe??zenet (int)

K??sz??ts neki egy konstruktort, amely egy int-et v??r a hibakod be??ll??t??s??ra.
*/
void test_hiba(){
  #if defined TEST_hiba && !defined TEST_BIRO
  const Hiba h(5);
  assert(h.get_hibakod() == 5);
  #endif
}


/*
Val??s??tsd meg a Jatekplatform oszt??lyra a += oper??tort ??gy, hogy param??tere egy Jatek-ra mutat?? konstans referencia legyen. 
Az oper??tor feladata, hogy megvizsg??lja, telep??thet??-e a j??t??k ??s ha igen akkor megn??veli a jatekok t??mb m??ret??t eggyel ??s elt??rolja az ??j j??t??kot. 
A telep??thet??s??ghez ezeket kell vizsg??lni (ebben a sorrendben):
- El??sz??r ellen??rizze szerepel-e m??r az a j??t??k a telep??tett j??t??kok list??j??ban. Ha igen, legyen egy Hiba kiv??tel dobva 1-es hibak??ddal.
- Ha m??g nincs telep??tve akkor a t??rhelyet kell ellen??rizni. Ha nincs el??g hely a j??t??kot telep??teni egy Hib??t kell dobni 2-es hibak??ddal.
- V??g??l ellen??rizni kell, hogy a platform firmware verzi??ja nagyobb-e vagy egyenl??, mint a j??t??k ??ltal elv??rt verzi??. 
Ha kisebb, akkor a j??t??k sajnos megint nem telep??thet??. Legyen kiv??tel dobva 3-as hibak??ddal.
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
Val??s??tsd meg a Jatekplatform oszt??ly m??sol?? konstruktor??t odafigyelve a dinamikus adattag megfelel?? kezel??s??re.
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
Val??s??tsd meg a Jatekplatform oszt??ly ??rt??kad??s oper??tor??t (=) odafigyelve a dinamikus adattag megfelel?? kezel??s??re.
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
K??sz??ts egy Playstation oszt??lyt, amely publikusan ??r??kl??dik a Jatekplatform oszt??lyb??l. ??j, priv??t l??that??s??g?? adattagja:

- pro: PS pro vagy sima (bool)

Legyen k??t unsigned ??s egy bool param??tert v??r?? konstruktora, 
amely k??z??l az els?? param??ter a t??rhelyet ??ll??tja be, a m??sodik a firmware verzi??t, a harmadik pedig azt, hogy pro-e a playstation.
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
Defini??ld fel??l a +=Jatek oper??tort a Playstation oszt??lyra! A += viselked??se nem pro playstation eset??n megegyezik az ??s??vel. 
Ha azonban a pro adattag ??rt??ke igaz, akkor a Playstation intelligens telep??t??sre k??pes. 
Ez azt jelenti, hogy ha a telep??t??s kiz??r??lag firmware verzi?? k??l??nbs??g miatt nem siker??lne (ez az egyetlen hiba, hely van el??g, nincs a j??t??k m??g telep??tve), 
akkor friss??tse fel a firmware verzi??t arra a szintre, amit a j??t??k elv??r ??s ut??na telep??tse a j??t??kot. M??k??dj??n az oper??tor polimorfikusan is!
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
