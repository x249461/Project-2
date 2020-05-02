#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>

#define INFINITY 2147483647; //największy możliwy int

using namespace std;


//Typy danych
struct ElementListy
{
  ElementListy *next; //następny element listy
  int docelowy, wagaKrawedzi;  //numer węzła docelowego i waga krawędzi
};

/*
class ElementListy {
public:
	int docelowy, wagaKrawedzi; //wierzchołek poczatkowy i waga krawędzi
	ElementListy* next;
};

*/

/*******************************************/
/*  Deklaracja klasy grafMacierz           */
/*******************************************/
class grafMacierz
{
public:
  int n; //rozmiar
  int **macierz;
  grafMacierz(int iloscWierzcholkow)
  {
    n = iloscWierzcholkow;
    macierz = new int * [n];
    for(int i=0; i < n; i++)
      {
	    macierz[i]=new int[n];
	        for(int j=0; j < n; j++)
	         {
	             macierz[i][j] = -1;
	         }
      }
  };
  int* &operator[](int i) {return macierz[i];}
};




/*******************************************/
/*  Deklaracja klasy grafLista             */
/*******************************************/
class grafLista
{
public:
  int n; //rozmiar
  ElementListy **listaSasiedztwa; // Tablica list sąsiedztwa
  grafLista(int iloscWierzcholkow)
  {
    n = iloscWierzcholkow;
    listaSasiedztwa = new ElementListy * [n];

    for(int i=0; i < n; i++)
    {
        listaSasiedztwa[i] = NULL;
    }
  };
  ElementListy*  &operator[](int i) {return listaSasiedztwa[i];};
};




/***************************************************/
/*  Funkcja wyświetlająca w programie              */
/*  ciąg wierzchołków czyli drogę oraz jej koszt   */
/***************************************************/

void wyswietldroge(int liczbaWierzcholkow, int poprzednicy[], int koszty[])
{
  int* Stos= new int[liczbaWierzcholkow];
  int wierz = 0;
  int i = 0;
  for(i = 0; i < liczbaWierzcholkow; i++)
    {
      cout << i << ": ";

      for(int w = i; w > -1; w = poprzednicy[w])
	Stos[wierz++] = w;
      //wyswietl przebyte wierzcholki
      while(wierz)
	cout << Stos[--wierz] << " ";
      //wyswietl koszt
      int inf=INFINITY;
       if(koszty[i] == inf)
	 cout << "brak mozliwej sciezki (graf nie jest spojny)"<<endl;
	else
	  cout << "koszt:" << koszty[i] << endl;
    }
}



/*******************************************/
/* Funkcja zapisująca drogę do pliku .txt  */
/*******************************************/
void zapiszdoplikudroge(int liczbaWierzcholkow, int poprzednicy[], int koszty[], string nplikudocel)
{

  int* Stos= new int[liczbaWierzcholkow];
  int wierz=0;
  ofstream plikdoc(nplikudocel);
 for(int i = 0; i < liczbaWierzcholkow; i++)
    {
      plikdoc << i << ": ";

      for(int w = i; w > -1; w = poprzednicy[w])
	Stos[wierz++] = w;
      //wyswietl przebyte wierzcholki
      while(wierz)
	plikdoc << Stos[--wierz] << " ";
      //wyswietl koszt
      int inf = INFINITY;
      if (koszty[i] == inf)
	plikdoc << "brak mozliwej sciezki (graf nie jest spojny)"<<endl;
	else
	  plikdoc << "koszt:" << koszty[i] << endl;
    }
  plikdoc.close();

}



/*******************************************/
/*  Algorytm Dijikstry z wykorzystaniem    */
/*       listy sąsiedztwa                  */
/*******************************************/
void dijkstraLista(grafLista graf, int wierzstart, string nplikudocel)
  {
    int i, j, u;
  //Tworzenie tablic dynamicznych
  int *koszty = new int [graf.n];   //Tablica kosztów dojścia
  int *poprzednicy = new int [graf.n];   //Tablica poprzedników
  bool *QS= new bool [graf.n];  //Zbiory Q iS

  //inicjalizacja tablic dynamicznych
   for (int i = 0; i < graf.n; i++)
    {
      koszty[i] = INFINITY;
      poprzednicy[i] = -1;
      QS[i] = false;
    }

  koszty[wierzstart] = 0;
  //Wyznaczam drogę dojścia
  for(i = 0; i < graf.n; i++)
    {
        //Szukam wierzchołka w Q o najmniejszym koszcie
      for(j = 0; QS[j]; j++);
      for(u = j++; j < graf.n; j++)
	if( !QS[j] && (koszty[j] < koszty[u])) u = j;
        //Znaleziony wierzchołek przenoszę do S
      QS[u] = true;

        //Modyfikuję odpowiednio wszystkich sąsiadów u, którzy są w Q
  ElementListy *krawedz;
  krawedz= new ElementListy;
  for(krawedz = graf[u]; krawedz; krawedz = krawedz -> next)
    if( !QS[krawedz -> docelowy] && (koszty[krawedz -> docelowy] > koszty[u] + krawedz -> wagaKrawedzi))
      {
	koszty[krawedz -> docelowy] = koszty[u] + krawedz -> wagaKrawedzi;
	poprzednicy [krawedz -> docelowy] = u;
      }
    }
  //wyswietldroge(graf.n, poprzednicy, koszty);
  zapiszdoplikudroge(graf.n, poprzednicy, koszty,nplikudocel);
/*
delete [] koszty;
delete [] poprzednicy;
delete [] QS;*/
    }




/*******************************************/
/*  Algorytm Dijikstry z wykorzystaniem    */
/*       macierzy sąsiedztwa               */
/*******************************************/
void dijkstraMacierz(grafMacierz graf, int wierzstart, string nplikudocel)
{
  int i, j ,u, m, h;
  //Tworzenie tablic dynamicznych
  int *koszty = new int [graf.n];
  int *poprzednicy = new int [graf.n];
  bool *QS= new bool [graf.n];
  int *Stos= new int[graf.n];
  int wierz=0;


   for (int i=0; i < graf.n; i++)
    {
      koszty[i] = INFINITY;
      poprzednicy[i] = -1;
      QS[i] = false;
    }

  koszty[wierzstart] = 0;

  for(i = 0; i < graf.n; i++)
    {
      for(j = 0; QS[j]; j++);
      for(u = j++; j < graf.n; j++)
	if( !QS[j] && (koszty[j] < koszty[u]))
	  u = j;
      QS[u] = true;



  h = 0;
  //tworzenie tablicy wszystkich sasiadow wierzcholka u
  int* sasiedzi=new int[graf.n];

  for(int m = 0; m < graf.n; m++)
    {
      if(graf[u][m] > -1)
	{
	  sasiedzi[h] = m;
	  h++;
	}
    }
  h--;
  for(h; h >= 0; h--)
    if( !QS[sasiedzi[h]] && (koszty[sasiedzi[h]] > koszty[u] + graf[u][sasiedzi[h]]))
      {
	koszty[sasiedzi[h]] = koszty[u] + graf[u][sasiedzi[h]];
	poprzednicy[sasiedzi[h]] = u;
      }
    }
  //wyswietldroge(graf.n, poprzednicy, koszty);
  zapiszdoplikudroge(graf.n, poprzednicy, koszty,nplikudocel);
/* delete [] koszty;
  delete [] poprzednicy;
  delete [] QS;
  delete [] Stos;*/
    }


void tworzeniePlikowZGrafami(int liczbaWierzch, double gestoscGrafu, int liczbaGrafow)
{
  int liczbaKrawedzi = 2 * liczbaWierzch * (liczbaWierzch - 1) * gestoscGrafu;
  int wierzcholekStartowyAlgorytmu = 2;
  for (int z = 0; z < liczbaGrafow; z++)
    {
      int** istniejace = new int* [liczbaWierzch];
      for(int i = 0; i < liczbaWierzch; i++)
	{
	  istniejace[i] = new int[liczbaWierzch];
	    for(int j = 0; j < liczbaWierzch; j++)
	     {
	        istniejace[i][j] = 0;
	    }
	}
      ostringstream ss;
      ss << z;
      string numer = ss.str();
      string nazwaPliku="graf"+numer+".txt";
      ofstream plik(nazwaPliku);

      plik << liczbaKrawedzi <<" "<< liczbaWierzch <<" "<< wierzcholekStartowyAlgorytmu << endl;

      int wierzcholekStartowy = 0;
      for(int kraw = 0; kraw < liczbaKrawedzi; kraw++)
	{
	  wierzcholekStartowy = wierzcholekStartowy % liczbaWierzch;
	  int wierzcholekKoncowy=rand() % liczbaWierzch;
	  int flag = 0;
	  if(wierzcholekStartowy == wierzcholekKoncowy)
	    flag = -1;
	  if(istniejace[wierzcholekStartowy][wierzcholekKoncowy] == 1)
	    {
            flag = -1;
	    }
	  while (wierzcholekKoncowy == wierzcholekStartowy || flag == -1)
	    {
	      wierzcholekKoncowy = (wierzcholekKoncowy+1) % liczbaWierzch;
	    if((istniejace[wierzcholekStartowy][wierzcholekKoncowy] == 0) && wierzcholekStartowy != wierzcholekKoncowy)
	     {
	       flag = 0;
	     }
	    }

	  istniejace[wierzcholekStartowy][wierzcholekKoncowy]=1;

	  int waga = rand() % 10 + 1;
	  plik << wierzcholekStartowy <<" "<< wierzcholekKoncowy <<" "<< waga << endl;
	  wierzcholekStartowy++;
	}
	  delete istniejace;
      plik.close();
    }
}





/*******************************************/
/*                                         */
/*    Program główny                       */
/*                                         */
/*******************************************/
int main()
{
  int liczbaGrafow = 100;

  int liczbaWierzcholkow; // = 10;   //DEKLARACJA LICZBY WIERZCHOŁKÓW DLA PROGRAMU
  double gestoscGrafu;// = 0.75;    //DEKLARACJA GĘSTOŚCI GRAFU

  char zakonczDzialanie;
  cout<<"___________PROGRAM WYZNACZAJACY NAJKROTSZA SCIEZKE NA PODSTAWIE ALGORYTMU DIJIKSTRY____________"<<endl;
  cout<<" Program moze wygenerowac duza ilosc plikow z grafami w swoim katalogu!"<<endl<< "Jesli sie NIE ZGADZASZ, nacisnij   n" <<endl
   <<"Jesli sie ZGADZASZ, nacisnij   t "<<endl;
    cin>>zakonczDzialanie;
  if(zakonczDzialanie == 'n'){
    cout<<"Program konczy dzialanie......"<<endl<<endl<<endl;
    return -2;
    }

  if(zakonczDzialanie == 't'){
      cout << "Wprowadz liczbe wierzcholkow:" <<endl;
      cin >> liczbaWierzcholkow;
      cout << "Wprowadz gestosc grafu:"<<endl;
      cin >> gestoscGrafu;

  cout<<"Program automatycznie tworzy 100 grafow o podanych wczesniej parametrach."<<endl<<
  "Efektem dzialania programu sa sciezki znalezione przy pomocy algorytmu Dijikstry zapisywane do pliku o nazwie droga[nrgrafu].txt"<<endl<<endl;
  cout<<"_______________________"<<endl;
  cout<<" Generowanie grafow...."<<endl;
  cout<<"_______________________"<<endl<<endl;
  tworzeniePlikowZGrafami(liczbaWierzcholkow, gestoscGrafu, liczbaGrafow);
  cout<<"Grafy pomyslnie wygenerowano."<<endl;
  cout<<"_______________________"<<endl<<endl;
  int start, koniec, weight;
  double czasyWykonywania[100];
  cout.precision(8);
  char listaCzyMacierz;
  cout<<"Wybierz sposob reprezentacji badanych grafow:"<<endl<<" l ----> lista sasiedztwa "<<endl<<" m ----> macierz sasiedztwa"<<endl;
  cin>>listaCzyMacierz;
  cout<<endl<<"_______________________"<<endl;
  cout<<"Algorytm pracuje nad znalezieniem sciezek we wszystkich grafach...."<<endl;
  cout<<"_______________________"<<endl<<endl;
  int ilosc_krawedzi, ilosc_wierzcholkow, wierzcholek_startowy;
   for (int ktorygraf = 0; ktorygraf < liczbaGrafow; ktorygraf++)
     {
    ostringstream ss;
    ss << ktorygraf;
    string numer = ss.str();
    string nazwaPliku="graf"+numer+".txt";
    string nplikudocel="droga"+numer+".txt";
    fstream plik;
    plik.open(nazwaPliku);

  if(plik.good())
    {
      plik >> ilosc_krawedzi;
      plik >> ilosc_wierzcholkow;
      plik >> wierzcholek_startowy;
   switch (listaCzyMacierz)
    {
    case 'l':
      {
	grafLista graf(ilosc_wierzcholkow);
	for(int i = 0; i < ilosc_krawedzi; i++)
	  {
	    plik >> start >> koniec >> weight;
	    ElementListy *krawedz;
	    krawedz= new ElementListy;  //tworzymy element listy sąsiedztwa
	    krawedz -> docelowy = koniec;  //wierzchołek docelowy krawędzi
	    krawedz -> wagaKrawedzi = weight; //waga krawędzi
	    krawedz -> next = graf[start];
	    graf[start] = krawedz;    //element dołączamy do listy
	  }
	/*** START POMIARU CZASU ***/
	double roznica;
	cout.setf(ios::fixed); //notacja zwykla
	clock_t start, koniec; //inicjacja zmiennych zegarowych
	start = clock(); //zapisanie czasu startu mierzenia

	dijkstraLista(graf,wierzcholek_startowy, nplikudocel);

	/*** KONIEC POMIARU CZASU ***/
	koniec = clock();  //zapisanie konca mierzenia czasu
	roznica = (double)(koniec-start)/CLOCKS_PER_SEC;
	czasyWykonywania[ktorygraf] = roznica;

	break;
      }
    case 'm':
      {
	grafMacierz graf(ilosc_wierzcholkow);
	for(int i = 0; i < ilosc_krawedzi; i++)
	  {
	    plik >> start >> koniec >> weight;
	    graf[start][koniec] = weight;
	  }
	//POMIARCZASU_START//
	double roznica;
	cout.setf(ios::fixed); //notacja zwykla
	clock_t start, koniec; //inicjacja zmiennych zegarowych
	start = clock(); //zapisanie czasu startu mierzenia

	dijkstraMacierz(graf, wierzcholek_startowy, nplikudocel);

	//POMIARCZASU_KONIEC//
	koniec = clock();//zapisanie konca mierzenia
	roznica = (double)(koniec - start) / CLOCKS_PER_SEC;
	czasyWykonywania[ktorygraf] = roznica;
      break;
      }
    }
   plik.close();
    }
   }
   double czasCalosci = 0;
   for(int count = 0; count < 100; count++)
    czasCalosci += czasyWykonywania[count];
   double czasSredni = czasCalosci / 100;
   cout<<"____WYNIKI DZIALANIA PROGRAMU:____"<<endl;
   cout<<"Liczba wierzcholkow: "<<liczbaWierzcholkow<<endl<<"Gestosc grafu: "<<gestoscGrafu<<endl<<"Sredni czas: "<<czasSredni<<endl<<endl<<endl;
  return 0;
}

}
