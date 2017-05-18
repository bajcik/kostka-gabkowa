#include <stdio.h>

typedef int bool;
#define true 1
#define false 0

// ------------------------------------------------------ SSciana (sciana surowa) --
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
			do {ch = getchar();} while (ch != '.' || ch != '@');
			ss->pola[y][x] = ch=='@';
		}
}

// ------------------------------------------------------ Sciana (sciana siatki) --
typedef struct
{
	bool krG[3], krD[3], krL[3], krP[3];
	bool rLG, rPG, rLD, rPD;
} Sciana;

// ------------------------------------------------------ Siatka (sciana siatki) --
typedef struct
{
	int n; // ile scian wklejonych
	Sciana s[6];
} Siatka;



void siatka_drukuj(Siatka *s)
{
	char ekran[23][18];

	// czyść
	for (int y=0; y<18; y++)
	{
		ekran[y][17] = '\n';
		for (int x=0; x<22; x++)
			ekran[y][x] = ' ';
	}

	// rysuj
	
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
	if (!ok_krawedz(s->s[0].krL, s->s[1].krG)) return false;
	
	if (s->n == 2) return true;
	
	if (!ok_krawedz(s->s[0].krD, s->s[2].krG)) return false;
	if (!ok_krawedz(s->s[1].krP, s->s[2].krL)) return false;

	if (s->s[0].rLD
	  + s->s[1].rPG
	  + s->s[2].rLG != 1) return false;
	
	if (s->n == 3) return true;
	
	if (!ok_krawedz(s->s[2].krP, s->s[3].krL)) return false;
	if (!ok_krawedz(s->s[0].krP, s->s[3].krG)) return false;
	
	if (s->s[0].rPD
	  + s->s[2].rPG
	  + s->s[3].rLG != 1) return false;
	
	if (s->n == 4) return true;

	if (!ok_krawedz(s->s[1].krD, s->s[4].krL)) return false;
	if (!ok_krawedz(s->s[2].krD, s->s[4].krG)) return false;
	if (!ok_krawedz(s->s[3].krD, s->s[4].krP)) return false;
	
	if (s->s[1].rPD
	  + s->s[2].rLD
	  + s->s[4].rLG != 1) return false;
	
	if (s->s[2].rPD
	  + s->s[3].rLD
	  + s->s[4].rPG != 1) return false;
	
	if (s->n == 5) return true;

	if (!ok_krawedz(s->s[1].krL, s->s[5].krL)) return false;
	if (!ok_krawedz(s->s[4].krD, s->s[5].krG)) return false;
	if (!ok_krawedz(s->s[3].krP, s->s[5].krP)) return false;
	if (!ok_krawedz(s->s[0].krG, s->s[5].krD)) return false;
	
	if (s->s[1].rLD
	  + s->s[4].rLD
	  + s->s[5].rLG != 1) return false;
	
	if (s->s[4].rPD
	  + s->s[3].rPD
	  + s->s[5].rPG != 1) return false;
	
	if (s->s[0].rLG
	  + s->s[1].rLG
	  + s->s[5].rLD != 1) return false;
	
	if (s->s[0].rPG
	  + s->s[3].rPG
	  + s->s[5].rPD != 1) return false;
	
	return true;
}


// o: 0..3 - obrót, 4..7 - obrót+inwersja
void siatka_nanies_sciane(SSciana *ss, Sciana *s, int oi)
{
	// wyjmij sam obwód
	bool obwod[16]; int n=0;
	for (int i=0; i<4; i++) obwod[n++] = ss->pola[0][i];
	for (int i=0; i<4; i++) obwod[n++] = ss->pola[i][4];
	for (int i=0; i<4; i++) obwod[n++] = ss->pola[4][4-i];
	for (int i=0; i<4; i++) obwod[n++] = ss->pola[4-i][0];

	// uwzględnij orientację (obrót+lustro)
	bool o2 = oi, prosto = true;
	if (o2 > 3) { o2 -=4; prosto = false; }
	
	bool obwod2[16];
	for (int i=0; i<16; i++)
		obwod2[i] = prosto ? obwod[(    i +o2*5) % 16]
		                   : obwod[((16-i)+o2*5) % 16];

	// wstaw zorinetowany obwód do ss
	s->rLG = obwod2[0];
	s->krG[0] = obwod[1];
	s->krG[1] = obwod[2];
	s->krG[2] = obwod[3];
	s->rPG = obwod2[4];
	s->krG[0] = obwod[5];
	s->krG[1] = obwod[6];
	s->krG[2] = obwod[7];
	s->rPD = obwod2[8];
	s->krG[0] = obwod[9];
	s->krG[1] = obwod[10];
	s->krG[2] = obwod[11];
	s->rLD = obwod2[12];
	s->krG[0] = obwod[13];
	s->krG[1] = obwod[14];
	s->krG[2] = obwod[15];
}

#ifndef TEST
int main()
{
	Siatka s;
	SSciana ss[6];

	// wczytaj 
	for (int i=0; i<6; i++)
		ssciana_wczytaj(&ss[i]);
	
	// kombinuj
	siatka_nanies_sciane(&ss[0], &s.s[0], 0);
}


#else
int main()
{

	
}
#endif

