#include <windows.h>
#include <iostream.h>

#include "Galois.h"
#include "BerleMas.h"

#define PrimPower(Pow) (GaFi->GFi(GaFi->PrimEl, ((GaFi->Dim-1)+((Pow)%(GaFi->Dim-1)))%(GaFi->Dim-1)))	//IDK will it work for non-simple Galois fields :(

char* EncodeByIDFT (short *Dest, short* Src, int Size, Galois* GaFi)
{
	int i, j, PoliVal;

	if (Size >= GaFi->Dim) return "Error: message does not fit in the DFT!";

	for (i=0; i<GaFi->Dim-1;i++)
	{
		for (PoliVal=j=0; j<Size; j++)	// (GaFi->Dim-1) - Size == trailing Zeros (ECC), so we don't mul by them
		{
			if (Src[j]>=GaFi->Dim) return "Error: message character does not fit in the Galois range!";
			PoliVal= GaFi->Add( PoliVal, GaFi->Mul(Src[j],PrimPower(j*i)) );
		}

		Dest[i]=PoliVal;
	}

	return NULL;
}

char* DecodeByDFT (short *Dest, short* Src, int Size, Galois* GaFi)
{
	int i, j, PoliVal;

	if (Size >= GaFi->Dim) return "Error: message does not fit in the DFT!";

	//Try to decode the Message

	for (i=0; i<GaFi->Dim-1;i++)
	{
		for (Dest[i]=j=0; j<GaFi->Dim-1; j++)
		{
			if (Src[j]>=GaFi->Dim) return "Error: message character does not fit in the Galois range!";
			Dest[i]=GaFi->Add( Dest[i], GaFi->Mul(Src[j],PrimPower(-j*i)) );
		}
		Dest[i]=GaFi->Div(Dest[i], GaFi->Dim-1);
	}

	//Check the semi-decoded Message

	for (i=Size; i<GaFi->Dim-1; i++)
		if (Dest[i])	//At least one error
			{
				short* Locator = (short*)malloc( ( ( GaFi->Dim - 1 - Size )/2 + 1) * sizeof (short) );
				int BadSymbols;
			//Give Error Syndrome to B.-M. and receive the Error Locator
				if (!BerleMassey (Locator, &BadSymbols, Dest+Size, GaFi->Dim-1-Size, GaFi) )
				{
					free (Locator);
					return "Too many errors!";
				}

			//Give Error Locator and the semi-decoded message to Forney and receive a pure decoded Error;
				short* Error   = (short*)malloc( (GaFi->Dim - 1) * sizeof (short) );
				if (!Forney (Dest, Size, Locator, BadSymbols+1, GaFi) )
				{
					free (Error);
					free (Locator);
					return "Software error!";	//this should not happen! Or can happen VERY rarely on some innocent-looking errors which tricked BerMas.
				}

			//Encode the Error, subtract it from the Message and have an error-free encoded Message (Error array re-used for it)
				EncodeByIDFT (Error, Dest, GaFi->Dim - 1, GaFi);
				for (j=0; j<GaFi->Dim-1; j++) Error[j] = GaFi->Sub(Src[j],Error[j]);

			//Decode it again, this time without errors :)
				DecodeByDFT (Dest, Error, Size, GaFi);	//Must NEVER go deeper than one level!!! If it does, something went very, very wrong.

				free (Error);
				free (Locator);
				return NULL;				
			}

	return NULL;
}

void main (void)
{
	int i;

	/*
	Galois Simple(7,1);	//Простое число. Длина сообщения "брутто" будет равна ему минус единица. Число контрольных символов -- по вкусу. Кодовый алфавит тоже не от балды, а равен этому числу.
	#define SynLen 4	//Длина синдрома ошибки (то есть числа контрольных символов, где мы приняли что-то не то)
	short ErrSyn[SynLen] = {2,1,0,5};	//Первый пример с Хабра -- error vector = 0,0,0,2,0,6 (3,5)
	Simple.PrimEl=5;
	// */
	
	/*
	Galois Simple(7,1);	//Простое число. Длина сообщения "брутто" будет равна ему минус единица. Число контрольных символов -- по вкусу. Кодовый алфавит тоже не от балды, а равен этому числу.
	#define SynLen 5	//Длина синдрома ошибки (то есть числа контрольных символов, где мы приняли что-то не то)
	short ErrSyn[SynLen] = {5,2,4,5,2};	//Второй пример с Хабра -- error vector = 1,0,0,0,1,0 (0,4)
	Simple.PrimEl=5;
	// */

	/*
	Galois Simple(11,1);	//Простое число. Длина сообщения "брутто" будет равна ему минус единица. Число контрольных символов -- по вкусу. Кодовый алфавит тоже не от балды, а равен этому числу.
	#define SynLen 7	//Длина синдрома ошибки (то есть числа контрольных символов, где мы приняли что-то не то)
	short ErrSyn[SynLen] = {5,8,4,7,4,5,8};	//Третий пример с Хабра -- error vector = 1,0,1,0,4,0,0,0,0,0 (0,2,4)
	Simple.PrimEl=6;	//forced
	// */

	/*

	short Locator[11];
//	cout<<"Berle-Real-Mess returned "<<BerleMess (SynLen, ErrSyn, &Simple)<<endl;
	cout<<"BerleMassey returned "<<BerleMassey (Locator, &i, ErrSyn, SynLen, &Simple)<<endl;

	/*/

	#define BRUTTO 12
//	#define BRUTTO 6

	Galois Simple(BRUTTO+1,1);
	short Test[]={3, 1};
	short SoloReed[BRUTTO];
	short Dec[BRUTTO];
	char* Error;

	Error = EncodeByIDFT (SoloReed, Test, sizeof(Test)/sizeof(short), &Simple);
	if (Error) {cout<<Error<<endl; cin>>i; return;}
for (i=0; i<BRUTTO; i++) cout<<SoloReed[i]<<" "; cout<<endl;


	SoloReed[3] = Simple.Add(SoloReed[3], 2);	//error!
	SoloReed[5] = Simple.Add(SoloReed[5], 6);	//error!


	SoloReed[0] = Simple.Add(SoloReed[0], 1);	//error!
	SoloReed[1] = Simple.Add(SoloReed[1], 6);	//error!

	SoloReed[8] = Simple.Add(SoloReed[8], 8);	//error!

for (i=0; i<BRUTTO; i++) cout<<SoloReed[i]<<" "; cout<<endl;


	Error = DecodeByDFT  (Dec, SoloReed , sizeof(Test)/sizeof(short), &Simple);
	if (Error) {cout<<Error<<endl; cin>>i; return;}
for (i=0; i<BRUTTO; i++) cout<<Dec[i]<<" "; cout<<endl;

	// */

	cin>>i;
}