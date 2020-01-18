#include <iostream>

using namespace std;

enum enumKursevi { HtmlCSSJavaScript, SoftwareEngeneeringFundamentals, MasteringSQL, WindowsSecurity };
const char *nizkurseva[4] = { "HtmlCSSJavaScript","SoftwareEngeneeringFundamentals","MasteringSQL","WindowsSecurity" };
const char *crt = "\n--------------------------------------------\n";

struct Datum
{
	int * _dan, *_mjesec, *_godina;
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

struct Kurs
{
	enumKursevi _kurs;
	Datum _pocetak;
	Datum _kraj;
	char * _imePredavaca;
	bool _aktivan;//SVAKI KURS JE NA POCETKU AKTIVAN

	void Unos(enumKursevi kurs, Datum pocetak, Datum kraj, const char *imepredavaca)
	{
		_kurs = kurs;
		_pocetak.Unos(*pocetak._dan, *pocetak._mjesec, *pocetak._godina);
		_kraj.Unos(*kraj._dan, *kraj._mjesec, *kraj._godina);
		_imePredavaca = new char[strlen(imepredavaca) + 1];
		strcpy_s(_imePredavaca, strlen(imepredavaca) + 1, imepredavaca);
		_aktivan = true;
	}
	void Ispis() {

		cout << nizkurseva[_kurs] << endl << "Pocetak ovog kursa je bio: ";
		_pocetak.Ispis();
		cout<<endl<< "Kraj ovog kursa je : ";
		_kraj.Ispis();
		cout << endl << "Predavac na ovome kursu je: " << _imePredavaca;
		if (_aktivan)
			cout << endl << "Ovaj kurs je aktivan";
		else
			cout << "Ovaj kurs nije aktivan";
	}

	void Dealociraj() { delete[] _imePredavaca; _imePredavaca = nullptr; 
	_pocetak.Dealociraj();
	_kraj.Dealociraj();
	}
};
struct Polaznik
{
	int _polaznikID;
	char * _imePrezime;
	void Unos(int polaznikID, const char * imePrezime)
	{
		_polaznikID = polaznikID;
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
	}
	void Ispis() { cout << _polaznikID << " " << _imePrezime << endl; }
	void Dealociraj() { delete[] _imePrezime; _imePrezime = nullptr; }
};
struct Polaganja
{
	Polaznik _polaznik;
	Kurs _kurs;
	Datum _datumPolaganja;
	float _ostvareniUspjeh;
	void Unos(Polaznik p, Kurs kurs, Datum datumpolaganja, float brojbodova)
	{
		_polaznik.Unos(p._polaznikID, p._imePrezime);
		_kurs.Unos(kurs._kurs, kurs._pocetak, kurs._kraj, kurs._imePredavaca);
		_datumPolaganja.Unos(*datumpolaganja._dan, *datumpolaganja._mjesec, *datumpolaganja._godina);
		_ostvareniUspjeh = brojbodova;
	}
	void Ispis() {

		_polaznik.Ispis();
		_kurs.Ispis();
		_datumPolaganja.Ispis();
		cout << endl << "Ostvareni uspjeh na ovome ispitu je: " << _ostvareniUspjeh << endl;

	}
	void Dealociraj() {
		_polaznik.Dealociraj();
		_kurs.Dealociraj();
		_datumPolaganja.Dealociraj();
	}


};
struct SkillsCentar
{
	char * _nazivCentra;
	Kurs * _kursevi[50];//JEDAN EDUKACIJSKI CENTAR MOZE NUDITI NAJVISE 50 KURSEVA
	int _trenutnoKurseva;
	Polaganja * _polaganja;
	int _trenutnoPolaganja;
	void Unos(const char *imecentra)
	{
		_nazivCentra = new char[strlen(imecentra) + 1];
		strcpy_s(_nazivCentra, strlen(imecentra) + 1, imecentra);
		_polaganja = nullptr;
		_trenutnoKurseva = 0;
		_trenutnoPolaganja = 0;
	}
	void Dealociraj() {
		delete[]_nazivCentra;
		_nazivCentra = nullptr;
		for (int i = 0; i < _trenutnoKurseva; i++)
		{
			_kursevi[i]->Dealociraj();
			_kursevi[i] = nullptr;
		}
		for (int i = 0; i < _trenutnoPolaganja; i++)
		{
			_polaganja[i].Dealociraj();
		}
		delete[]_polaganja; _polaganja = nullptr;

	}

	/*jedan edukacijski centar moze nuditi vise istih kurseva (npr. MasteringSQL), ali se oni moraju
	realizovati u razlictim periodima (pocetak-kraj) tj. ne smiju se preklapati
	novi kurs istog tipa (npr. MasteringSQL) se mora realizovati nakon prethodno dodanog npr. ako prethodni kurs MasteringSQL
	traje 01.03.2016 - 01.04.2016, onda se novi kurs MasteringSQL moze dodati jedino ako pocinje nakon 01.04.2016*/

	bool DodajKurs(Kurs k)
	{
		for (int i = 0; i < _trenutnoKurseva; i++)
		{
			if (_kursevi[i]->_kurs == k._kurs)
				if (k._pocetak.GetDays() < _kursevi[i]->_kraj.GetDays())
					return false;
		}
		if (_trenutnoKurseva > 49)
			return false;
		_kursevi[_trenutnoKurseva] = new Kurs;
		_kursevi[_trenutnoKurseva]->Unos(k._kurs, k._pocetak, k._kraj, k._imePredavaca);
		_trenutnoKurseva++;
		return true;
	}
	/*nije moguce dodati polaganje onog kursa koji nije evidentiran (registrovan, ponudjen) u tom edukacijskom centru
	  moguce je polagati samo aktivne kurseve i to najkasnije 5 dana od dana zavrsetka (kraja) posljednje dodanog
	  (aktivnog) kursa iste vrste (npr. MasteringSQL)
	  jedan polaznik moze vise puta polagati isti kurs, ali novi procenat mora biti veci od prethodno dodanog (na istom kursu)*/
	
	bool Provjera(Polaganja p)
	{
		bool test = false;
		for (int i = 0; i < _trenutnoKurseva; i++)
		{
			if (p._kurs._kurs == _kursevi[i]->_kurs)
				test = true;

		}
		if (!test)
			return false;
		for (int i = 0; i < _trenutnoKurseva; i++)
		{
			if (p._kurs._kurs == _kursevi[i]->_kurs)
			{
				if (!_kursevi[i]->_aktivan)
					return false;
				if ((p._datumPolaganja.GetDays() - _kursevi[i]->_kraj.GetDays()) > 5)
					return false;
			}
		}
		return true;
	}

	bool DodajPolaganje(Polaganja polaganje)
	{
		if (!Provjera(polaganje))
			return false;
		Polaganja *temp = new Polaganja[_trenutnoPolaganja + 1];
		for (int i = 0; i < _trenutnoPolaganja; i++)
		{
			temp[i].Unos(_polaganja[i]._polaznik, _polaganja[i]._kurs, _polaganja[i]._datumPolaganja, _polaganja[i]._ostvareniUspjeh);
			_polaganja[i].Dealociraj();
		}
		temp[_trenutnoPolaganja].Unos(polaganje._polaznik, polaganje._kurs, polaganje._datumPolaganja, polaganje._ostvareniUspjeh);
		delete[]_polaganja;
		_polaganja = temp;
		_trenutnoPolaganja++;
		return true;
	}
	Polaganja *PolaganjaByDatum(Datum od, Datum _do, int &brojpolozenih)
	{
		Polaganja *temp = nullptr;
		brojpolozenih = 0;
		for (int i = 0; i < _trenutnoPolaganja; i++)
		{
			if (_polaganja[i]._datumPolaganja.GetDays() >= od.GetDays() && _polaganja[i]._datumPolaganja.GetDays() <= _do.GetDays())
				brojpolozenih++;
		}
		if (brojpolozenih == 0)
			return temp;
		temp = new Polaganja[brojpolozenih];
		for (int i = 0; i < _trenutnoPolaganja; i++)
		{
			if (_polaganja[i]._datumPolaganja.GetDays() >= od.GetDays() && _polaganja[i]._datumPolaganja.GetDays() <= _do.GetDays())
				temp[i].Unos(_polaganja[i]._polaznik, _polaganja[i]._kurs, _polaganja[i]._datumPolaganja, _polaganja[i]._ostvareniUspjeh);
		}
		return temp;
	}


};
float PretragaRekurzivno(enumKursevi k, SkillsCentar m, int godina, int brojac = 0, int prosjek = 0,int uspjenopolaganje=0)
{
	if (m._trenutnoPolaganja == brojac)
	{
		if (prosjek == 0)
			return 0;
		return float(prosjek) / uspjenopolaganje;
	}
	if (m._polaganja[brojac]._kurs._kurs == k && *m._polaganja[brojac]._datumPolaganja._godina == godina)
		return PretragaRekurzivno(k, m, godina, brojac + 1, prosjek += m._polaganja[brojac]._ostvareniUspjeh, uspjenopolaganje + 1);
	return PretragaRekurzivno(k, m, godina, brojac + 1, prosjek, uspjenopolaganje);
}


void main()
{
	Datum datum1, datum2, datum3, datum4, datum5, datum6;
	datum1.Unos(26, 11, 2015);
	datum2.Unos(29, 5, 2016);
	datum3.Unos(5, 6, 2016);
	datum4.Unos(15, 8, 2016);
	datum5.Unos(13, 7, 2016);
	datum6.Unos(22, 9, 2016);

	Kurs softverski, html, sql, security;
	softverski.Unos(SoftwareEngeneeringFundamentals, datum1, datum2, "Emina Junuz");
	html.Unos(HtmlCSSJavaScript, datum2, datum3, "Larisa Tipura");
	sql.Unos(MasteringSQL, datum3, datum4, "Jasmin Azemovic");
	security.Unos(WindowsSecurity, datum3, datum4, "Adel Handzic");

	Polaznik denis, zanin, indira;
	denis.Unos(1, "Denis Music");
	zanin.Unos(2, "Zanin Vejzovic");
	indira.Unos(3, "Indira Hamulic");

	Polaganja denisHtml, zaninHtml, indiraSql;
	denisHtml.Unos(denis, html, datum5, 61);
	//61 predstavlja ostvareni uspjeh/procenat, a uspjesno polozenim se smatra 
	//svaki kurs na kome je polaznik ostvari vise od 55%
	zaninHtml.Unos(zanin, html, datum6, 93);

	SkillsCentar mostar;
	mostar.Unos("Skills Center Mostar");
	/*jedan edukacijski centar moze nuditi vise istih kurseva (npr. MasteringSQL), ali se oni moraju
	realizovati u razlictim periodima (pocetak-kraj) tj. ne smiju se preklapati
	novi kurs istog tipa (npr. MasteringSQL) se mora realizovati nakon prethodno dodanog npr. ako prethodni kurs MasteringSQL
	traje 01.03.2016 - 01.04.2016, onda se novi kurs MasteringSQL moze dodati jedino ako pocinje nakon 01.04.2016*/
	if (mostar.DodajKurs(softverski)) {
		cout << crt << "KURS USPJESNO REGISTROVAN" << crt;
			softverski.Ispis();
			cout << crt;
		}
		/*nije moguce dodati polaganje onog kursa koji nije evidentiran (registrovan, ponudjen) u tom edukacijskom centru
		  moguce je polagati samo aktivne kurseve i to najkasnije 5 dana od dana zavrsetka (kraja) posljednje dodanog
		  (aktivnog) kursa iste vrste (npr. MasteringSQL)
		  jedan polaznik moze vise puta polagati isti kurs, ali novi procenat mora biti veci od prethodno dodanog (na istom kursu)*/
		if (mostar.DodajPolaganje(denisHtml)) {
			cout << crt << "POLAGANJE EVIDENTIRANO" << crt;
			//denisHtml.Ispis();
		}
		Datum OD, DO;
		int brojPolaganja = 0;
		OD.Unos(1, 6, 2016);
		DO.Unos(1, 8, 2016);
		//PolaganjaByDatum – vraca niz polaganja koja su uspjesno realizovana u periodu OD-DO
		Polaganja * polaganjaByDatum = mostar.PolaganjaByDatum(OD, DO, brojPolaganja);
		cout << "U periodu od ";
		OD.Ispis();
		cout << " do ";
		DO.Ispis();
		cout << " uspjesno je realizovano " << brojPolaganja << " polaganja--->>>";
		for (size_t i = 0; i < brojPolaganja; i++)
			polaganjaByDatum[i].Ispis();

		/*PretragaRekurzivno - rekurzivna funkcija pronalazi prosjecni uspjeh koji su polaznici tokom
		  godine (npr.2016) ostvarili na odredjenom kursu (npr. MasteringSQL)*/
		cout << "Prosjecan uspjeh na kursu MasteringSQL u 2016 godini je " << PretragaRekurzivno(MasteringSQL, mostar,2016) << endl;
		////Ispis - ispisuje sve informacije o edukacijskom centru. prije ispisa sortirati sve kurseve na osnovu pocetka odrzavanja kursa
		//mostar.Ispis();
		datum1.Dealociraj();
		datum2.Dealociraj();
		datum3.Dealociraj();
		datum4.Dealociraj();
		datum5.Dealociraj();
		datum6.Dealociraj();
		OD.Dealociraj();
		DO.Dealociraj();
		softverski.Dealociraj();
		html.Dealociraj();
		sql.Dealociraj();
		security.Dealociraj();
		denis.Dealociraj();
		indira.Dealociraj();
		zanin.Dealociraj();
		denisHtml.Dealociraj();
		zaninHtml.Dealociraj();
		for (int i = 0; i < brojPolaganja; i++)
		{
			polaganjaByDatum[i].Dealociraj();
		}
		delete[]polaganjaByDatum;
		mostar.Dealociraj();
		system("pause>0");
		
	}