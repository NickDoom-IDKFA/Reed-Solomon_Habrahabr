#include <windows.h>
#include <iostream.h>

#include "Galois.h"
#include "BerleMas.h"


int MulByToeplitz (int x, int* Gamma, short *Syndrome, int SyndLength, int Size, Galois* GF)	//VERBOTEN!!! �����襭�� � ��।�� ��ꥪ⮢ �� �������. ��᫥���� ��ࠬ��� ���� ⮫쪮 �� ᡮથ ��� ���殢 ����� ��������஬.
{
	int y, Val;
	if (x+Size>SyndLength) return -1;	//����।��񭭮� (� �� �㦭��, ����) ���祭��.
	Val=0;	//���樠�����㥬 �⮫��� १����;
	for (y=0;y<Size;y++) Val=GF->Add( Val, GF->Mul(Gamma[y],Syndrome[x+Size-y-1]) );	//������塞 � ���� �⮫��� �������� �, ���������� �� ᢮� ������ ��ப ������ �񯫨� (�ࠧ� �� �� � ࠧ����).
	return Val;
}


#define MaxSynd 1024	//ToDo!!!

//Mess -- � ��᫥ ���, ����� �� ��ࠡ�⠫�
BOOL BerleMess (int SyndLen, short* Syndrome, Galois* GF)	//VERBOTEN!!! �����襭�� � ��।�� ��ꥪ⮢ �� �������. ��᫥���� ��ࠬ��� ���� ⮫쪮 �� ᡮથ ��� ���殢 ����� ��������஬.
{
	int i,j,k;

	int G[MaxSynd]={1};	//���ᨬ��쭠� ����� �������� �, ������ ����� ��� ���-� ����� 㬭����� �� ������ �񯫨� � ᨭ�஬�� �訡�� -- �� ��� ࠧ ࠧ��� ᨭ�஬�, ஢�� ���� ���祭�� � �㤥� ��।����.
	int S=1;	//���� � ����� 1 (� �񯫨� 1�1, ᮮ⢥��⢥���).
	int N=0;	//��᪮�쪮 㦥 �ந��஢����� ����� ��������.
	static struct
	{
		int Size;	//����� �������� �᫨⥫�;
		G[MaxSynd];	//������� �᫨⥫� (���, �� ᠬ� ������� � � �㦭�� �⠤��);
		int D;		//������� �������⥫� (�㦭� ����� ��������)
	} History[MaxSynd];	//��� �� ���ਫ, �� ᪮�쪮 室�� ������ �����蠥��� ⥬ ��� ��� �ᯥ宬 :( �� � ����� ���� �த�����, �᫨ ��⭮ :( ����� �㤥� � ����ᮬ - ����� ����� ᨭ�஬� �筮 ���� �� �㤥�.
	int Temp[MaxSynd];
	int E;			//�⤥��� ����� �������� (�㯮 ��⨬�����, �⮡� �� ��ॢ������ �� �� ࠧ)

	while (S<=SyndLen)	//�த� �� ⠪, �������. �᫨ ��諮 �� �।��� - �����, �襭�� �� ������� (�訡�� ᫨誮� �����)
	{
for (i=0; i<S; i++) cout<<G[i]<<" ";
cout<<" X [T] = ";
for (i=0; i<S; i++) cout<<MulByToeplitz(i,G,Syndrome,SyndLen,S,GF)<<" ";	//������ ��������;
cout<<endl;
	
		for (E=i=0;i<N;i++) if (E=MulByToeplitz(i,G,Syndrome,SyndLen,S,GF))	//�᫨ �������� ᮤ�ন� ���㫥�� ������ ᫥�� �� �������� � �������⥫� -- 㡥�� �� �� ������, � �⮬ ��� ������� ࠭�� ����⠭�� ����� � �����樥��� �� ���ਨ.
		{
			if (E<0) return FALSE;// break;	//��諨 �� �।��� �������� ������;

			memset (Temp, 0, sizeof (Temp));
			memcpy (Temp+(S-History[i].Size), History[i].G, History[i].Size*sizeof(int));
cout<<"Subtracting "<<E<<" / "<<History[i].D<<" [ ";					//�� ᠬ� ��५�窨 �१ ��� ��࠭��� �� 堡஢᪮�� ���㠫�
for (k=0;k<S;k++) cout<<Temp[k]<<" ";
cout<<"]=";
			for (k=0;k<S;k++) Temp[k]=GF->Div( GF->Mul(Temp[k],E) , History[i].D );
for (k=0;k<S;k++) cout<<Temp[k]<<" ";
cout<<endl;
			for (k=0;k<S;k++) G[k]=GF->Sub(G[k],Temp[k]);

for (k=0; k<S; k++) cout<<G[k]<<" ";
cout<<" X [T] = ";
for (k=0; k<S; k++) cout<<MulByToeplitz(k,G,Syndrome,SyndLen,S,GF)<<" ";	//������ ��������;
cout<<endl;
		}
	
		if (E<0 || MulByToeplitz(N,G,Syndrome,SyndLen,S,GF)<0)	//�� �㫨, � ��⮬ ����� ���稫��� %)
		{
			if (i<(SyndLen+1)/2) {S--; continue;}
			else break;	//Success!!!
		}

		if (MulByToeplitz(N,G,Syndrome,SyndLen,S,GF))		//�� �㫨, � ��।��� -- �� ����. �������� ���, �� �������.
		{
			History[N].D=MulByToeplitz(N,G,Syndrome,SyndLen,S,GF);
			memcpy (History[N].G, G, S*sizeof(int));
			History[N].Size=S;
cout<<"For "<<N<<" remembered: ";
for (i=0; i<S; i++) cout<<History[N].G[i]<<" ";
cout<<"/ ";
cout<<History[N].D<<endl;
			N++;
		} else if (N==SyndLen/2) break;	//Success!!!
	
		G[S]=0;	//�����ᨬ ��� ������, �����⨬ ���� �=0 � ����ਬ ����祭��� ���������.
		S++;
	}
if (S==SyndLen) return FALSE;	//Not sure yet

	if (S<=SyndLen)
	{
		for (i=0;i<GF->Dim-1;i++)
		{
			int x,v;
			for (x=1,j=0; j<i; j++) x=GF->Mul(x,GF->PrimEl);
cout<<"X="<<GF->PrimEl<<"^"<<i<<"="<<x<<endl;
			for (v=k=0,j=1; k<SyndLen/2+1; k++,j=GF->Mul(j,x)) v=GF->Add( v,GF->Mul(j,G[k]) );
			if (!v) cout<<i<<" -- Error!"<<endl;
		}
	}
	
	return TRUE;	//Success!
}

//������� � ���-����
//��૥���� � ����
//���� ���
//��㣮� ����
//��� � � ����������� :(

BOOL BerleMassey (short* Locator, int* NErrs, short* Syndrome, int SyndLen, Galois* GF)
{
	int *s, *C, *B, *T;
	int L=0, m=1, b=1, n, d;

	s=(int*)malloc(SyndLen*sizeof(int));
	C=(int*)malloc(SyndLen*sizeof(int));
	B=(int*)malloc(SyndLen*sizeof(int));
	T=(int*)malloc(SyndLen*sizeof(int));
	memset (s, 0, SyndLen*sizeof(int));
	memset (C, 0, SyndLen*sizeof(int));
	memset (B, 0, SyndLen*sizeof(int));
	C[0]=1;
	B[0]=1;

	for (n=0; n<SyndLen; n++)
	{
		d=Syndrome[n];
		for (int i=n-1,j=1; i>=n-L; i--,j++) d=GF->Add(d, GF->Mul(Syndrome[i],C[j]) );

		if (!d)
		{
			m++;
			continue;
		}

		if (n < 2*L)
		{
			for (int i=m; i<SyndLen /*todo -- optimise*/; i++) C[i] = GF->Sub(   C[i],   GF->Mul( GF->Div(d,b) , B[i-m] )   );
			m++;
			continue;
		}
		
		memcpy (T, C, SyndLen*sizeof(int)); //ToDo -- optimise
		for (int i=m; i<SyndLen /*todo -- optimise*/; i++) C[i] = GF->Sub(   C[i],   GF->Mul( GF->Div(d,b) , B[i-m] )   );
		L = n + 1 - L;
		memcpy (B, T, SyndLen*sizeof(int)); //ToDo -- optimise
		b = d;
		m = 1;
	}

cout<<L<<" errors found; Locator = ";
for (int i=0;i<SyndLen;i++) cout<<C[i]<<" "; cout<<endl;

if (L > SyndLen/2) return FALSE;	//Unrecoverable?

	b=0;
	for (int i=0;i<GF->Dim-1;i++)
	{
		int x,v,j,k;
		for (x=1,j=0; j<i; j++) x=GF->Mul(x,GF->PrimEl);
cout<<"X="<<GF->PrimEl<<"^"<<i<<"="<<x<<endl;
		for (v=k=0,j=1; k<L+1; k++,j=GF->Mul(j,x)) v=GF->Add( v,GF->Mul(j,C[k]) );
if (!v) cout<<i<<" -- Error!"<<endl;
		if (!v) b++;
	}

	if (b!=L) return FALSE;		//Eq not solved => too many errors!

	*NErrs = L;
	for (int i=0;i<L+1;i++) Locator[i]=C[i];

	return TRUE;
}

BOOL Forney (short* ErrMask, int Size, short* Locator, int LocLength, Galois* GF)
{
	int i, j, k, s;

//Locator[0]=4;
//Locator[1]=5;
//Locator[2]=1;

cout<<"Forney received the ErrMask";
for (i=0;i<GF->Dim-1;i++) cout<<" "<<ErrMask[i]; cout<<endl;
cout<<"Forney received the Locator";
for (i=0;i<LocLength;i++) cout<<" "<<Locator[i]; cout<<endl;

	if (LocLength + Size > GF->Dim - 1) return FALSE;	//Locator is too long (not from this message?)

	for (i=0; i<= GF->Dim-1 - (LocLength+Size); i++)
	{

//for (j=s=0,k=GF->Dim-2-i; j<LocLength; j++,k--) cout<<j<<" X "<<k<<" = "<<Locator[j]<<" x "<<ErrMask[k]<<endl;

		for (j=s=0,k=GF->Dim-2-i; j<LocLength; j++,k--) s = GF->Add(  s,  GF->Mul(Locator[j],ErrMask[k])  );
//cout<<i<<":"<<s<<"--??"<<endl;
		if (s) return FALSE;		//Wrong error locator -- it does not actually locate all existing errors!
	}
	for (	;i<= GF->Dim-1 - LocLength; i++)
	{

//for (j=s=0,k=GF->Dim-2-i; j<LocLength-1; j++,k--) cout<<j<<" X "<<k<<" = "<<Locator[j]<<" x "<<ErrMask[k]<<endl;

		for (j=s=0,k=GF->Dim-2-i; j<LocLength-1; j++,k--) s = GF->Add(  s,  GF->Mul(Locator[j],ErrMask[k])  );
//cout<<i<<":"<<s<<"--??"<<endl;
		ErrMask[k] = GF->Sub(  0,  GF->Div(s,Locator[j]) );
//cout<<i<<" -> "<<ErrMask[k]<<"--!!"<<endl;
	}

	return TRUE;
}