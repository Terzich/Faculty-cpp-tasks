#include<iostream>
using namespace std;

template<class T>
class Kolekcija {
	T* _elementi;
	int _trenutno;
public:
	//Implementirati potrebne funkcije za rad sa generi�kom klasom
	Kolekcija():_elementi(nullptr), _trenutno(0)
	{
	}
	int GetTrenutno()const  { return _trenutno; }
	T& operator[](int index)const
	{
		return _elementi[index];
	}
	bool Add(const T &element)
	{
		for (int i = 0; i < _trenutno; i++)
		{
			if (_elementi[i] == element)
				return false;
		}
		T *temp = new T[_trenutno + 1];
		for (int i = 0; i < _trenutno; i++)
		{
			temp[i] = _elementi[i];
		}
		temp[_trenutno] = element;
		delete[]_elementi;
		_elementi = temp;
		_trenutno++;
		return true;
	}
	bool RemoveByIndex(int index) {
		if( _trenutno<1 || index>_trenutno-1)
			return false;
		T *temp = new T[_trenutno - 1];
		int c = 0;
		for (int i = 0; i < _trenutno; i++)
		{
			if (i != index)
			{
				temp[c] = _elementi[i];
				c++;
			}
		}
		delete[]_elementi;
		_elementi = temp;
		_trenutno--;
		return true;
	}
	bool RemoveByObject(const T &obj)
	{
		for (int i = 0; i < _trenutno; i++)
		{
			if (_elementi[i] == obj)
			{
				RemoveByIndex(i);
				return true;
			}
		}
		return false;
	}
	int GetTrenutno() { return _trenutno; }
	void Info() {
		for (int i = 0; i < _trenutno; i++)
		{
			cout << _elementi[i];
		}

	}
};
int brojClanski = 1;
class Clan
{
	const int _clanskiBroj; //automatski generisati i sprije�iti dupliranje
	char*_imePrezime;
	char _telefon[20];
	int _brojIznajmljenihKnjiga;
public:

	Clan(const char* imePrezime="///", const char* telefon="x"):_clanskiBroj(brojClanski++)
	{
		int vel = strlen(imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, imePrezime);
		strncpy_s(_telefon, telefon, _TRUNCATE);
		_brojIznajmljenihKnjiga = 0;
	}
	Clan(const Clan&c):_clanskiBroj(brojClanski++)
	{
		int vel = strlen(c._imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, c._imePrezime);
		strncpy_s(_telefon, c._telefon, _TRUNCATE);
	}
	~Clan()
	{
		if(_imePrezime!=nullptr)
			delete[] _imePrezime;
		_imePrezime = nullptr;
	}
	void operator=(const Clan &c)
	{
		if (_imePrezime != nullptr)
			delete[]_imePrezime;
		int size = strlen(c._imePrezime) + 1;
		strcpy_s(_imePrezime, size, c._imePrezime);
		strcpy_s(_telefon, 20, c._telefon);
		_brojIznajmljenihKnjiga = c._brojIznajmljenihKnjiga;
	}
	void UvecajIznajmljivanje(){ _brojIznajmljenihKnjiga++; }
	friend ostream& operator<<(ostream&, const Clan&);
	friend bool operator==(const Clan&, const Clan&);
};
 bool operator==(const Clan &c1, const Clan &c2)
{
	 return c1._clanskiBroj == c2._clanskiBroj;
}

ostream& operator<<(ostream& cout, const Clan& c)
{
	cout << "\nPodaci o clanu\n";
	cout << "Clanski broj: " << c._clanskiBroj << endl;
	cout << "Ime i prezime: " << c._imePrezime << endl;
	cout << "Telefon: " << c._telefon << endl;

	return cout;
}

class Knjiga
{
	char _ISBN[20];
	char* _naslov;
	int _godinaIzdanja;
	Clan* _clan; //Podaci o �lanu kod kojeg se knjiga trenutno nalazi. Pohraniti samo adresu postoje�eg �lana.
	int _brojDanaZauzetosti;

	static bool ProvjeriISBN(const char* ISBN)
	{
		//Funkcija vr�i provjeru validnosti ISBN broja knjige
		//Podr�ani su 13-znamenkasti ISBN brojevi zapisani u formatu: 978-953-220-545-9
		//Pravilo: Broj treba da sadr�i 4 znaka '-', bez slovnih oznaka (mala i velika slova)
		//Dodatno naknadno provjeriti da izme�u pojedinih znakova '-' budu zapisana 3 broja, a 
		//iza posljednjeg samo jedan
		if (strlen(ISBN) != 17)
			return false;
		int c = 0;
		for (int i = 0; i < strlen(ISBN); i++)
		{
			if (ISBN[i] == '-')
				c++;
			if (ISBN[i] != '-'&&ISBN[i] <'0' || ISBN[i] > '9')
				return false;
		}
		if (c != 4)
			return false;
		c = 0;
		for (int i = 0; i < strlen(ISBN); i++)
		{
			if (ISBN[i] == '-' && i == 3 || ISBN[i] == '-' && i == 7 || ISBN[i] == '-' && i == 11 || ISBN[i] == '-' && i == 15)
				c++;
		}
		if (c < 4)
			return false;
		return true;
	}

public:
	//Potrebne konstruktor i destruktor funkcije
	//Knjiga inicijalno nije zadu�ena

	Knjiga(const char *isbn = "/", const char *naslov = "/", int godinaizdanja = 0)
		:_godinaIzdanja(godinaizdanja),_clan(nullptr),_brojDanaZauzetosti(0)
	{
		strcpy_s(_ISBN,20,isbn);
		int vel = strlen(naslov) +1 ;
		_naslov = new char[vel];
		strcpy_s(_naslov, vel, naslov);
	}
	Knjiga(const Knjiga &k)
	{
		strcpy_s(_ISBN, 20, k._ISBN);
		int size = strlen(k._naslov);
		strcpy_s(_naslov, size, k._naslov);
		_godinaIzdanja = k._godinaIzdanja;
		_clan = k._clan;
		_brojDanaZauzetosti = k._brojDanaZauzetosti;
	}
	~Knjiga()
	{
		delete[] _naslov; _naslov = nullptr;
		delete _clan; _clan = nullptr;
	}
	void operator=(const Knjiga &k)
	{
		strcpy_s(_ISBN, 20, k._ISBN);
		if (_naslov != nullptr)
			delete[] _naslov;
		int size = strlen(k._naslov)+1;
		_naslov = new char[size];
		strcpy_s(_naslov,size, k._naslov);
		_godinaIzdanja = k._godinaIzdanja;
		if (_clan != nullptr)
			delete _clan;
		_clan = k._clan;
		_brojDanaZauzetosti = k._brojDanaZauzetosti;
	}
	//Funkciju ZaduziKnjigu koja treba da postavi podatke o clanu kod kojeg se knjiga trenutno nalazi
	//Voditi ra�una da se originalnom objektu tipa Clan uve�a broj iznajmljenih knjiga za jedan.
	bool ZaduziKnjigu(Clan *clan, int brojdana)
	{
		if (_clan != nullptr) {
			cout << "Ova knjiga je vec zaduzena" << endl;
			return false;
		}
		_clan = clan;
		_clan->UvecajIznajmljivanje();
		_brojDanaZauzetosti = brojdana;
		return true;
	}
	//Funkciju RazduziKnjigu koja knjigu ozna�ava kao slobodnu, odnosno uklanja podatke o clanu kod kojeg se ona trenutno nalazi.
	bool RazduziKnjigu() {
		if (_clan == nullptr)
		{
			cout << "Ova knjiga nije zaduzena" << endl;
			return false;
		}
		_clan = nullptr;
		_brojDanaZauzetosti = 0;
		return true;
	}
	//Funkciju ProvjeriZaduzenost koja vra�a logi�ku vrijednost "true" ukoliko je knjiga zadu�ena, u suprotnom vra�a "false"
	bool ProvjeriZaduzenost() {
		if (_clan != nullptr && _brojDanaZauzetosti > 0)
			return true;
		return false;
	}
	friend bool operator==(const Knjiga &k1,const Knjiga &k2);
	friend ostream& operator<<(ostream &, const Knjiga &);
};
	bool operator==(const Knjiga &k1,const Knjiga &k2) {
		return(strcmp(k1._ISBN, k2._ISBN) == 0);
	}
ostream& operator<<(ostream & cout, const Knjiga &k)
{
	if (k._clan == nullptr) {
		cout << "Slobodna knjiga:" << endl;
		cout << "ISBN: " << k._ISBN << endl;
		cout << "Naslov knjige: " << k._naslov << endl;
		cout<< "Godina izdanja: " << k._godinaIzdanja << endl;
	}
	if (k._clan != nullptr)
	{
		cout << "Zauzeta knjiga:" << endl;
		cout << "ISBN: " << k._ISBN << endl;
		cout << "Naslov knjige: " << k._naslov << endl;
		cout << "Godina izdanja: " << k._godinaIzdanja << endl;
		cout << "Zauzeta je " << k._brojDanaZauzetosti << " dana" << endl;
		cout << *k._clan;
	}
	return cout;
}
class Polica
{
	int& _brojPolice;
	int _kapacitet;
	Kolekcija<Knjiga> _knjige;
public:
	//Potrebne konstruktor i destruktor funkcije
	Polica(int brojpolice=0,int kapacitet=0):_brojPolice(brojpolice),_kapacitet(kapacitet)
	{
	}
	//Funkciju za dodavanje nove knjige na policu. Voditi ra�una o kapacitetu police prilikom dodavanja.
	bool DodajKnjiguNaPolicu(const Knjiga &knjiga) {
		if (_knjige.GetTrenutno() == _kapacitet)
		{
			cout << "Nije moguce dodati vise knjiga na policu, jer je kapacitet pun! " << endl;
			return false;
		}
		if(_knjige.Add(knjiga))
			return true;
		return false;
	}
	void operator=(const  Polica &p)
	{
		_brojPolice = p._brojPolice;
		_kapacitet = p._kapacitet;
		for (int i = 0; i < p._knjige.GetTrenutno(); i++)
		{
			_knjige.Add(p._knjige[i]);
		}
	}
	//Funkciju za uklanjanje knjige sa police.
	bool UkloniKnjiguSaPolice(const Knjiga &k)
	{
		if (_kapacitet == 0)
		{
			cout << "Ova polica nema knjiga! " << endl;
			return false;
		}
		_knjige.RemoveByObject(k);
		_kapacitet--;
		return false;
	}
	//Info funkciju koja ispisuje podatke o svim slobodnim knjigama, svim zadu�enim knjigama, i knjigama koje su na �itanju du�e
	//od n dana, pri �emu se n zadaje kao parametar

	void Info(int brojDanaNaCitanju)
	{
		_knjige.Info();
	}
};

class Bibilioteka
{
	char* _naziv;
	Kolekcija<Polica*> _police;
	Kolekcija<Clan*> _clanovi;
public:

	//Potrebne konstruktor i destruktor funkcije
	Bibilioteka(const char *naziv="/")
	{
		int size = strlen(naziv);
		_naziv - new char[size];
		strcpy_s(_naziv, size, naziv);
	}
	~Bibilioteka()
	{
		delete[]_naziv; _naziv = nullptr;
	}
	//Operator+= dodavanje novog �lana biblioteke.
	void operator+=(Clan *c)
	{
		_clanovi.Add(c);
	}

	//Funkciju za dodavanje nove police u biblioteku.
	void DodajPolicu(Polica &p) {
		_police.Add(&p);
	}

	//Operator() za dodavanje nove knjige na policu �iji je broj proslije�en kao parametar.
	void operator()(const Knjiga &knjiga,int brojPolice)
	{



	}

	//Funkciju za uklanjanje knjige sa police �iji je broj proslije�en kao parametar.

	//Funkciju za izdavanje knjige odre�enom �lanu. Kao parametri se �alju �lanski broj i ISBN broj knjige.
	//Provjeriti prvenstveno da li je �lan u�lanjen u biblioteku.
	//Voditi ra�una o tome da li je knjiga ve� zauzeta. �lan mo�e imati najvi�e dvije zadu�ene knjige.

	//Funkciju koja razdu�uje knjigu sa proslije�enim ISBN brojem.
	
	//Operator++ koji pove�ava broj dana zauzetosti svih zadu�enih knjiga za jedan.

	//Funkciju koja pronalazi i ispisuje sve slobodne knjige na odre�enoj polici (proslijediti broj police kao parametar).

	//Funkciju koja pronalazi knjigu/e na osnovu naslova i/ili pisca i ispisuje sve podatke, kao i to da li je knjiga/e trenutno zadu�ena.

	//Funkciju koja pronalazi sve knjige koje su zadu�ene du�e od 14 dana i ispisuje podatke o �lanovima kod kojih se nalaze.
};

int main()
{
	//Testirati sve dostupne funkcionalnosti klase Biblioteka
	//Dodati najmanje 5 polica sa po 5 knjiga u objekat klase biblioteka, te u�laniti barem 10 �lanova.
	Clan *enes = new Clan("Enes Hockic", "062547786");
	Clan *nihad= new Clan("Nihad Hrustic", "061274133");
	Clan *ahmed = new Clan("Ahmed Terzic", "062650592");
	Knjiga AnaKarenjina("978-953-220-545-9", "Ana Karenjina", 1998), DervisISmrt("978-953-220-545-8", "Dervis i smrt", 1999), Proces("978-953-220-545-7", "Proces", 1998),Ruza("978-953-220-545-3", "Ruza", 1998);
	Polica polica1(1, 3);
	if (AnaKarenjina.ZaduziKnjigu(enes,5))
		cout << "knjiga zaduzena" << endl;
	if (AnaKarenjina.RazduziKnjigu())
		cout << "knjiga razduzena" << endl;
	if (AnaKarenjina.RazduziKnjigu())
		cout << "knjiga razduzena" << endl;
	if (AnaKarenjina.ZaduziKnjigu(enes, 5))
		cout << "knjiga zaduzena" << endl;
	if (DervisISmrt.ZaduziKnjigu(nihad, 8))
		cout << "knjiga zaduzena" << endl;
	if (Proces.ZaduziKnjigu(ahmed, 7))
		cout << "knjiga zaduzena" << endl;
	
	if (polica1.DodajKnjiguNaPolicu(AnaKarenjina))
		cout << "Knjiga dodana na policu!" << endl;
	if (polica1.DodajKnjiguNaPolicu(DervisISmrt))
		cout << "Knjiga dodana na policu!" << endl;
	if (polica1.DodajKnjiguNaPolicu(Proces))
		cout << "Knjiga dodana na policu!" << endl;
	if (polica1.DodajKnjiguNaPolicu(Ruza))
		cout << "Knjiga dodana na policu!" << endl;
	cout << endl;
	polica1.Info(5);

	Bibilioteka biblioteka("Biblioteka");
	//biblioteka += nihad;

	system("pause");
	return 0;
}

//978-953-220-545-9