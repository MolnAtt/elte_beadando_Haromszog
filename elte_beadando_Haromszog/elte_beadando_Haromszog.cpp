#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool between_cl(const int& a, const int& b, const int& c) { return (a <= b && b <= c) || (c <= b && b <= a); }
bool between_int(const int& a, const int& b, const int& c) { return (a < b && b < c) || (c < b && b < a); }


class Pont
{
public:
	int x, y;
	int ix;
	Pont(int a=0, int b=0, int i=-1)
	{
		x = a;
		y = b;
		ix = i;
	}



	/// <summary>
	/// QOP szög elõjelét adja meg.
	/// </summary>
	/// <param name="Q">a másik pont</param>
	/// <returns>QOP szög elõjele</returns>
	static int elojel(int S) { return S < 0 ? -1 : (S == 0 ? 0 : 1); }
	int irany(const Pont& Q) const { return elojel(y * Q.x - x * Q.y); }
	int irany(const Pont& B, const Pont& C) const { return (B - *this).irany(C - *this); }
	bool operator ==(const Pont& Q) const { return this->x == Q.x && this->y == Q.y; }
	bool operator !=(const Pont& Q) const { return !(*this == Q); }
	Pont operator -(const Pont& Q) const { return Pont(this->x - Q.x, this->y - Q.y); }
	int negyzeteshossz() const { return x * x + y * y; }
	int negyzetes_tavolsaga_ettol(const Pont& P) const { return (*this - P).negyzeteshossz(); }

	
	bool teglalapban_cl(const Pont& P, const Pont& Q) const { return between_cl(x, P.x, Q.x) && between_cl(y, P.y, Q.y); }
	bool teglalapban_int(const Pont& P, const Pont& Q) const { return between_int(x, P.x, Q.x) && between_int(y, P.y, Q.y); }
	bool is_in_cl(const Pont& P, const Pont& Q) const { return 0 == irany(P, Q) && teglalapban_cl(P, Q); }
	bool is_in_int(const Pont& P, const Pont& Q) const { return 0 == irany(P, Q) && teglalapban_int(P, Q); }
	
	bool is_in_int(const Pont& A, const Pont& B, const Pont& C) const
	{
		int merreforog = A.irany(B, *this);
		return merreforog == B.irany(C, *this) && merreforog == C.irany(A, *this);
	}

	bool is_in_cl(const Pont& A, const Pont& B, const Pont& C) const { return is_in_int(A, B, C) || is_in_cl(A, B) || is_in_cl(B, C) || is_in_cl(C, A); }

	
	~Pont(){}

	Pont balmeroleges() const { return Pont(-y, x); }
	Pont jobbmeroleges() const { return Pont(y, -x); }

	/// <summary>
	/// két ponton keresztül normálvektoros egyenlet három együtthatója egy vektorban.
	/// </summary>
	/// <param name="Q"></param>
	/// <returns></returns>
	vector<int> egyenes(const Pont& Q) const
	{
		/** /
		Pont iranyvektor = *this - Q;
		Pont normalvektor = iranyvektor.balra_forgat();
		/**/
		Pont N = (*this - Q).balmeroleges();
		return vector<int>{N.x, N.y, x* N.x + y * N.y};
	}
private:

};

class Ponthalmaz
{
public:

	int N;
	
	vector<Pont>* lista;
	
	Ponthalmaz()
	{
		cin >> N;
		lista = new vector<Pont>(N);
		for (int i = 0; i < N; i++)
		{
			int x, y;
			cin >> x >> y;
			lista->at(i)=Pont(x, y, i);
		}
	}

	/// <summary>
	/// Kiválaszt, feltéve, hogy biztosan van.
	/// </summary>
	/// <param name="predikatum">a kiválasztási feltétel</param>
	/// <returns>a kiválasztott pont</returns>
	Pont Find(bool (*predikatum)(Pont P))
	{
		int i = 0;
		while (!predikatum(lista->at(i))) { i++; }
		return lista->at(i);
	}

	vector<Pont> egy_haromszog()
	{
		Pont A = lista->at(0);
		int i = 1;
		while (lista->at(i) == A) { i++; }

		Pont B = lista->at(i);
		int j = 2;
		while (lista->at(j).is_in_cl(A, B)) { j++; }

		Pont C = lista->at(j);

		return vector<Pont>{A, B, C};
	}

	vector<Pont> egy_kisharomszog()
	{
		vector<Pont> H = egy_haromszog();
		
		// H0--H1 szakaszban H1-et közelítjük H0-hoz, ha van közéjük esõ pont

		for (int i = 0; i < N; i++)
			if (lista->at(i).is_in_int(H[0], H[1]))
				H[1] = lista->at(i);

		// H0--H1--H2 háromszögben H2-õt közelítjük H0--H1 szakaszhoz, ha van közéjük (háromszög lezártja kivéve a három pont) esõ pont 
		for (int i = 0; i < N; i++)
			if (lista->at(i).is_in_int(H[0], H[1], H[2]) || lista->at(i).is_in_int(H[0], H[2]) || lista->at(i).is_in_int(H[1], H[2]))
				H[2] = lista->at(i);

		return H;
	}


	Pont bal_also_sarokpont() const
	{
		Pont* best = &(lista->at(0));
		for (int i = 1; i < N; i++)
			if (lista->at(i).x < best->x || (lista->at(i).x == best->x && lista->at(i).y < best->y))
				best = &(lista->at(i));
		return *best;
	}

	/** /
	Ponthalmaz konvex_burok()
	{
		return;
	}
	/**/

	~Ponthalmaz(){}

private:

};


int main()
{
	Ponthalmaz H;
	vector<Pont> h = H.egy_kisharomszog();
	cout << 1+h[0].ix << " " << 1+h[1].ix << " " << 1+h[2].ix;
}

/** /
6
5 6 
7 5 
7 7 
4 5 
6 2 
2 3

/**/
