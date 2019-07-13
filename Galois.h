class Galois
{
	private:
	unsigned short* DivLUT;
	unsigned short* MulLUT;
	unsigned short* AddLUT;
	unsigned short* SubLUT;
	unsigned short* GField;

	public:
	int Dim, Base, Order, PrimEl;
	Galois (int, int);
	int Div (int, int);
	int Mul (int, int);
	int Add (int, int);
	int Sub (int, int);
	int GFi (int, int);
	~Galois ();
};
