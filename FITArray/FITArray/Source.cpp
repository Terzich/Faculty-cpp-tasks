#include <iostream>
using namespace std;
/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU
RUNTIME ERROR CE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URAÐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTICNI ONIMA KOJI SU KORIŠTENI U
TESTNOM CODE-U, OSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE,
POMOCNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAÐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU
MORAJU PREDATI SVOJ RAD
6. ZA POTREBE TESTIRANJA, UNUTAR MAIN FUNKCIJE MOZETE DODAVATI NOVE TESTNE PODATKE
****************************************************************************/
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)
const char * crt = "\n----------------------------------------------------\n";
class Datum {
	int * _dan, *_mjesec, *_godina;
public:
	Datum()
	{
		_dan = nullptr; 
		_mjesec = nullptr;
		_godina = nullptr;
	}
	Datum(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	Datum(const Datum &d)
	{
		_dan = new int(*d._dan);
		_mjesec= new int(*d._mjesec);
		_godina = new int(*d._godina);

	}

	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	void operator=(const Datum &datum)
	{
		if (_dan != nullptr)
			delete[]_dan;
		if (_mjesec != nullptr)
			delete[]_mjesec;
		if (_godina != nullptr)
			delete[]_godina;
		_dan = new int(*datum._dan);
		_mjesec= new int(*datum._mjesec);
		_godina= new int(*datum._godina);
	}

	friend ostream & operator<<(ostream & COUT, const Datum &obj) {
		COUT << *obj._dan << "/" << *obj._mjesec << "/" << *obj._godina;
		return COUT;
	}
};
template<class T, int velicina>
class FITArray {
	T _niz[velicina];
	int _trenutno;
	bool _dozvoliDupliciranje;
public:
	//OSTALE FUNKCIJE CLANICE DEFINISATI VAN TEMPLATE KLASE
	FITArray(bool dozvoliDupliciranje = true);
	int GetTrenutno ()const;
	T * GetNiz() { return _niz; }
	bool operator+=(const T &element);
	bool operator-=(const T &element);
	FITArray operator()(int Od, int Do);
	friend ostream& operator<< <>(ostream &cout, const FITArray &el);
	T operator[] (int index)const {
		return _niz[index];
	}
	void RemoveAll() {
		_trenutno = 0;
	}

};
template<class T, int velicina>
FITArray<T, velicina>::FITArray(bool dozvoljidupliciranje)
{
	_dozvoliDupliciranje = dozvoljidupliciranje;
	_trenutno = 0;
}
template<class T, int velicina>
int FITArray<T, velicina>::GetTrenutno()const { return _trenutno; }
template<class T, int velicina>
bool FITArray<T, velicina>:: operator+=(const T &element)
{
	if (_trenutno == velicina)
	{
		cout << "Niz je pun!" << endl;
		return false;
	}
	for (int i = 0; i < _trenutno; i++)
	{
		if (element == _niz[i] && _dozvoliDupliciranje == false)
			return false;
	}
	_niz[_trenutno] = element;
	_trenutno++;
	return true;
}
template<class T, int velicina>
ostream& operator<<(ostream &cout, const FITArray<T, velicina> &el)
{
	for (int i = 0; i < el._trenutno; i++)
	{
		cout << el._niz[i] << endl;
	}
	return cout;
}
template<class T, int velicina>
bool FITArray<T, velicina>:: operator-=(const T &element)
{
	if (_trenutno == 0)
	{
		cout << "Niz je prazan!!!" << endl;
		return false;
	}
	int c = 0;
	for (int i = 0; i < _trenutno; i++)
	{
		if (element != _niz[i])
		{
			_niz[c] = _niz[i];
			c++;
		}
	}
	_trenutno--;
	return true;
}
template<class T, int velicina>
FITArray<T,velicina> FITArray<T, velicina>::operator()(int Od, int Do)
{
	FITArray<T, velicina>newGK;
	for (int i = Od; i <= Do; i++)
	{
		newGK += _niz[i];
	}
	return newGK;
}


class PolozeniPredmet {
	char * _naziv;
	int _ocjena;
	Datum _datumPolaganja;
public:
	PolozeniPredmet()
	{
		_naziv = nullptr;
		_ocjena = 0;
	}
	PolozeniPredmet(const Datum &datumPolaganja, const char*nazivP,int ocjena):_datumPolaganja(datumPolaganja),_ocjena(ocjena)
	{
		int size = strlen(nazivP) + 1;
		_naziv = new char[size];
		strcpy_s(_naziv, size, nazivP);
	}
	PolozeniPredmet(const PolozeniPredmet &p):_datumPolaganja(p._datumPolaganja)
	{
		int size = strlen(p._naziv) + 1;
		_naziv = new char[size];
		strcpy_s(_naziv, size, p._naziv);
		_ocjena = p._ocjena;

	}
	~PolozeniPredmet() { delete[] _naziv; _naziv = nullptr; }
	int GetOcjena() { return _ocjena; }
	friend ostream & operator<<(ostream & COUT, PolozeniPredmet *obj) {
		COUT << obj->_naziv << "(" << obj->_ocjena << ")" << obj->_datumPolaganja << endl;
		return COUT;
	}
	friend bool operator==(const PolozeniPredmet &p1, const PolozeniPredmet &p2);
};
bool operator==(const PolozeniPredmet &p1, const PolozeniPredmet &p2)
{
	return strcmp(p1._naziv, p2._naziv) == 0;
}

class Student {
	static int _indeksCounter; //sluzi za odredjivanje narednog broja indeksa koji ce biti dodijeljen studentu, pocevsi od 160000
	const int _indeks;
	char * _imePrezime;
	FITArray<PolozeniPredmet*, 40> _polozeniPredmeti;
public:
	Student():_indeks(_indeksCounter++){_imePrezime = nullptr;}
	Student(const char *imeP):_indeks(_indeksCounter++)
	{
		int size = strlen(imeP) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imeP);
	}
	~Student() {
		delete _imePrezime; _imePrezime = nullptr;
		for (int i = 0; i < _polozeniPredmeti.GetTrenutno(); i++)
		{
			delete _polozeniPredmeti[i];
		}
		_polozeniPredmeti.RemoveAll();
	}
	void operator=(const Student &student)
	{
		for (int i = 0; i < _polozeniPredmeti.GetTrenutno(); i++)
		{
			delete _polozeniPredmeti[i];
		}
		_polozeniPredmeti.RemoveAll();
		for (int i = 0; i < student._polozeniPredmeti.GetTrenutno(); i++)
		{
			PolozeniPredmet *tempP = new PolozeniPredmet(*student._polozeniPredmeti[i]);
			_polozeniPredmeti += tempP;
		}
	}
	bool DodajPredmet(const PolozeniPredmet &predmet)
	{
		PolozeniPredmet *temp = new PolozeniPredmet(predmet);
		if (_polozeniPredmeti += temp)
			return true;
		return false;
	}
	float GetProsjek() {
		float prosjek= 0;
		int c = 0;
		for (int i = 0; i < _polozeniPredmeti.GetTrenutno(); i++)
		{
			prosjek+=_polozeniPredmeti[i]->GetOcjena();
			c++;
		}
		prosjek /= c;
		return prosjek;
	}

	friend ostream & operator<<(ostream & COUT, const Student & obj) {
		COUT << obj._indeks << " " << obj._imePrezime << endl<<obj._polozeniPredmeti;
		return COUT;
	}
};
int Student::_indeksCounter = 160000;

void main() {
	const int max = 20;
	Datum jucer(20, 11, 2017), danas(21, 11, 2017), sutra(22, 11, 2017);
	cout << jucer<<endl << danas << endl << sutra << endl;
	jucer = danas;
	cout << jucer << endl << danas << endl << sutra << endl;
	//parametar tipa bool odredjuje da li je u nizu dozvoljeno dupliciranje elemenata
	FITArray<int, max> nizIntegera(false);
	for (size_t i = 0; i < max - 1; i++)
		nizIntegera += i;//dodaje novog clana niza
	if (!(nizIntegera += 6))//pokusavamo dodati dupli clan niza
		cout << "Element nije dodan u niz" << endl;
	cout << crt << nizIntegera << crt; //ispisuje sve clanove niza
	nizIntegera -= 9; //uklanja clan niza sa vrijednoscu 9
	nizIntegera -= 17;
	FITArray<int, max> noviNizIntegera(nizIntegera);
	cout << crt << noviNizIntegera << crt;
	/*parametri odredjuju lokacije (indekse u nizu) elemenata OD - DO koje je potrebno vratiti. u
	slucaju da u nizu ne postoji trazeni broj elemenata funkcija treba da vrati sve element od
	lokacije OD pa do posljednje dostupnog elementa */
	cout << "Clanovi niza od lokacije 2 do lokacije 5 su -> " << noviNizIntegera(2, 5) <<crt;
	Student adel("Adel Handzic"), jasmin("Jasmin Azemovic");
	PolozeniPredmet prII(jucer, "PRII", 8), prIII(sutra, "PRIII", 7);
	adel.DodajPredmet(prII);
	adel.DodajPredmet(prIII); 
	cout << adel << endl;
	//vraca prosjecnu ocjenu studenta
	cout << "Prosjecna ocjena -> " << adel.GetProsjek() << crt;
	jasmin = adel;
	cout << jasmin << endl;
	cout << "Prosjecna ocjena -> " << jasmin.GetProsjek() << crt;
	system("pause");
}