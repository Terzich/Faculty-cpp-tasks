#include <iostream>
#include<memory>
using namespace std;
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

enum VrstaObaveze { Seminarski, Parcijalni1, Parcijalni2, Integralni, Prakticni };
char *niz[5] = { "Seminarski","Parcijalni1","Parcijalni2","Integralni","Prakticni" };

struct Datum
{
	int * _dan  , *_mjesec, *_godina;
	void Unos(int d, int m, int g)
	{
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	int GetDays() { return *_dan + *_mjesec * 30 + *_godina * 365; }
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() { delete _dan; delete _mjesec; delete _godina; }
};
struct ObavezeNaPredmetu
{
	shared_ptr<VrstaObaveze> _vrstaObaveze;
	Datum _datumIzvrsenja;
	char * _napomena;
	int _ocjena; // 5 - 10 
	void Unos(VrstaObaveze vrsta, Datum datum, int ocjena, char * napomena)
	{
		_vrstaObaveze = make_shared<VrstaObaveze>(vrsta);
		_datumIzvrsenja = datum;
		_ocjena = ocjena;
		_napomena = new char[strlen(napomena) + 1];
		strcpy_s(_napomena, strlen(napomena) + 1, napomena);
	}
	void Ispis()
	{
		cout << niz[*_vrstaObaveze]<< " " << _ocjena << " " << _napomena;
		_datumIzvrsenja.Ispis();
		cout << endl;
	}
	void Dealociraj()
	{
		
		//_datumIzvrsenja.Dealociraj();
		delete[] _napomena; _napomena = nullptr;
	}
};

char * _kljucneRijeci[15] = { NULL }; /*da bi se odredjena obaveza na predmetu smatrala uspjesno izvrsenom,
									  u napomeni (objekta ObavezeNaPredmetu) ne smije stajati niti jedna od
									  rijeci koja se nalazi u nizu _kljucneRijeci*/

struct PolozeniPredmet
{
	Datum _datumPolaganja;/*datum koji ce se evidentirati kao datum kada je predmet polozen tj. kada je formirana konacna ocjena*/
	char * _nazivPredmeta;
	ObavezeNaPredmetu * _listaIzvrsenihObaveza;
	int _trenutnoIzvrsenihObaveza;
	int _minimalanBrojDana; /*odnosi se na minimalan broj dana koji mora proci od izvrsenja pojedinih obaveza na predmetu (npr. 7 dana)*/
	int _konacnaOcjena; //formira se na osnovu ocjena izvrsenih obaveza

	void Unos(char * naziv, int minimalanBrojDana)
	{
		_nazivPredmeta = new char[strlen(naziv) + 1];
		strcpy_s(_nazivPredmeta, strlen(naziv) + 1, naziv);
		_listaIzvrsenihObaveza = nullptr;
		_trenutnoIzvrsenihObaveza = 0;
		_minimalanBrojDana = minimalanBrojDana;
		_konacnaOcjena = 0;
	}
	void Dealociraj()
	{

		//_datumPolaganja.Dealociraj();
		delete[] _nazivPredmeta; _nazivPredmeta = nullptr;
		for (size_t i = 0; i < _trenutnoIzvrsenihObaveza; i++)
			_listaIzvrsenihObaveza[i].Dealociraj();
		delete[]_listaIzvrsenihObaveza;
	}
	/*Na osnovu vrijednosti primljenih parametara osigurati dodavanje novoizvrsene obaveze na predmetu.
	Potrebno je onemoguciti dodavanje identicnih obaveza, a izmedju izvrsenja pojedinih obaveza mora proci
	najmanje onoliko dana koliko je definisano vrijednoscu atributa _minimalanBrojDana.
	Identicna vrsta obaveze se moze dodati samo u slucaju da je prethodno dodana obaveza (identicne vrste)
	imala ocjenu 5 ili je u napomeni sadrzavala neku od zabranjenih rijeci (definisanih nizom _kljucneRijeci).*/

	bool DodajIzvrsenuObavezu(VrstaObaveze ob, Datum d, int ocjena, char* nap)
	{
		for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++)
		{
			bool test = strcmp(_listaIzvrsenihObaveza[i]._napomena, nap);

			if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == ob && *_listaIzvrsenihObaveza[i]._datumIzvrsenja._dan == *d._dan &&
				*_listaIzvrsenihObaveza[i]._datumIzvrsenja._mjesec == *d._mjesec && *_listaIzvrsenihObaveza[i]._datumIzvrsenja._godina == *d._godina
				&& _listaIzvrsenihObaveza[i]._ocjena == ocjena&& test == false)
				return false;

			if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == ob)
			{
				if (_listaIzvrsenihObaveza[i]._ocjena > 5)
					return false;
				if (strstr(_listaIzvrsenihObaveza[i]._napomena, _kljucneRijeci[0]) == nullptr && strstr(_listaIzvrsenihObaveza[i]._napomena, _kljucneRijeci[1]) == nullptr
					&& strstr(_listaIzvrsenihObaveza[i]._napomena, _kljucneRijeci[2]) == nullptr)
					return false;
				int razlika = d.GetDays() - _listaIzvrsenihObaveza[i]._datumIzvrsenja.GetDays();
				if (razlika < 8)
					return false;

			}

			if (i = _trenutnoIzvrsenihObaveza - 1)
			{
				int razlika = d.GetDays() - _listaIzvrsenihObaveza[i]._datumIzvrsenja.GetDays();
				if (razlika < 8)
					return false;

			}
		}
		ObavezeNaPredmetu *temp = new ObavezeNaPredmetu[_trenutnoIzvrsenihObaveza + 1];
		for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++)
		{
			temp[i].Unos(*_listaIzvrsenihObaveza[i]._vrstaObaveze, _listaIzvrsenihObaveza[i]._datumIzvrsenja, _listaIzvrsenihObaveza[i]._ocjena, _listaIzvrsenihObaveza[i]._napomena);
			_listaIzvrsenihObaveza[i].Dealociraj();
		}
		temp[_trenutnoIzvrsenihObaveza].Unos(ob, d, ocjena, nap);
		delete[] _listaIzvrsenihObaveza;
		_listaIzvrsenihObaveza = temp;
		_trenutnoIzvrsenihObaveza++;
		return true;
	}
	/*Konacna ocjene predstavlja prosjecnu ocjenu na predmetu (prosjecnu ocjenu izvrsenih
	obaveza koje ispunjavaju uslove za uspjesno polaganje predmeta),a za njeno formiranje
	student mora posjedovati polozen integralni ili dva parijcalna ispita. Ukoliko je ispit
	polozen putem parcijalnih ispita, student takodjer mora imati pozitivno (ocjenom vecom od 5)
	ocijenjena najmanje dva seminarska i jedan prakticni rad. Takodjer, datum polaganja drugog parcijalnog
	ispita ne smije biti raniji (manji) od datuma polaganja prvog parcijalnog ispita. Konacna ocjena ne
	smije biti formirana u slucaju da napomena bilo koje odbaveze (koja je ucestvovala u formiranju konacne ocjene)
	sadrzi neku od zabranjenih rijeci (niz _kljucneRijeci). U slucaju da neki od navedenih uslova nije zadovoljen
	konacna ocjena treba biti postavljena na vrijednost 5. Ukoliko su ispunjeni svi uslovi, osim formiranja konacne
	ocjene, datum polaganja je potrebno postaviti na vrijednost datuma posljednje izvrsene obaveze sa najvecom ocjenom.
	*/
	float FormirajKonacnuOcjenu() {
		_konacnaOcjena = 5;
		bool integralni = false, parcijalni = false;
		for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++)
		{
			if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Integralni && _listaIzvrsenihObaveza[i]._ocjena > 5)
				integralni = true;
			if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Parcijalni1 && _listaIzvrsenihObaveza[i]._ocjena > 5)
			{

				for (int j = i + 1; j < _trenutnoIzvrsenihObaveza; j++)
				{
					int razlika = _listaIzvrsenihObaveza[j]._datumIzvrsenja.GetDays() - _listaIzvrsenihObaveza[i]._datumIzvrsenja.GetDays();
					if (*_listaIzvrsenihObaveza[j]._vrstaObaveze == Parcijalni2 && _listaIzvrsenihObaveza[j]._ocjena > 5 && razlika > 0)
						parcijalni = true;
				}
			}
		}
		bool test = false;
		if (parcijalni)
		{
			int c = 0, cc = 0;
			for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++)
			{
				if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Seminarski && _listaIzvrsenihObaveza[i]._ocjena > 5)
					c++;
				if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Prakticni &&	_listaIzvrsenihObaveza[i]._ocjena > 5)
					cc++;
			}
			if (c >= 2 && cc >= 1)
				test = true;
		}
		bool dane = true;
		for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++)
		{
			if (_listaIzvrsenihObaveza[i]._ocjena > 5)
				if (strstr(_listaIzvrsenihObaveza[i]._napomena, _kljucneRijeci[0]) != nullptr || strstr(_listaIzvrsenihObaveza[i]._napomena, _kljucneRijeci[1]) != nullptr
					|| strstr(_listaIzvrsenihObaveza[i]._napomena, _kljucneRijeci[2]) != nullptr)
				{
					dane = false;
				}

		}
		int brojocjena = 0, prosjek = 0, poz = 0;
		if (integralni&&dane || test&&dane)
		{
			for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++)
			{
				int temp = 0;
				if (_listaIzvrsenihObaveza[i]._ocjena > 5)
				{
					temp = _listaIzvrsenihObaveza[i]._ocjena;
					brojocjena++;
					prosjek += _listaIzvrsenihObaveza[i]._ocjena;
					if (i = _trenutnoIzvrsenihObaveza - 1)
						_konacnaOcjena = prosjek / brojocjena;
				}
				if (_listaIzvrsenihObaveza[i]._ocjena > temp)
				{
					temp = _listaIzvrsenihObaveza[i]._ocjena;
					poz = i;
				}
			}
			_datumPolaganja.Unos(*_listaIzvrsenihObaveza[poz]._datumIzvrsenja._dan, *_listaIzvrsenihObaveza[poz]._datumIzvrsenja._mjesec, *_listaIzvrsenihObaveza[poz]._datumIzvrsenja._godina);
		}
		return _konacnaOcjena;
	}
};
	float PretragaRekurzivno(PolozeniPredmet pp, VrstaObaveze v, Datum od, Datum doo, int p, int bu,int c=0)	{
		if (c == pp._trenutnoIzvrsenihObaveza)
		{
			if (bu == 0)
				return 0;
			return float(p) / bu;
		}
		if (*pp._listaIzvrsenihObaveza[c]._vrstaObaveze == v && pp._listaIzvrsenihObaveza[c]._datumIzvrsenja.GetDays() >= od.GetDays() &&
			pp._listaIzvrsenihObaveza[c]._datumIzvrsenja.GetDays() <= doo.GetDays() && pp._listaIzvrsenihObaveza[c]._ocjena > 5)
			return PretragaRekurzivno(pp, v, od, doo, p + pp._listaIzvrsenihObaveza[c]._ocjena, bu + 1, c + 1);
		return PretragaRekurzivno(pp, v, od, doo, p , bu , c + 1);

	}
	int GetBrojObavezaIznadProsjeka(PolozeniPredmet p, float prosjek)
	{
		auto brojobaveza = [p, prosjek](){
			int c = 0;
			for (int i = 0; i < p._trenutnoIzvrsenihObaveza; i++)
			{
				if (p._listaIzvrsenihObaveza[i]._ocjena > prosjek)
					c++;

			}

			return c;
		};
		return brojobaveza();
	}

void main()
{
	_kljucneRijeci[0] = "prepisivao";
	_kljucneRijeci[1] = "ometao";
	_kljucneRijeci[2] = "nije dostavio";

	Datum datumSeminarski1, datumSeminarski2, datumParcijalni1, datumParcijalni2, datumIntegralni;
	datumSeminarski1.Unos(10, 6, 2017);
	datumSeminarski2.Unos(18, 6, 2017);
	datumParcijalni1.Unos(22, 6, 2017);
	datumParcijalni2.Unos(30, 6, 2017);

	PolozeniPredmet prII;
	prII.Unos("Programiranje II", 7);


	//datumi zamjenjeni da bi zadatak radio (nadopuniti kasnije)
	if (prII.DodajIzvrsenuObavezu(Seminarski, datumSeminarski1, 8, "implementirani svi algoritmi"))
		cout << "Seminarski1...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(Seminarski, datumSeminarski2, 6, "rad slican kao kod studenta IB150388"))
		cout << "Seminarski2...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(Parcijalni1, datumParcijalni1, 6, "uslovno polozen ispit"))
		cout << "Parcijalni 1...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(Parcijalni2, datumParcijalni2, 5, "previse gresaka, a tokom ispita ometao druge studente"))
		cout << "Parcijalni 2...dodan" << endl;


	cout << "Konacna ocjena iz predmeta PRII je: " << prII.FormirajKonacnuOcjenu() << endl;

	datumParcijalni2.Unos(22, 7, 2017);
	if (prII.DodajIzvrsenuObavezu(Integralni, datumParcijalni2, 6, ""))
		cout << "Integralni...dodan" << endl;
	cout << "Konacna ocjena iz predmeta PRII je: " << prII.FormirajKonacnuOcjenu() << endl;
	Datum OD, DO; OD.Unos(1, 1, 2017); DO.Unos(1, 7, 2017);

	/*Rekurzivna funkcija treba vratiti prosjecnu ocjenu uspjesno izvrsenih obaveze na
	predmetu odredjene vrste u definisanom periodu. Izbjegavati koristenje globalnih varijabli*/
	float prosjek = PretragaRekurzivno(prII, Seminarski, OD, DO, 0, 0);
	cout << "Prosjecna ocjena izvrsenih obaveza je: " << prosjek << endl;

	/*Koristeci lambda-u, unutar funkcije GetBrojObavezaIznadProsjeka pronaci broj izvrsenih obaveza
	koje imaju istu ili vecu ocjenu od prosjecne*/
	int iznadProsjeka = GetBrojObavezaIznadProsjeka(prII, prosjek);
	cout << "Broj izvrsenih obaveza iznad prosjeka je: " << iznadProsjeka << endl;

	/*datumParcijalni1.Dealociraj();
	datumParcijalni2.Dealociraj();
	datumSeminarski1.Dealociraj();
	datumSeminarski2.Dealociraj();
	datumParcijalni2.Dealociraj();
	prII.Dealociraj();
*/

	system("pause>0");
}