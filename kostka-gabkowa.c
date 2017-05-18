#include <stdio.h>

typedef int bool;
#define true 1
#define false 0

// ------------------------------------------------------ SSciana (sciana surowa) --
/*
     +--+--+--+--+--+
	 |  |  |  |  |  |
     +--+--+--+--+--+
	 |  |  |  |  |  |
     +--+--+--+--+--+
	 |  |  |  |  |  |
     +--+--+--+--+--+
	 |  |  |  |  |  |
     +--+--+--+--+--+
	 |  |  |  |  |  |
     +--+--+--+--+--+
*/
typedef struct 
{
	bool pola[5][5];
} SSciana;

void ssciana_wczytaj(SSciana *ss)
{
	for (int y=0; y<5; y++)
		for (int x=0; x<5; x++)
		{
			char ch;
			do {ch = getchar();} while (ch != '.' && ch != '@');
			ss->pola[y][x] = ch=='@';
		}
}

// ------------------------------------------------------ Sciana (sciana siatki) --
/*
     +---+--------+---+
	 |rLG|  >krG> |rPG|
     +---+--------+---+
	 |   |        |   |
	 | A |        | V |
	 |krL|        |krP|
	 | A |        | V |
	 |   |        |   |
     +---+--------+---+
	 |rLD|  <krD< |rPD|
     +---+--------+---+
*/
typedef struct
{
	bool krG[3], krD[3], krL[3], krP[3]; // krawędzie
	bool rLG, rPG, rLD, rPD;             // rogi
} Sciana;

// o: 0..3 - obrót, 4..7 - obrót+inwersja
void sciana_nanies_surowa(Sciana *s, SSciana *ss, int oi)
{
	// wyjmij sam obwód
	bool obwod[16]; int n=0;
	for (int i=0; i<4; i++) obwod[n++] = ss->pola[0][i];
	for (int i=0; i<4; i++) obwod[n++] = ss->pola[i][4];
	for (int i=0; i<4; i++) obwod[n++] = ss->pola[4][4-i];
	for (int i=0; i<4; i++) obwod[n++] = ss->pola[4-i][0];

	// uwzględnij orientację (obrót+lustro)
	bool o2 = oi, prosto = true;
	if (o2 >= 4) { o2 -= 4; prosto = false; }
	
	bool obwod2[16];
	for (int i=0; i<16; i++)
		obwod2[i] = prosto ? obwod[(    i +o2*4) % 16]
		                   : obwod[((16-i)+o2*4) % 16];

	// wstaw zorinetowany obwód do ss
	s->rLG = obwod2[0];
	s->krG[0] = obwod2[1];
	s->krG[1] = obwod2[2];
	s->krG[2] = obwod2[3];
	s->rPG = obwod2[4];
	s->krP[0] = obwod2[5];
	s->krP[1] = obwod2[6];
	s->krP[2] = obwod2[7];
	s->rPD = obwod2[8];
	s->krD[0] = obwod2[9];
	s->krD[1] = obwod2[10];
	s->krD[2] = obwod2[11];
	s->rLD = obwod2[12];
	s->krL[0] = obwod2[13];
	s->krL[1] = obwod2[14];
	s->krL[2] = obwod2[15];
}

// ------------------------------------------------------ Siatka (sciana siatki) --
/*
       +---+ 
       | 0 |
   +---+---+---+
   | 2 | 1 | 3 |
   +---+---+---+
       | 4 |
       +---+ 
       | 5 |
       +---+ 
*/

typedef struct
{
	int n; // ile scian wklejonych
	Sciana s[6];
} Siatka;



void siatka_drukuj(Siatka *s)
{
	char ekran[23][18];

	// czyść
	for (int y=0; y<23; y++)
	{
		for (int x=0; x<18; x++)
			ekran[y][x] = ' ';
		ekran[y][17] = '\n';
	}

	// rysuj
	int dx[] = {6, 0, 6, 12, 6, 6};
	int dy[] = {0, 6, 6, 6, 12, 18};

	for (int ns=0; ns<s->n; ns++)
	{
#		define PIX(y,x,v) ekran[dy[ns] + y][dx[ns] + x] = v ? '@':'.'
		
		PIX(0, 0, s->s[ns].rLG);
		PIX(0, 4, s->s[ns].rPG);
		PIX(4, 0, s->s[ns].rLD);
		PIX(4, 4, s->s[ns].rPD);

		for (int i=0; i<3; i++)
		{
			PIX(0, 1+i, s->s[ns].krG[i]);
			PIX(4, 3-i, s->s[ns].krD[i]);
			PIX(3-i, 0, s->s[ns].krL[i]);
			PIX(1+i, 4, s->s[ns].krP[i]);
		}

		for (int i=1; i<=3; i++)
			for (int j=1; j<=3; j++)
				PIX(i, j, true);
	}
	
	// wyświetl
	puts((char*)(void*)ekran);
}

bool ok_krawedz(bool A[], bool B[])
{
	return A[0] == B[2] && A[1] == B[1] && A[2] == B[1];
}

#define OKR(s1,k1,s2,k2) if (!ok_krawedz(s->s[s1].kr ## k1, s->s[s2].kr ## k2)) return false;
bool siatka_sprawdz(Siatka *s)
{
	if (s->n == 1) return true;

	if (!ok_krawedz(s->s[0].krD, s->s[1].krG)) return false;
	
	if (s->n == 2) return true;
	
	if (!ok_krawedz(s->s[0].krL, s->s[2].krG)) return false;
	if (!ok_krawedz(s->s[1].krL, s->s[2].krP)) return false;

	if (s->s[0].rLD
	  + s->s[1].rLG
	  + s->s[2].rPG != 1) return false;
	
	if (s->n == 3) return true;
	
	if (!ok_krawedz(s->s[0].krP, s->s[3].krG)) return false;
	if (!ok_krawedz(s->s[1].krP, s->s[3].krL)) return false;
	
	if (s->s[0].rPD
	  + s->s[1].rPG
	  + s->s[3].rLG != 1) return false;
	
	if (s->n == 4) return true;

	if (!ok_krawedz(s->s[1].krD, s->s[4].krG)) return false;
	if (!ok_krawedz(s->s[2].krD, s->s[4].krL)) return false;
	if (!ok_krawedz(s->s[3].krD, s->s[4].krP)) return false;
	
	if (s->s[1].rLD
	  + s->s[2].rPD
	  + s->s[4].rLG != 1) return false;
	
	if (s->s[2].rPD
	  + s->s[3].rLD
	  + s->s[4].rPG != 1) return false;
	
	if (s->n == 5) return true;

	if (!ok_krawedz(s->s[2].krL, s->s[5].krL)) return false;
	if (!ok_krawedz(s->s[4].krD, s->s[5].krG)) return false;
	if (!ok_krawedz(s->s[3].krP, s->s[5].krP)) return false;
	if (!ok_krawedz(s->s[0].krG, s->s[5].krD)) return false;
	
	if (s->s[2].rLD
	  + s->s[4].rLD
	  + s->s[5].rLG != 1) return false;
	
	if (s->s[4].rPD
	  + s->s[3].rPD
	  + s->s[5].rPG != 1) return false;
	
	if (s->s[0].rLG
	  + s->s[2].rLG
	  + s->s[5].rLD != 1) return false;
	
	if (s->s[0].rPG
	  + s->s[3].rPG
	  + s->s[5].rPD != 1) return false;
	
	return true;
}


// --------------------------------------------------------------------------------
//   s: siatka
//  ss: lista wskaźników surowych ścianek do wykorzystania
// ssn: długość listy ścianek
void zbadaj_poziom(Siatka *s, SSciana **ss, int ssn)
{
	s->n++; printf("%d", s->n);

	for (int is=0; is<ssn; is++)  // wszystkie ściany
	{
		// druga lista wskaźników ścian - bez is'owego
		SSciana *ss2[6];
		int ssn2 = 0;
		for (int i=0; i<ssn; i++)
			if (i != is)
				ss2[ssn2++] = ss[i];

		for (int io=0; io<8; io++) // wszystkie orientacje
		{
			sciana_nanies_surowa(&s->s[s->n], ss[is], io);
			if (siatka_sprawdz(s))
			{
				if (s->n == 6) siatka_drukuj(s);
				else           zbadaj_poziom(s, ss2, ssn-1);
			}
		}
	}
	s->n--;
}


#ifndef TEST
int main()
{
	Siatka s;
	SSciana surowa[6];

	// wczytaj 
	for (int i=0; i<6; i++)
		ssciana_wczytaj(&surowa[i]);
	
	// pierwsza na stałe
	sciana_nanies_surowa(&s.s[0], &surowa[0], 0);
	s.n = 1;

	// kombinuj
	SSciana *ss[5] = {&surowa[1], &surowa[2], &surowa[3], &surowa[4], &surowa[5]};
	zbadaj_poziom(&s, ss, 5);
}


#else
#include <stdlib.h> // atoi()

void test_wczytaj_orientuj_printuj(int oi)
{
	Siatka s;

	for (int i=0; i<6; i++)
	{
		SSciana ss;
		
		ssciana_wczytaj(&ss);
		sciana_nanies_surowa(&s.s[i], &ss, oi);
	}
	s.n = 6;

	siatka_drukuj(&s);
}

int main(int argc, char **argv)
{
	switch (atoi(argv[1]))
	{
		case 0: test_wczytaj_orientuj_printuj(atoi(argv[2])); break;
	}
	return 0;
}
#endif

















