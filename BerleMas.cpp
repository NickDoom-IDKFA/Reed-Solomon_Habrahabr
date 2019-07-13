#include <windows.h>
#include <iostream.h>

#include "Galois.h"
#include "BerleMas.h"


int MulByToeplitz (int x, int* Gamma, short *Syndrome, int SyndLength, int Size, Galois* GF)	//VERBOTEN!!! Соглашения о передаче объектов не существует. Последний параметр катит только при сборке всех сырцов одним компилятором.
{
	int y, Val;
	if (x+Size>SyndLength) return -1;	//Неопределённое (и не нужное, кстати) значение.
	Val=0;	//Инициализируем столбец результата;
	for (y=0;y<Size;y++) Val=GF->Add( Val, GF->Mul(Gamma[y],Syndrome[x+Size-y-1]) );	//Добавляем к нему столбцы полинома Г, домноженные на свои элементы строк матрицы Тёплица (сразу же её и развернём).
	return Val;
}


#define MaxSynd 1024	//ToDo!!!

//Mess -- в смысле каша, которая не заработала
BOOL BerleMess (int SyndLen, short* Syndrome, Galois* GF)	//VERBOTEN!!! Соглашения о передаче объектов не существует. Последний параметр катит только при сборке всех сырцов одним компилятором.
{
	int i,j,k;

	int G[MaxSynd]={1};	//Максимальная длина полинома Г, которую вообще хоть как-то можно умножать на матрицу Тёплица с синдромом ошибки -- это как раз размер синдрома, ровно одно значение и будет определённым.
	int S=1;	//Начнём с длины 1 (и Тёплица 1х1, соответственно).
	int N=0;	//Насколько уже проитерировались вдоль полинома.
	static struct
	{
		int Size;	//Длина полинома числителя;
		G[MaxSynd];	//Полином числителя (ага, тот самый полином Г с нужной стадии);
		int D;		//Скалярный знаменатель (нужный элемент несрастухи)
	} History[MaxSynd];	//Ещё не вкурил, за сколько ходов алгоритм завершается тем или иным успехом :( да я вообще едва продираюсь, если честно :( Пусть будет с запасом - больше длины синдрома точно ведь не будет.
	int Temp[MaxSynd];
	int E;			//Отдельный элемент несрастухи (тупо оптимизация, чтобы не перевычислять по сто раз)

	while (S<=SyndLen)	//Вроде бы так, кажется. Если вышло за пределы - значит, решений не существует (ошибок слишком много)
	{
for (i=0; i<S; i++) cout<<G[i]<<" ";
cout<<" X [T] = ";
for (i=0; i<S; i++) cout<<MulByToeplitz(i,G,Syndrome,SyndLen,S,GF)<<" ";	//Текущая несрастуха;
cout<<endl;
	
		for (E=i=0;i<N;i++) if (E=MulByToeplitz(i,G,Syndrome,SyndLen,S,GF))	//Если несрастуха содержит ненулевые элементы слева от кандидата в знаменатели -- уберём их по одному, в этом нам помогут ранее рассчитанные вектора и коэффициенты из Истории.
		{
			if (E<0) return FALSE;// break;	//Вышли за пределы известных данных;

			memset (Temp, 0, sizeof (Temp));
			memcpy (Temp+(S-History[i].Size), History[i].G, History[i].Size*sizeof(int));
cout<<"Subtracting "<<E<<" / "<<History[i].D<<" [ ";					//Те самые стрелочки через всю страницу из хабровского мануала
for (k=0;k<S;k++) cout<<Temp[k]<<" ";
cout<<"]=";
			for (k=0;k<S;k++) Temp[k]=GF->Div( GF->Mul(Temp[k],E) , History[i].D );
for (k=0;k<S;k++) cout<<Temp[k]<<" ";
cout<<endl;
			for (k=0;k<S;k++) G[k]=GF->Sub(G[k],Temp[k]);

for (k=0; k<S; k++) cout<<G[k]<<" ";
cout<<" X [T] = ";
for (k=0; k<S; k++) cout<<MulByToeplitz(k,G,Syndrome,SyndLen,S,GF)<<" ";	//Текущая несрастуха;
cout<<endl;
		}
	
		if (E<0 || MulByToeplitz(N,G,Syndrome,SyndLen,S,GF)<0)	//Все нули, а потом данные кончились %)
		{
			if (i<(SyndLen+1)/2) {S--; continue;}
			else break;	//Success!!!
		}

		if (MulByToeplitz(N,G,Syndrome,SyndLen,S,GF))		//Все нули, а очередной -- не ноль. Запомним его, он полезный.
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
	
		G[S]=0;	//Подбросим ещё данных, допустим новый Г=0 и измерим полученную несрастуху.
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

//Плавали в Лох-Нессе
//Берлекемп и Мэсси
//Один серый
//Другой белый
//Как ж я заебаааался :(

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