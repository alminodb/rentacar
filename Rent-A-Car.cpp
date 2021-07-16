/*
					 _____            _                       _____
					|  __ \          | |          /\         / ____|
					| |__) |___ _ __ | |_ ______ /  \ ______| |     __ _ _ __
					|  _  // _ \ '_ \| __|______/ /\ \______| |    / _` | '__|
					| | \ \  __/ | | | |_      / ____ \     | |___| (_| | |
					|_|  \_\___|_| |_|\__|    /_/    \_\     \_____\__,_|_|

		Na projektu radili:
				* Odobašic Almin
		Pocetak rada:
				* 21.05.2021 - 00:06
		Završetak rada:
				* 02.06.2021 - 13:02
		
		Elementi projekta:
				* Login/Register system
				* Cuvanje i ucitavanje novih vozila i korisnika
				* Kreiranje novih vozila za iznajmljivanje
				* Admin system
				* Sortiranje tabele po nazivu/cijeni/godistu
				* Rentanje vozila
				* Log file
				* Istek renta koristeci sistemsko datum i vrijeme
				* Uredjivanje korisnickih podataka
				* Mogucnost provjere rentanih vozila za korisnika(njegova vlastita vozila) i za admine(sva rentana vozila)
				* System zarade

*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <dirent.h>
#include "table.h"
using namespace std;
// ------------------------------- definicije

#define naslov cout << "\n\n--------------------------------------------------" << \
	"[ RENT-A-CAR ]--------------------------------------------------\n\n"
#define naslov2 cout << "\n\n------------------------------------------------------------------------------" << \
	"[ RENT-A-CAR ]------------------------------------------------------------------------------\n\n"
#define aPanel cout << "\n\n--------------------------------------------------" << \
	"[ ADMIN-PANEL ]--------------------------------------------------\n\n"
#define MAX_VOZILA 50
#define MAX_KORISNICI 50

// --------------------------------------------------

// ------------------------------- enumeracije

enum vrstaGoriva {DIZEL = 1, BENZIN, PLIN, ELEKTRICNI};
enum tipVozila {LIMUZINA = 1, KARAVAN};
enum transmisijaVozila {MANUELNI = 1, AUTOMATIK, POLUAUTOMATIK};
enum pogonVozila{PREDNJI = 1, ZADNJI, SVACETIRI};
enum sortiranjeVozila{NISTA, ABECEDA, CIJENA, GODISTE, RENTANJE, ID};


// ---------------------------------------------------

// -------------------------------- strukture
struct KORISNIK{
	char username[16],
		password[25],
		ime[16],
		prezime[25],
		grad[25],
		godiste[5];
	int admin = 0;
};
KORISNIK *sviKorisnici = new KORISNIK[MAX_KORISNICI];
KORISNIK korisnik;

struct VOZILO{
	int id, brDana;
	char proizvodjac[25], model[16], boja[16], kubikaza[5],
		rentao[16];
	vrstaGoriva gorivo;
	tipVozila tipVozila;
	transmisijaVozila transmisija;
	pogonVozila pogon;
	int godiste, brojVrata, kilowati, kilometraza, sjedista;
	double cijena, placeno;
	bool klima, tempomat, navigacija, rentano = false;
	time_t datumRentanja, datumIsteka;
	
	// Ucitavanje svih vozila iz datoteka u strukturni niz
	void ucitajVozila(VOZILO *vozilo){
		string str, s;
		ifstream dat;
		int x = 0;
		for(int i = 0; i < MAX_VOZILA; i++){
			str = "Vozila\\" + to_string(i) + ".txt";
			dat.open(str);
			if(dat){
				while(getline(dat, s)){
					dat >> str;
					if(str == "Proizvodjac="){
						dat >> str;
						vozilo[x].proizvodjac[0] = '\0';
						strcat(vozilo[x].proizvodjac, str.c_str());
					}
					else if(str == "Model="){
						dat >> str;
						vozilo[x].model[0] = '\0';
						strcat(vozilo[x].model, str.c_str());
					}
					else if(str == "Boja="){
						dat >> str;
						vozilo[x].boja[0] = '\0';
						strcat(vozilo[x].boja, str.c_str());
					}
					else if(str == "Gorivo="){
						dat >> str;
						if(str == "DIZEL")
							vozilo[x].gorivo = DIZEL;
						else if(str == "BENZIN")
							vozilo[x].gorivo = BENZIN;
						else if(str == "PLIN")
							vozilo[x].gorivo = PLIN;
						else if(str == "ELEKTRICNI")
							vozilo[x].gorivo = ELEKTRICNI;
					}
					else if(str == "Tip="){
						dat >> str;
						if(str == "LIMUZINA")
							vozilo[x].tipVozila = LIMUZINA;
						else if(str == "KARAVAN")
							vozilo[x].tipVozila = KARAVAN;
					}
					else if(str == "Transmisija="){
						dat >> str;
						if(str == "MANUELNI")
							vozilo[x].transmisija = MANUELNI;
						else if(str == "AUTOMATIK")
							vozilo[x].transmisija = AUTOMATIK;
						else if(str == "POLUATOMATIK")
							vozilo[x].transmisija = POLUAUTOMATIK;
					}
					else if(str == "Pogon="){
						str = "";
						dat >> str;
						if(str == "PREDNJI")
							vozilo[x].pogon = PREDNJI;
						else if(str == "ZADNJI")
							vozilo[x].pogon = ZADNJI;
						else if(str == "SVACETIRI")
							vozilo[x].pogon = SVACETIRI;
					}
					else if(str == "Godiste="){
						dat >> str;
						vozilo[x].godiste = stoi(str);
					}
					else if(str == "BrojVrata="){
						dat >> str;
						vozilo[x].brojVrata = stoi(str);
					}
					else if(str == "Kilowati="){
						dat >> str;
						vozilo[x].kilowati = stoi(str);
					}
					else if(str == "Kilometraza="){
						dat >> str;
						vozilo[x].kilometraza = stoi(str);
					}
					else if(str == "Sjedista="){
						dat >> str;
						vozilo[x].sjedista = stoi(str);
					}
					else if(str == "Kubikaza="){
						dat >> str;
						strcat(vozilo[x].kubikaza, str.c_str());
					}
					else if(str == "Cijena="){
						dat >> str;
						vozilo[x].cijena = stod(str);
					}
					else if(str == "Klima="){
						dat >> str;
						if(str == "true")
							vozilo[x].klima = true;
						else
							vozilo[x].klima = false;
					}
					else if(str == "Tempomat="){
						dat >> str;
						if(str == "true")
							vozilo[x].tempomat = true;
						else
							vozilo[x].tempomat = false;
					}
					else if(str == "Navigacija="){
						dat >> str;
						if(str == "true")
							vozilo[x].navigacija = true;
						else
							vozilo[x].navigacija = false;
					}
					else if(str == "Rentano="){
						dat >> str;
						if(str == "true")
							vozilo[x].rentano = true;
						else
							vozilo[x].rentano = false;
					}
					else if(str == "DatumRenta="){
						dat >> str;
						vozilo[x].datumRentanja = stoll(str);
					}
					else if(str == "DatumIsteka="){
						dat >> str;
						vozilo[x].datumIsteka = stoll(str);
					}
					else if(str == "Rentao="){
						dat >> str;
						vozilo[x].rentao[0] = '\0';
						strcat(vozilo[x].rentao, str.c_str());
					}
					else if(str == "Placeno="){
						dat >> str;
						vozilo[x].placeno = stod(str);
					}
					else if(str == "Dani="){
						dat >> str;
						vozilo[x].brDana = stoi(str);
					}
					vozilo[x].id = i;
				}
				x++;
				dat.close();
			}
		}
	}
};
VOZILO test;
VOZILO *vozilo = new VOZILO[MAX_VOZILA];
// ---------------------------------------------------

// ------------------------------- funkcije/forwardi

void pocetnaStranica();
void greska(string str);
void prijavaKorisnika();
void registracijaKorisnika();
bool korisnikPostoji(string username);
void sacuvajKorisnika(KORISNIK k);
void ucitajKorisnika(string username);
void odjava();
int brojVozila();
void ucitajVozila(VOZILO *vozilo);
void listaVozila(sortiranjeVozila S);
void adminPanel();
void dodavanjeVozila();
void resizeConsole();
void ucitajSveKorisnike();
void listaKorisnika();
void dodajAdmina();
int nadjiKorisnika(char username[]);
void profilPanel();
void ukloniSpace(char *c);
double cijenaRentanja(double cijena, int brDana);
int nadjiVozilo(int id);
void listaRentanihVozila(sortiranjeVozila SORT);
void provjeraRentova();
void logger(string str);
double ucitajZaradu();
void sacuvajZaradu();
int nadjiVoziloID(int id);

// --------------------------------------------------

// -------------------------------- globalne varijable

const char nastavak[] = "----------------------------------------------------------------\n\tPritisni neku tipku za nastavak.\n";

string korisnikPrijavljen = "";
int brVozila = 0;
int brKorisnika = 0;
int sortV = 0;
double zarada;

// ----------------------------------------------------
int main() {

	// varijable
	int izbor;
	brVozila = brojVozila();
	test.ucitajVozila(vozilo);
	resizeConsole();
	ucitajSveKorisnike();
	provjeraRentova();
	zarada = ucitajZaradu();
	
	while(true){
		izbor = int(0);
		system("CLS");
		if(korisnikPrijavljen != "")
			cout << "\n " << korisnik.ime << " " << korisnik.prezime << " - " << korisnik.grad;
		naslov;
		cout << endl;
		pocetnaStranica();
		cout << "\tIzbor: ";
		cin >> izbor;
		if(korisnikPrijavljen == ""){
			if(izbor < 1 || izbor > 4 || !cin){
				greska("Izbor nije validan!\n");
				cin.clear();
				cin.ignore(1000, '\n');
				cout << nastavak;
				getch();
				continue;
			}
			else{
				switch(izbor){
					case 1: // prijava korisnika
						prijavaKorisnika();
						break;
					case 2: // registracija korisnika
						registracijaKorisnika();
						break;
					case 3: // lista vozila
						listaVozila(ID);
						break;
					case 4: // izlaz
						return 0;
						break;
					default:
						break;
				}
			}
		}
		else{
			if(izbor < 1 || izbor > 5 || !cin){
				greska("Izbor nije validan!\n");
				cin.clear(); cin.ignore(1000, '\n');
				cout << nastavak;
				getch();
				continue;
			}
			else{
				switch(izbor){
					case 1: // lista vozila
						listaVozila(ID);
						break;
					case 2: // profil
						profilPanel();
						break;
					case 3: // odjava
						odjava();
						break;
					case 4: // admin panel
						adminPanel();
						break;
					case 5: // izlaz
						return 0;
						break;
					default: break;
				}
			}
		}
	}


	delete[] vozilo;
	delete[] sviKorisnici;
	return 0;
}








// Pocetna stranica

void pocetnaStranica(){
	if(korisnikPrijavljen == ""){
		cout << "\t[1] Prijava\n";
		cout << "\t[2] Registracija\n";
		cout << "\t[3] Lista vozila\n";
		cout << "\t[4] Izlaz\n\n";
	}
	else{
		cout << "\t[1] Lista vozila\n";
		cout << "\t[2] Profil\n";
		cout << "\t[3] Odjava\n";
		cout << "\t[4] Admin panel\n";
		cout << "\t[5] Izlaz\n\n";
	}
}

// poruka za gresku
void greska(string str){
	cout << "\t[GRESKA] " << str;
}


// Interface za prijavu korisnika
void prijavaKorisnika(){
	char *username = new char;
	string password;
	char c;
	
	system("CLS");
	if(korisnikPrijavljen == ""){
		naslov; // ispisuje zaglavlje
		cin.ignore(1000, '\n');
		do{
			cout << "\tUsername: ";
			cin.getline(username, 15);
			if(strchr(username, 32) != NULL){
				greska("Username ne moze imati znak space u sebi!\n");
				cin.clear();
			}
		} while(strchr(username, 32) != NULL);
		cout << "\tPassword: ";
		do{
			c = getch();
			if(c != '\r' && c != 0x08 && c != 32){ // 0x08 je backspace
				password += c;
				cout << "*";
			}
		}while(c != '\r');
		cout << endl;
		if(!korisnikPostoji(username)){
			greska("Netacni username i password!\n");
			cout << nastavak;
			getch();
		}
		else{
			string str = "Korisnici\\"+string(username)+".txt";
			ifstream dat(str);
			while(getline(dat, str)){
				dat >> str;
				if(str == "Password="){
					dat >> str;
					if(!strcmp(str.c_str(), password.c_str())){
						cout << "\n\tUspjesno ste prijavljeni!\n";
						ucitajKorisnika(username);
						korisnikPrijavljen = username;
						cout << nastavak;
						getch();
						break;
					}
					else{
						cout << endl;
						greska("Netacni username i password!\n");
						cout << nastavak;
						getch();
					}
				}
			}
			dat.close();
		}
	}
	else{
		greska("Korisnik je vec prijavljen!\n");
		cout << nastavak;
		getch();
	}
	
	delete[] username;
}


// Provjera da li korisnik uopste postoji u bazi
bool korisnikPostoji(string username){
	string path = "Korisnici\\"+username+".txt";
	ifstream dat(path);
	if(dat.fail())
		return false;
	else{
		dat.close();
		return true;
	}
}


// Sistem registracije korisnika
void registracijaKorisnika(){
	system("CLS");
	naslov;
	
	string password;
	char username[16], ime[16], prezime[25];
	char grad[25], godiste[5] = "1900";
	char c;
	
	if(korisnikPrijavljen == ""){
		cin.ignore(1000, '\n');
		do{ // unos username
			cout << "\tUsername: ";
			cin.getline(username, 16);
			if(strlen(username) < 4 || strlen(username) > 15){
				greska("Username mora biti duzine izmedju 4 i 15 karaktera!\n");
			}
			if(korisnikPostoji(username)){
				greska("Taj username se vec koristi!\n");
			}
			if(strchr(username, 32) != NULL){
				greska("Username ne smije imati space znak!\n");
				continue;
			}
		} while(strlen(username) < 4 || strlen(username) > 15 || korisnikPostoji(username));
		do{ // unos password
			cout << "\tPassword: ";
			do{
				c = getch();
				if(c != '\r' && c != 0x08 && c != 32){
					password += c;
					cout << "*";
				}
			}while(c != '\r');
			cout << endl;
			if(password.length() < 4 || password.length() > 15)
				greska("Password mora biti duzine izmedju 4 i 15 karaktera!\n"), password = "";
		} while(password.length() < 4 || password.length() > 15);
		do{ // unos imena
			cout << "\tIme: ";
			cin.getline(ime, 16);
			ukloniSpace(ime);
			if(strlen(ime) < 2 || strlen(ime) > 15)
				greska("Ime mora biti duzine izmedju 2 i 15 karaktera!\n");
		} while(strlen(ime) < 2 || strlen(ime) > 15);
		if(islower(ime[0])) ime[0] = ime[0] - 32;
		do{ // unos prezimena
			cout << "\tPrezime: ";
			cin.getline(prezime, 26);
			ukloniSpace(prezime);
			if(strlen(prezime) < 3 || strlen(prezime) > 25)
				greska("Prezime mora biti duzine izmedju 3 i 15 karaktera!\n");
		} while(strlen(prezime) < 3 || strlen(prezime) > 25);
		if(islower(prezime[0]))	prezime[0] = prezime[0] - 32;
		do{ // unos grada
			cout << "\tGrad: ";
			cin.getline(grad, 25);
			ukloniSpace(grad);
			if(strlen(grad) < 2 || strlen(grad) > 15)
				greska("Grad mora biti duzine izmedju 2 i 15 karaktera!\n");
		} while(strlen(grad) < 2 || strlen(grad) > 15);
		if(islower(grad[0])) grad[0] = grad[0] - 32;
		do{
			cout << "\tGodiste: ";
			cin >> godiste;
			if(stoi(godiste) < 1900 || stoi(godiste) > 2010 || !cin){
				greska("Godiste ne moze biti manje od 1900 i vece od 2010!\n");
				cin.clear();
				cin.ignore(1000, '\n');
			}
		} while(stoi(godiste) < 1900 || stoi(godiste) > 2010 || !cin);
		korisnik.ime[0] = '\0';
		strcat(korisnik.ime, ime);
		korisnik.prezime[0] = '\0';
		strcat(korisnik.prezime, prezime);
		korisnik.username[0] = '\0';
		strcat(korisnik.username, username);
		korisnik.password[0] = '\0';
		strcat(korisnik.password, password.c_str());
		korisnik.grad[0] = '\0';
		strcat(korisnik.grad, grad);
		korisnik.godiste[0] = '\0';
		korisnik.admin = 0;
		strcat(korisnik.godiste, godiste);
		korisnikPrijavljen = username;
		sacuvajKorisnika(korisnik);
		sviKorisnici[brKorisnika++] = korisnik;
		cout << "\n\tUspjesno ste se registrovali na username: " << korisnik.username << "\n\n";
		stringstream stream;
		stream.str("");
		stream << "[REGISTRACIJA] Korisnik se registrovao na username " << korisnik.username;
		logger(stream.str());
	}
	else{
		cout << endl;
		greska("Korisnik je vec prijavljen!\n");
	}
	
	cout << nastavak;
	getch();
}

// Cuvanje korisnika u bazu
void sacuvajKorisnika(KORISNIK k){
	string path = "Korisnici\\" + string(k.username) + ".txt";
	ofstream dat(path);
	
	dat << "[INFORMACIJE O KORISNIKU]\n";
	dat << "Username= " << k.username << endl;	
	dat << "Password= " << k.password << endl;
	dat << "Ime= " << k.ime << endl;
	dat << "Prezime= " << k.prezime << endl;
	dat << "Grad= " << k.grad << endl;
	dat << "Godiste= " << k.godiste << endl;
	dat << "Admin= " << k.admin << endl;
	
	dat.close();
}

// Ucitavanje korisnika iz baze
void ucitajKorisnika(string username){
	string str = "Korisnici\\" + username + ".txt";
	ifstream dat(str);
	if(dat){
		str = "";
		
		while(getline(dat, str)){
			dat >> str;
			if(str == "Username=")
				dat >> korisnik.username;
			if(str == "Password=")
				dat >> korisnik.password;
			if(str == "Ime=")
				dat >> korisnik.ime;
			if(str == "Prezime=")
				dat >> korisnik.prezime;
			if(str == "Grad=")
				dat >> korisnik.grad;
			if(str == "Godiste=")
				dat >> korisnik.godiste;
			if(str == "Admin="){
				dat >> str;
				korisnik.admin = stoi(str);
			}
		}
		
		dat.close();
	}
	else{
		cout << endl; greska(""); cout << "\tGreska pri otvaranju korisnika " << username << endl;
	}
}

// odjavljivanje korisnika

void odjava(){
	system("CLS");
	naslov;
	char c;
	
	if(korisnikPrijavljen != ""){
		cout << "\tDa li ste sigurni da se zelite odjaviti?\n";
		cout << "\t[D] Da\n";
		cout << "\t[N] Ne\n";
		do{
			c = getch();
			if(c == 'd'){
				korisnikPrijavljen = "";
				cout << "\n\tUspjesno ste se odjavili!\n";
				break;
			}
			else if(c == 'n'){
				cout << "\n\tNiste se odjavili!\n";
				break;
			}
		} while(c != 'd' && c != 'n');
	}
	
	cout << nastavak;
	getch();
}

// vraca broj vozila u datoteci
int brojVozila(){
	ifstream dat;
	string str;
	int n = 0;
	
	for(int x = 0; x < MAX_VOZILA; x++){
		str = "Vozila\\" + to_string(x) + ".txt";
		dat.open(str);
		if(dat){
			n++;
			dat.close();
		}
	}
	return n;
}

int nextID(){
	ifstream dat;
	string str;
	for(int x = 0; x < MAX_VOZILA; x++){
		str = "Vozila\\" + to_string(x) + ".txt";
		dat.open(str);
		if(dat){
			dat.close();
		}
		else{
			return x;
		}
	}
}

// Sacuvanje vozila u datoteku
void sacuvajVozilo(VOZILO v){
	ofstream dat;
	int x = nextID();
	string str = "Vozila\\" + to_string(x) + ".txt";
	dat.open(str);
	
	dat << "[PODACI O VOZILU]" << endl;
	dat << "Proizvodjac= " << v.proizvodjac << endl;
	dat << "Model= " << v.model << endl;
	dat << "Boja= " << v.boja << endl;
	dat << "Gorivo= ";
	switch(v.gorivo){
		case DIZEL:
			dat << "DIZEL" << endl;
			break;
		case BENZIN:
			dat << "BENZIN" << endl;
			break;
		case PLIN:
			dat << "PLIN" << endl;
			break;
		case ELEKTRICNI:
			dat << "ELEKTRICNI" << endl;
		default: break;
	}
	dat << "Tip= ";
	switch(v.tipVozila){
		case LIMUZINA:
			dat << "LIMUZINA" << endl;
			break;
		case KARAVAN:
			dat << "KARAVAN" << endl;
			break;
		default: break;			
	}
	dat << "Transmisija= ";
	switch(v.transmisija){
		case MANUELNI:
			dat << "MANUELNI" << endl;
			break;
		case AUTOMATIK:
			dat << "AUTOMATIK" << endl;
			break;
		case POLUAUTOMATIK:
			dat << "POLUAUTOMATIK" << endl;
			break;
		default: break;
	}
	dat << "Pogon= ";
	switch(v.pogon){
		case PREDNJI:
			dat << "PREDNJI" << endl;
			break;
		case ZADNJI:
			dat << "ZADNJI" << endl;
			break;
		case SVACETIRI:
			dat << "SVACETIRI" << endl;
			break;
		default: break;
	}
	dat << "Godiste= " << v.godiste << endl;
	dat << "BrojVrata= " << v.brojVrata << endl;
	dat << "Kilowati= " << v.kilowati << endl;
	dat << "Kilometraza= " << v.kilometraza << endl;
	dat << "Sjedista= " << v.sjedista << endl;
	dat << "Kubikaza= " << v.kubikaza << endl;
	dat << "Cijena= " << v.cijena << endl;
	dat << "Klima= " << ((v.klima) ? "true" : "false") << endl;
	dat << "Tempomat= " << ((v.tempomat) ? "true" : "false") << endl;
	dat << "Navigacija= " << ((v.navigacija) ? "true" : "false") << endl;
	dat << "Rentano= " << ((v.rentano) ? "true" : "false") << endl;
	dat << "DatumRenta= " << v.datumRentanja << endl;
	dat << "DatumIsteka= " << v.datumIsteka << endl;
	dat << "Rentao= " << v.rentao << endl;
	dat << "Placeno= " << v.placeno << endl;
	dat << "Dani= " << v.brDana << endl;
	
	dat.close();
}

// funkcije za sortiranje nizova
void sortByName(VOZILO v[]){
	string str1, str2;
	for(int x = 0; x < brVozila; x++){
		for(int y = x+1; y < brVozila; y++){
			str1 = v[x].proizvodjac;
			str2 = v[y].proizvodjac;
			if(sortV != 0){
				if(str1 < str2){
					swap(v[x], v[y]);
				}
			}
			else{
				if(str1 > str2){
					swap(v[x], v[y]);
				}
			}
		}
	}
	sortV = abs(sortV - 1);
}
void sortByPrice(VOZILO v[]){
	for(int x = 0; x < brVozila; x++){
		for(int y = x + 1; y < brVozila; y++){
			if(sortV != 0){
				if(v[x].cijena < v[y].cijena){
					swap(v[x], v[y]);
				}
			}
			else{
				if(v[x].cijena > v[y].cijena){
					swap(v[x], v[y]);
				}
			}
		}
	}
	sortV = abs(sortV - 1);
}
void sortByYear(VOZILO v[]){
	for(int x = 0; x < brVozila; x++){
		for(int y = x + 1; y < brVozila; y++){
			if(sortV != 0){
				if(v[x].godiste < v[y].godiste){
					swap(v[x], v[y]);
				}
			}
			else{
				if(v[x].godiste > v[y].godiste){
					swap(v[x], v[y]);
				}
			}
		}
	}
	sortV = abs(sortV - 1);
}
void sortByID(VOZILO v[]){
	for(int x = 0; x < brVozila; x++){
		for(int y = x + 1; y < brVozila; y++){
			if(sortV != 0){
				if(v[x].id < v[y].id){
					swap(v[x], v[y]);
				}
			}
			else{
				if(v[x].id > v[y].id){
					swap(v[x], v[y]);
				}
			}
		}
	}
	sortV = abs(sortV - 1);
}

// provjera validnosti vozila
bool validID(int index){
	if(vozilo[index].proizvodjac[0] != '\0')
		return true;
	else
		return false;
}

// funkcija koja izlistava sva vozila

void listaVozila(sortiranjeVozila S){
	
	system("CLS");
	naslov2;
	VOZILO v;
	
	switch(S){
		case NISTA:
			break;
		case ABECEDA:
			sortByName(vozilo);
			break;
		case CIJENA:
			sortByPrice(vozilo);
			break;
		case GODISTE:
			sortByYear(vozilo);
			break;
		case ID:
			sortByID(vozilo);
			break;
		default: break;
	}
	
	
	
	cout << right << setw(30) << "[A] Sortiraj po abecedi" <<
			right << setw(30) << "[C] Sortiraj po cijeni" << 
			right << setw(30) << "[G] Sortiraj po godistu";
		
	if(korisnikPrijavljen != "")	
		cout << right << setw(30) << "[R] Rentaj vozilo";
		
	cout << "\n\n";

	
	TextTable t('-', '|', '+');
	t.add("ID");
	t.add("Proizvodjac");
	t.add("Model");
	t.add("Boja");
	t.add("Gorivo");
	t.add("Tip");
	t.add("Transmisija");
	t.add("Pogon");
	t.add("Godiste");
	t.add("Vrata");
	t.add("kW");
	t.add("Kilometraza");
	t.add("Sjedista");
	t.add("Kubikaza");
	t.add("Cijena");
	t.add("Klima");
	t.add("Tempomat");
	t.add("Navigacija");
	t.endOfRow();
	cout << setfill(' ') << " \n";
	for(int x = 0; x < MAX_VOZILA; x++){
		v = vozilo[x];
		if(validID(x) && strlen(v.proizvodjac) > 1 && !v.rentano){
			t.add(to_string(v.id));
			t.add(v.proizvodjac);
			t.add(v.model);
			t.add(v.boja);
			switch(v.gorivo){
				case DIZEL:
					t.add("Dizel");
					break;
				case BENZIN:
					t.add("Benzin");
					break;
				case PLIN:
					t.add("Plin");
					break;
				case ELEKTRICNI:
					t.add("Elek.");
				default: break;
			}
			switch(v.tipVozila){
				case LIMUZINA:
					t.add("Limuzina");
					break;
				case KARAVAN:
					t.add("Karavan");
					break;
				default: break;			
			}
			switch(v.transmisija){
				case MANUELNI:
					t.add("Manuelni");
					break;
				case AUTOMATIK:
					t.add("Automatik");
					break;
				case POLUAUTOMATIK:
					t.add("Poluautomatik");
					break;
				default: break;			
			}
			switch(v.pogon){
				case PREDNJI:
					t.add("Prednji");
					break;
				case ZADNJI:
					t.add("Zadnji");
					break;
				case SVACETIRI:
					t.add("4x4");
					break;
				default: break;
			}
			t.add(to_string(v.godiste));
			t.add(to_string(v.brojVrata));
			t.add(to_string(v.kilowati));
			t.add(to_string(v.kilometraza));
			t.add(to_string(v.sjedista));
			t.add(v.kubikaza);
			stringstream stream;
			stream << fixed << setprecision(2) << v.cijena;
			string sstr = stream.str() + " BAM";
			t.add(sstr);
			t.add(((v.klima) ? "Da" : "Ne"));
			t.add(((v.tempomat) ? "Da" : "Ne"));
			t.add(((v.navigacija) ? "Da" : "Ne"));
		
			t.endOfRow();
		}
	}
	cout << t << endl;
	
	if(S != RENTANJE){
		cout << endl << endl;
		cout << nastavak << endl;
		char c = getch();
		if(c == 'a' || c == 'A'){
			listaVozila(ABECEDA);
			sortV = abs(sortV - 1);
		}
		if(c == 'c' || c == 'C'){
			listaVozila(CIJENA);
			sortV = abs(sortV - 1);
		}
		if(c == 'g' || c == 'G'){
			listaVozila(GODISTE);
			sortV = abs(sortV - 1);
		}
		if((c == 'r' || c == 'R') && korisnikPrijavljen != ""){
			listaVozila(RENTANJE);
		}
	}
	else{
		int izbor, dani;
		do{
			cout << "\tUnesi ID vozila kojeg zelite rentati: ";
			cin >> izbor;
			izbor = nadjiVoziloID(izbor);
			if(!validID(nadjiVoziloID(izbor)) || !cin || vozilo[izbor].rentano){
				greska("Vozilo sa tim ID-om ne postoji!\n");
				cin.clear(); cin.ignore(1000, '\n');
				cout << endl << nastavak << endl; getch();
				return;
			}
		} while(!validID(nadjiVoziloID(izbor)) || !cin);
		do{
			cout << "\tUnesi na koliko dana zelis rentati vozilo: ";
			cin >> dani;
			if(dani < 1 || dani > 30 || !cin){
				greska("Rentati mozete na izmedju 1 i 30 dana!\n");
				cin.clear(); cin.ignore(1000, '\n');
				cout << endl << nastavak << endl; getch();
				return;
			}
		} while(dani < 1 || dani > 30 || !cin);
			system("CLS");
			naslov2;
			char c;
			cout << "\n\tDa li ste sigurni da zelite rentati vozilo " <<
					vozilo[izbor].proizvodjac << " " << vozilo[izbor].model << " za " << cijenaRentanja(vozilo[izbor].cijena, dani) << "?" <<
					"\n\t[D] Da\n\t[N] Ne\n";
			while(true){
				c = getch();
				if(c != 'n' && c != 'N' && c != 'd' && c != 'D')
					continue;
				else
					break;
			}
			if(c == 'd' || c == 'D'){		
				cout << "\n\tUspjesno ste rentali vozilo marke " << vozilo[izbor].proizvodjac << endl;
				cout << "\tRentano na " << dani << ((dani == 1) ? " dan\n":" dana\n");
				vozilo[izbor].placeno = cijenaRentanja(vozilo[izbor].cijena, dani); // cijena koja je placena za rentanje vozila na x dana
				cout << "\tCijena rentanja: " << vozilo[izbor].placeno << " BAM" << endl;
				stringstream strm;
				strm << "[RENTANJE VOZILA]" << " Korisnik " << korisnikPrijavljen << " je rentao vozilo " <<
						vozilo[izbor].proizvodjac << " " << vozilo[izbor].model;
				logger(strm.str());
				
				time_t sada = time(0);
				
				zarada += vozilo[izbor].placeno;
				sacuvajZaradu();
				vozilo[izbor].datumRentanja = sada;
				vozilo[izbor].datumIsteka = sada+86400*dani;
				vozilo[izbor].brDana = dani;
				vozilo[izbor].rentano = true;
				vozilo[izbor].rentao[0] = '\0';
				strcat(vozilo[izbor].rentao, korisnik.username);
				if(izbor != -1){
					string str = "Vozila\\" + to_string(nadjiVozilo(izbor)) + ".txt";
					remove(str.c_str());
					sacuvajVozilo(vozilo[izbor]);
				}
				else{
					greska("Doslo je do greske prilikom rentanja! Vozilo ne postoji u bazi!\n");
				}
			}
			else if(c == 'n' || c == 'N'){
				cout << "\n\n\tOdustali ste od rentanja vozila!\n";
			}
		cout << endl << nastavak; getch();
	}
}

// izlistavanje rentanih vozila
void listaRentanihVozila(sortiranjeVozila S){
	
	system("CLS");
	naslov;
	VOZILO v;
	
	switch(S){
		case NISTA:
			break;
		case ABECEDA:
			sortByName(vozilo);
			break;
		case CIJENA:
			sortByPrice(vozilo);
			break;
		case GODISTE:
			sortByYear(vozilo);
			break;
		case ID:
			sortByID(vozilo);
			break;
		default: break;
	}
	
	
	
	cout << right << setw(30) << "[A] Sortiraj po abecedi" <<
			right << setw(30) << "[C] Sortiraj po cijeni" << 
			right << setw(30) << "[G] Sortiraj po godistu";
		
	cout << "\n\n";

	
	TextTable t('-', '|', '+');
	t.add("ID");
	t.add("Proizvodjac");
	t.add("Model");
	t.add("Cijena");
	t.add("Dani");
	t.add("Vrijeme rentanja");
	t.add("Vrijeme isteka");
	t.add("Rentao");
	t.endOfRow();
	cout << setfill(' ') << " \n";
	for(int x = 0; x < MAX_VOZILA; x++){
		v = vozilo[x];
		if(validID(x) && strlen(v.proizvodjac) > 1 && v.rentano){
			t.add(to_string(v.id));
			t.add(v.proizvodjac);
			t.add(v.model);

			stringstream stream;
			stream << fixed << setprecision(2) << v.placeno;
			string str = stream.str() + " BAM";
			t.add(str);
			t.add(to_string(v.brDana));
			
			// vrijeme rentanja
			stream.str("");
			time_t vrijemeRentanja = v.datumRentanja;
			tm *local = localtime(&vrijemeRentanja);
			stream << local->tm_mday << "." << local->tm_mon+1 << "." << local->tm_year+1900 <<
					" - " << local->tm_hour << ":" << local->tm_min << ":" << local->tm_sec;
			str = stream.str();
			t.add(str);
			
			// vrijeme isteka
			stream.str("");
			time_t vrijemeIsteka = v.datumIsteka;
			local = localtime(&vrijemeIsteka);
			stream << local->tm_mday << "." << local->tm_mon+1 << "." << local->tm_year+1900 <<
					" - " << local->tm_hour << ":" << local->tm_min << ":" << local->tm_sec;
			str = stream.str();
			t.add(str);
			
			// username koji je rentao vozilo
			t.add(v.rentao);
					
			t.endOfRow();
		}
	}
	cout << t << endl;
	
	cout << endl << endl;
	cout << nastavak << endl;
	char c = getch();
	if(c == 'a' || c == 'A'){
		listaRentanihVozila(ABECEDA);
		sortV = abs(sortV - 1);
	}
	if(c == 'c' || c == 'C'){
		listaRentanihVozila(CIJENA);
		sortV = abs(sortV - 1);
	}
	if(c == 'g' || c == 'G'){
		listaRentanihVozila(GODISTE);
		sortV = abs(sortV - 1);
	}
}

// admin panel

void adminPanel(){
	if(korisnik.admin == 1){
		int izbor;
		while(true){
			system("CLS");
			aPanel;
			cout << "\t[1] Lista korisnika\n";
			cout << "\t[2] Lista rentanih vozila\n";
			cout << "\t[3] Dodaj vozilo\n";
			cout << "\t[4] Dodaj admina\n";
			cout << "\t[5] Ukupna zarada\n";
			cout << "\t[6] Pocetna stranica\n\n";
			do{
				cout << "\tIzbor: ";
				cin >> izbor;
				if(izbor < 1 || izbor > 6 || !cin){
					greska("Pogresan izbor!\n");
					cin.clear(); cin.ignore(1000, '\n');
				}
			} while(izbor < 1 || izbor > 6|| !cin);
			
			switch(izbor){
				case 1: // lista korisnika
					listaKorisnika();
					break;
				case 2: // lista rentanih vozila
					listaRentanihVozila(sortiranjeVozila::ID);
					break;
				case 3: // dodavanje vozila
					dodavanjeVozila();
					break;
				case 4: // dodavanje admina
					dodajAdmina();
					break;
				case 5: // ukupna zarada
					{
						system("CLS");
						naslov;
						if(korisnik.admin){
							cout << "\tUkupna zarada u firmi: " << zarada << " BAM" << endl;
							cout << endl << nastavak; getch();
						}
						break;
					}
				case 6: // vracanje na main menu
					return;
					break;
				default: break;
			}
		}
	}
	else{
		system("CLS");
		cout << "\n\n------------------------------------[ ADMIN PANEL ]------------------------------------\n\n";
		greska("Samo administratori imaju pristup ovoj opciji!\n");
		cout << nastavak << endl;
		getch();
	}
		
}

// uklanjanje praznog mjesta iz niza karaktera
void ukloniSpace(char *c){
	while(*c != '\0'){
		if(*c == 32)
			*c = '_';
		c++;
	}
}


void dodavanjeVozila(){
	system("CLS");
	naslov;
	VOZILO v;
	int izbor;
	cin.ignore(1000, '\n');

	cout << "\tProizvodjac: ";
	cin.getline(v.proizvodjac, 25);
	ukloniSpace(v.proizvodjac);
	cout << "\tModel: ";
	cin.getline(v.model, 15);
	ukloniSpace(v.model);
	if(strlen(v.proizvodjac) < 1 || strlen(v.model) < 1){
		greska("Pogresan unos! Smatra se da ste odustali od dodavanja vozila!\n");
		cout << endl << nastavak; getch();
		return;
	}
	cout << "\tBoja: ";
	cin >> v.boja;
	cout << "\tGorivo:\n";
	cout << "\t\t[1] Dizel\n";
	cout << "\t\t[2] Benzin\n";
	cout << "\t\t[3] Plin\n";
	cout << "\t\t[4] Elektricni\n";
	do{
		cout << "\tIzbor: ";
		cin >> izbor;
		if(izbor < 1 || izbor > 4 || !cin){
			greska("Pogresan izbor!\n");
			cin.clear(); cin.ignore(1000, '\n');
		}
		else{
			switch(izbor){
				case 1:	v.gorivo = DIZEL; break;
				case 2: v.gorivo = BENZIN; break;
				case 3: v.gorivo = PLIN; break;
				case 4: v.gorivo = ELEKTRICNI; break;
				default: break;
			}
		}
	} while(izbor < 1 || izbor > 4 || !cin);
	cout << "\tTip vozila:\n";
	cout << "\t\t[1] Limuzina\n\t\t[2] Karavan\n";
	do{
		cout << "\tIzbor: ";
		cin >> izbor;
		if(izbor < 1 || izbor > 2 || !cin){
			greska("Pogresan izbor!\n");
			cin.clear(); cin.ignore(1000, '\n');
		}
		else{
			switch(izbor){
				case 1: v.tipVozila = LIMUZINA; break;
				case 2: v.tipVozila = KARAVAN; break;
				default: break;
			}
		}
	} while(izbor < 1 || izbor > 2 || !cin);
	cout << "\tTransmisija:\n";
	cout << "\t\t[1] Manuelni\n\t\t[2] Automatik\n\t\t[3] Poluautomatik\n";
	do{
		cout << "\tIzbor: ";
		cin >> izbor;
		if(izbor < 1 || izbor > 3 || !cin){
			greska("Pogresan izbor!\n");
			cin.clear(); cin.ignore(1000, '\n');
		}
		else{
			switch(izbor){
				case 1: v.transmisija = MANUELNI; break;
				case 2: v.transmisija = AUTOMATIK; break;
				case 3: v.transmisija = POLUAUTOMATIK; break;
				default: break;
			}
		}
	} while(izbor < 1 || izbor > 3 || !cin);
	cout << "\tPogon:\n";
	cout << "\t\t[1] Prednji\n\t\t[2] Zadnji\n\t\t[3] 4x4\n";
	do{
		cout << "\tIzbor: ";
		cin >> izbor;
		if(izbor < 1 || izbor > 3 || !cin){
			greska("Pogresan izbor!\n");
			cin.clear(); cin.ignore(1000, '\n');
		}
		else{
			switch(izbor){
				case 1: v.pogon = PREDNJI; break;
				case 2: v.pogon = ZADNJI; break;
				case 3: v.pogon = SVACETIRI; break;
				default: break;
			}
		}
	} while(izbor < 1 || izbor > 3 || !cin);
	cout << "\tGodiste: ";
	cin >> v.godiste;
	cout << "\tBroj vrata: ";
	cin >> v.brojVrata;
	cout << "\tKilowati: ";
	cin >> v.kilowati;
	cout << "\tKilometraza: ";
	cin >> v.kilometraza;
	cout << "\tBroj sjedista: ";
	cin >> v.sjedista;
	cout << "\tKubikaza: ";
	cin >> v.kubikaza;
	cout << "\tCijena rentanja po danu: ";
	cin >> v.cijena;
	cout << "\tKlima:\n";
	cout << "\t\t[1] Da\n\t\t[2] Ne\n";
	do{
		cout << "\tIzbor: ";
		cin >> izbor;
		if(izbor == 1)
			v.klima = true;
		else if(izbor == 2)
			v.klima = false;
		else
			greska("Pogresan izbor!\n");
	} while(izbor != 1 && izbor != 2);
	cout << "\tTempomat:\n";
	cout << "\t\t[1] Da\n\t\t[2] Ne\n";
	do{
		cout << "\tIzbor: ";
		cin >> izbor;
		if(izbor == 1)
			v.tempomat = true;
		else if(izbor == 2)
			v.tempomat = false;
		else
			greska("Pogresan izbor!\n");
	} while(izbor != 1 && izbor != 2);
	cout << "\tNavigacija:\n";
	cout << "\t\t[1] Da\n\t\t[2] Ne\n";
	do{
		cout << "\tIzbor: ";
		cin >> izbor;
		if(izbor == 1)
			v.navigacija = true;
		else if(izbor == 2)
			v.navigacija = false;
		else
			greska("Pogresan izbor!\n");
	} while(izbor != 1 && izbor != 2);
	v.rentano = false;
	v.rentao[0] = '\0';
	v.brDana = 0;
	v.id = nextID();

	sacuvajVozilo(v);
	vozilo[brVozila++] = v;
	
	cout << "\n\n\tUspjesno ste dodali vozilo u bazu!\n";
	stringstream stream;
	stream.str("");
	stream << "[DODAVANJE VOZILA] Dodato vozilo u bazu: " << v.proizvodjac << " " << v.model;
	logger(stream.str());
	cout << nastavak; getch();
}


// Promjena velicine konzole
void resizeConsole(){
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, 1400, 600, TRUE);
}

// Izlistavanje svih korisnika koji se nalaze u bazi

void ucitajSveKorisnike(){
	
	// directory varijable
	DIR *dir;
	dirent *entry;
	// fajl varijable
	ifstream dat;
	string str, s;
	int &x = brKorisnika;
	dir = opendir("Korisnici\\");
	if(dir){
		while((entry = readdir(dir)) != NULL){
			if(entry->d_name[0] != '.'){
				s = "Korisnici\\";
				s += entry->d_name;
				dat.open(s);
				if(dat){
					while(getline(dat, str)){
						dat >> str;
						if(str == "Ime="){
							dat >> str;
							sviKorisnici[x].ime[0] = '\0';
							strcat(sviKorisnici[x].ime, str.c_str());
						}
						else if(str == "Prezime="){
							dat >> str;
							sviKorisnici[x].prezime[0] = '\0';
							strcat(sviKorisnici[x].prezime, str.c_str());
						}
						else if(str == "Username="){
							dat >> str;
							sviKorisnici[x].username[0] = '\0';
							strcat(sviKorisnici[x].username, str.c_str());
						}
						else if(str == "Password="){
							dat >> str;
							sviKorisnici[x].password[0] = '\0';
							strcat(sviKorisnici[x].password, str.c_str());
						}
						else if(str == "Grad="){
							dat >> str;
							sviKorisnici[x].grad[0] = '\0';
							strcat(sviKorisnici[x].grad, str.c_str());
						}
						else if(str == "Godiste="){
							dat >> str;
							sviKorisnici[x].godiste[0] = '\0';
							strcat(sviKorisnici[x].godiste, str.c_str());
						}
						else if(str == "Admin="){
							dat >> str;
							sviKorisnici[x].admin = stoi(str);
						}
					}
					x++;
					dat.close();
				}
				else cout << "Nemoguce otvoriti " << s << endl;
			}
		}
		
		closedir(dir);
	}
	
}


// Lista svih korisnika
void listaKorisnika(){
	system("CLS");
	naslov;
	
	cout << "\t\tLISTA SVIH KORISNIKA\n\n";
	TextTable tb('-', '|', '+');
	tb.add("Ime"); tb.add("Prezime"); tb.add("Username"); tb.add("Password"); tb.add("Grad"); tb.add("Godiste"); tb.add("Admin");
	tb.endOfRow();
	
	for(int x = 0; x < brKorisnika; x++){
		tb.add(sviKorisnici[x].ime);
		tb.add(sviKorisnici[x].prezime);
		tb.add(sviKorisnici[x].username);
		tb.add(sviKorisnici[x].password);
		tb.add(sviKorisnici[x].grad);
		tb.add(sviKorisnici[x].godiste);
		tb.add((sviKorisnici[x].admin == 1) ? "Da" : "Ne");
		tb.endOfRow();
	}
	
	cout << tb << endl;
	
	cout << "\n\n" << nastavak; getch();
	
}

// dodavanje admina u bazu

void dodajAdmina(){
	
	system("CLS");
	naslov;
	KORISNIK k;
	
	cout << "\n\t\tDODAVANJE ADMINA\n\n";
	cout << "\tUnesi username korisnika: ";
	cin >> k.username;
	string str = "Korisnici\\" + string(k.username) + ".txt";
	
	ifstream dat(str);
	
	if(dat){
		
		while(getline(dat, str)){
			dat >> str;
			if(str == "Username=")
				dat >> k.username;
			if(str == "Password=")
				dat >> k.password;
			if(str == "Ime=")
				dat >> k.ime;
			if(str == "Prezime=")
				dat >> k.prezime;
			if(str == "Grad=")
				dat >> k.grad;
			if(str == "Godiste=")
				dat >> k.godiste;
			if(str == "Admin="){
				dat >> str;
				k.admin = stoi(str);
			}
		}
		if(k.admin == 1){
			greska("Taj username je vec administrator!\n");
			cout << "\tDa li zelite skinuti admina tom usernameu?\n";
			cout << "\t[1] DA\n\t[2] NE\n";
			int izbor;
			do{
				cout << "\tIzbor: ";
				cin >> izbor;
				if(izbor < 1 || izbor > 2 || !cin){
					greska("Pogresan izbor!\n");
					cin.clear(); cin.ignore(1000, '\n');
				}
			} while(izbor < 1 || izbor > 2 || !cin);
			switch(izbor){
				case 1: 
					{
					k.admin = 0; 
					sacuvajKorisnika(k);
					sviKorisnici[nadjiKorisnika(k.username)].admin = 0;
					cout << "\tUspjesno ste skinuli admina za username " << k.username << endl;
					stringstream stream;
					stream.str("");
					stream << "[SKIDANJE ADMINA] Admin " << korisnik.username << " je skinuo administratorsku poziciju korisniku " << k.username;
					logger(stream.str());
					}
					break;
				case 2: cout << "\tOdustali ste od skidanja admina za tog korisnika!\n"; break;
				default: break;
			}
		}
		else if(k.admin == 0){
			cout << "\tDali ste funkciju administratora za username " << k.username << endl;
			k.admin = 1;
			sacuvajKorisnika(k);
			sviKorisnici[nadjiKorisnika(k.username)].admin = 1;
			stringstream stream;
			stream.str("");
			stream << "[NOVI ADMIN] Admin " << korisnik.username << " je dao administratorsku poziciju korisniku " << k.username;
			logger(stream.str());
		}
		
		dat.close();
	}
	else{
		cout << "\tTaj username ne postoji u bazi!\n";
	}
	
	
		
	cout << nastavak;
	getch();
}
int nadjiKorisnika(char username[]){
	for(int x = 0; x < MAX_KORISNICI; x++){
		if(!strcmp(sviKorisnici[x].username, username)){
			return x;
		}
	}
}
int nadjiVozilo(int id){ // trazi fajl id vozila
	for(int x = 0; x < brVozila; x++){
		if(vozilo[id].id == x)
			return x;
	}
	return -1;
}
int nadjiVoziloID(int id){ // trazi id vozila po id od fajla
	for(int x = 0; x < MAX_VOZILA; x++){
		if(vozilo[x].id == id){
			return x;
		}
	}
	return -1;
}

// profil korisnika
void profilPanel(){
	system("CLS");
	naslov;
	
	char c;
	string str, novistr;
	int izbor;
	
	cout << "\t[1] Informacije\n";
	cout << "\t[2] Promjena passworda\n";
	cout << "\t[3] Promjena username-a\n";
	cout << "\t[4] Promjena imena i prezimena\n";
	cout << "\t[5] Promjena grada\n";
	cout << "\t[6] Promjena godista\n";
	cout << "\t[7] Informacije o rentanim vozilima\n";
	cout << "\t[8] Pocetna stranica\n\n";
	
	do{
		cout << "\tIzbor: ";
		cin >> izbor;
		if(izbor < 1 || izbor > 8 || !cin){
			greska("Pogresan izbor!\n");
			cin.clear(); cin.ignore(1000, '\n');
		}
	} while(izbor < 1 || izbor > 8 || !cin);
	
	switch(izbor){
		case 1: // informacije
			system("CLS");
			naslov;
			cout << "\tIme: " << korisnik.ime << endl;
			cout << "\tPrezime: " << korisnik.prezime << endl;
			cout << "\tUsername: " << korisnik.username << endl;
			cout << "\tPassword: PRITISNI 'p' DA VIDIS PASSWORD" << endl;
			cout << "\tGrad: " << korisnik.grad << endl;
			cout << "\tGodiste: " << korisnik.godiste << endl;
			cout << "\tAdministrator: " << ((korisnik.admin == 1) ? "Da" : "Ne") << endl;
			cout << "\n\n" << nastavak;
			if(getch() == 'p'){
				system("CLS");
				naslov;
				cout << "\tIme: " << korisnik.ime << endl;
				cout << "\tPrezime: " << korisnik.prezime << endl;
				cout << "\tUsername: " << korisnik.username << endl;
				cout << "\tPassword: " << korisnik.password << endl;
				cout << "\tGrad: " << korisnik.grad << endl;
				cout << "\tGodiste: " << korisnik.godiste << endl;
				cout << "\tAdministrator: " << ((korisnik.admin == 1) ? "Da" : "Ne") << endl << endl;
			}
			else{
				return;
			}
			
			
			break;
		case 2: // promjena stra
			system("CLS");
			naslov;
			do{
				cout << "\tUnesite novi password: ";
				str = "";
				do{
					c = getch();
					if(c != '\r' && c != 0x08 && c != 32){
						str += c;
						cout << "*";
					}
				}while(c != '\r');
				cout << endl;
				cout << "\tPonovite novi password: ";
				novistr = "";
				do{
					c = getch();
					if(c != '\r' && c != 0x08 && c != 32){
						novistr += c;
						cout << "*";
					}
				}while(c != '\r');
				cout << endl;
				if(!strcmp(str.c_str(), novistr.c_str())){
					korisnik.password[0] = '\0';
					strcat(korisnik.password, str.c_str());
					cout << "\tUspjesno ste promijenili vas password!\n\n";
					sacuvajKorisnika(korisnik);
					izbor = nadjiKorisnika(korisnik.username); // koristi se varijabla izbor da se sacuva memorija
					sviKorisnici[izbor].password[0] = '\0';
					strcat(sviKorisnici[izbor].password, korisnik.password);					
				}
				else if(str.length() < 4 || str.length() > 15)
					greska("Password mora biti duzine izmedju 4 i 15 karaktera!\n");
				else{
					greska("Lozinke se ne poklapaju!\n");
				}
			} while(strcmp(str.c_str(), novistr.c_str()) || str.length() < 4 || str.length() > 15);
			
			
			break;
		case 3: // promjena usernamea
			
			system("CLS");
			naslov;
			cin.ignore(1000, '\n');
			while(true){
				str = ""; // koristena varijabla iz case 2 da bi se sacuvala memorija
				cout << "\tUnesi novi username: ";
				getline(cin, str);
				if(str.find(' ') != -1){
					greska("Username ne smije sadrzati space znak!\n");
					continue;
				}
				else if(str.length() < 4 || str.length() > 15){
					greska("Username mora biti duzine izmedju 4 i 15 znakova!\n");
					continue;
				}
				else if(korisnikPostoji(str)){
					greska("Taj username se vec koristi!\n");
					continue;
				}
				else{
					cout << "\tUspjesno ste promijenili username u " << str << endl;
					novistr = "Korisnici\\" + string(korisnik.username) + ".txt";
					remove(novistr.c_str());
					izbor = nadjiKorisnika(korisnik.username); // koristi se varijabla izbor da se sacuva memorija
					korisnik.username[0] = '\0';
					strcat(korisnik.username, str.c_str());
					sacuvajKorisnika(korisnik);
					sviKorisnici[izbor].username[0] = '\0';
					strcat(sviKorisnici[izbor].username, korisnik.username);
					break;
				}
			}
			break;
		case 4: // promjena imena i prezimena
			
			system("CLS");
			naslov;
			cin.ignore(1000, '\n');	
			while(true){
				cout << "\tUnesi ime: ";
				getline(cin, str);
				if(str.length() < 2 || str.length() > 15){
					greska("Ime mora biti duzine izmedju 2 i 15 karaktera!\n");
					continue;
				}
				else if(str.find(' ') != -1){
					greska("Ime ne smije sadrzati znak space!\n");
					continue;
				}
				cout << "\tUnesi prezime: ";
				getline(cin, novistr);
				if(novistr.length() < 2 || novistr.length() > 15){
					greska("Prezime mora biti duzine izmedju 2 i 15 karaktera!\n");
					continue;
				}
				else if(novistr.find(' ') != -1){
					greska("Ime ne smije sadrzati znak space!\n");
					continue;
				}
				else{
					cout << "\tUspjesno ste promijenili ime i prezime u: " << str << " " << novistr << endl;
					korisnik.ime[0] = '\0'; korisnik.prezime[0] = '\0';
					strcat(korisnik.ime, str.c_str());
					strcat(korisnik.prezime, novistr.c_str());
					sacuvajKorisnika(korisnik);
					izbor = nadjiKorisnika(korisnik.username); // koristi se varijabla izbor da se sacuva memorija
					sviKorisnici[izbor].ime[0] = '\0';
					sviKorisnici[izbor].prezime[0] = '\0';
					strcat(sviKorisnici[izbor].ime, korisnik.ime);
					strcat(sviKorisnici[izbor].prezime, korisnik.prezime);
					break;
				}
				break;
			}
			
			break;
		case 5: // promjena grada
			
			system("CLS");
			naslov;
			cin.ignore(1000, '\n');
			while(true){
				cout << "\tUnesi naziv grada u kojem zivite: ";
				getline(cin, str);
				if(str.length() < 2 || str.length() > 25){
					greska("Duzina imena grada ne smije biti manja od 2 i veca od 25 karaktera!\n");
					continue;
				}
				else{
					cout << "\tUspjesno ste promijenili grad u kojem zivite u " << str << endl;
					korisnik.grad[0] = '\0';
					strcat(korisnik.grad, str.c_str());
					ukloniSpace(korisnik.grad);
					sacuvajKorisnika(korisnik);
					izbor = nadjiKorisnika(korisnik.username); // koristi se varijabla izbor da se sacuva memorija
					sviKorisnici[izbor].grad[0] = '\0';
					strcat(sviKorisnici[izbor].grad, korisnik.grad);
					break;
				}
			}
			
			break;
		case 6: // promjena godista
			system("CLS");
			naslov;
			cin.ignore(1000, '\n');
			while(true){
				cout << "\tUnesite vase godiste: ";
				getline(cin, str);
				if(stoi(str) < 1900 || stoi(str) > 2010){
					greska("Godiste mora biti izmedju 1900 i 2010\n");
					continue;
				}
				else{
					cout << "\tUspjesno ste promijenili godiste u " << str << endl;
					korisnik.godiste[0] = '\0';
					strcat(korisnik.godiste, str.c_str());
					sacuvajKorisnika(korisnik);
					izbor = nadjiKorisnika(korisnik.username); // koristi se varijabla izbor da se sacuva memorija
					sviKorisnici[izbor].godiste[0] = '\0';
					strcat(sviKorisnici[izbor].godiste, korisnik.godiste);
					break;
				}
			}
			
			break;
		case 7: // izlista rentana vozila
			{
				system("CLS");
				naslov2;
				VOZILO v;
				int n = 0;
				TextTable t('-', '|', '+');
				t.add("ID");
				t.add("Proizvodjac");
				t.add("Model");
				t.add("Boja");
				t.add("Gorivo");
				t.add("Tip");
				t.add("Transmisija");
				t.add("Pogon");
				t.add("Godiste");
				t.add("Vrata");
				t.add("kW");
				t.add("Kilometraza");
				t.add("Sjedista");
				t.add("Kubikaza");
				t.add("Cijena");
				t.add("Klima");
				t.add("Tempomat");
				t.add("Navigacija");
				t.add("Istek");
				t.endOfRow();
				cout << setfill(' ') << " \n";
				for(int x = 0; x < MAX_VOZILA; x++){
					v = vozilo[x];
					if(!strcmp(v.rentao, korisnik.username) && v.rentano){
						n++;
						t.add(to_string(v.id));
						t.add(v.proizvodjac);
						t.add(v.model);
						t.add(v.boja);
						switch(v.gorivo){
							case DIZEL:
								t.add("Dizel");
								break;
							case BENZIN:
								t.add("Benzin");
								break;
							case PLIN:
								t.add("Plin");
								break;
							case ELEKTRICNI:
								t.add("Elek.");
							default: break;
						}
						switch(v.tipVozila){
							case LIMUZINA:
								t.add("Limuzina");
								break;
							case KARAVAN:
								t.add("Karavan");
								break;
							default: break;			
						}
						switch(v.transmisija){
							case MANUELNI:
								t.add("Manuelni");
								break;
							case AUTOMATIK:
								t.add("Automatik");
								break;
							case POLUAUTOMATIK:
								t.add("Poluautomatik");
								break;
							default: break;			
						}
						switch(v.pogon){
							case PREDNJI:
								t.add("Prednji");
								break;
							case ZADNJI:
								t.add("Zadnji");
								break;
							case SVACETIRI:
								t.add("4x4");
								break;
							default: break;
						}
						t.add(to_string(v.godiste));
						t.add(to_string(v.brojVrata));
						t.add(to_string(v.kilowati));
						t.add(to_string(v.kilometraza));
						t.add(to_string(v.sjedista));
						t.add(v.kubikaza);
						stringstream stream;
						stream << fixed << setprecision(2) << v.placeno;
						string sstr = stream.str() + " BAM";
						t.add(sstr);
						t.add(((v.klima) ? "Da" : "Ne"));
						t.add(((v.tempomat) ? "Da" : "Ne"));
						t.add(((v.navigacija) ? "Da" : "Ne"));
						stream.str("");
						time_t time = v.datumIsteka;
						tm *tm = localtime(&time);
						stream << tm->tm_mday << "." << tm->tm_mon << "." << tm->tm_year;
						sstr = stream.str();
						t.add(sstr);
					
						t.endOfRow();
					}
				}
				if(n == 0)
					cout << "\tNiste rentali nijedno vozilo!\n";
				else
					cout << t << "\n\n";
			}
			break;
		case 8: // vracanje na pocetnu
			return;
			break;
		default: break;
	}
	
	cout << nastavak; getch();
	
}

// rekurzivna funkcija za racunanje cijene rentanja (nakon 4. dana svaki naredni dan je 5% jeftiniji od normalne cijene)

double cijenaRentanja(double cijena, int brDana){
	if(brDana == 1){
		return cijena;
	}
	else if(brDana > 1 && brDana < 5){
		return cijena + cijenaRentanja(cijena, brDana - 1);
	}
	else if(brDana >= 5){
		return (cijena - cijena*0.05) + cijenaRentanja(cijena, brDana - 1);
	}
}

// provjera rentanih vozila
void provjeraRentova(){
	time_t now = time(0);
	for(int x = 0; x < MAX_VOZILA; x++){
		if(validID(x) && vozilo[x].rentano){
			if(now > vozilo[x].datumIsteka){
				
				stringstream stream;
				stream.str("");
				stream << "[RENT ISTEKAO] Rent istekao za vozilo " << vozilo[x].proizvodjac << " " << vozilo[x].model <<
						" koje je rentano od strane korisnika " << vozilo[x].rentao;
				logger(stream.str());
				
				vozilo[x].rentano = false;
				vozilo[x].rentao[0] = '\0';
				vozilo[x].brDana = 0;
				vozilo[x].placeno = 0;
				string str = "Vozila\\" + to_string(vozilo[x].id) + ".txt";
				remove(str.c_str());
				sacuvajVozilo(vozilo[x]);
			}
		}
	}
}


// funkcija koja ce spremati sva vaznija desavanja u log.txt fajl koji se nalazi u istom folderu kao i sam program
void logger(string str){
	ofstream dat;
	dat.open("log.txt", ios_base::app);
	time_t t = time(0);
	tm *local = localtime(&t);
	dat << "[" << local->tm_mday << "." << local->tm_mon+1 << "." << local->tm_year+1900 << "-"
		<< local->tm_hour << ":" << local->tm_min << "]";
	dat << str << endl;
	dat.close();
}


// ucitavanje i spremanje zarade u datoteku zarada.txt
double ucitajZaradu(){
	ifstream dat("zarada.txt");
	
	if(!dat){
		ofstream d("zarada.txt");
		d << 0 << endl;
		d.close();
	}
	
	double z;
	dat >> z;
	dat.close();
	return z;
}

void sacuvajZaradu(){
	ofstream dat("zarada.txt");
	dat << zarada << endl;	
	dat.close();
}



























