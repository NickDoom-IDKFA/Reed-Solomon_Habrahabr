#include <windows.h>

#include <iostream.h>	//For debug
#include <stdio.h>	//For debug

#include "Galois.h"

Galois::Galois (int Base, int Order)
{
	int i, j, k;

//	if (Order != 1) return FALSE;	//Not supported yet!

	for (Dim=Base,i=1; i<Order; Dim*=Base);

	DivLUT = (unsigned short*)malloc(Dim*Dim*sizeof(unsigned short));
	MulLUT = (unsigned short*)malloc(Dim*Dim*sizeof(unsigned short));
	AddLUT = (unsigned short*)malloc(Dim*Dim*sizeof(unsigned short));
	SubLUT = (unsigned short*)malloc(Dim*Dim*sizeof(unsigned short));
	GField = (unsigned short*)malloc(Dim*Dim*sizeof(unsigned short));

	// Вот эта вот вся пежня -- только для первой степени. Предполагается, что для других степеней достаточно правильно забить LUTы и всё остальное заработает.

	for (i=0;i<Dim;i++) for (j=0;j<Dim;j++) MulLUT[i*Dim+j]=(i*j)%Dim;	//Забиваем таблицу умножения 
	for (i=0;i<Dim;i++) DivLUT[i*Dim+0]=0xFFFF;					//Флаги "ошибка деления на 0"
	for (i=0;i<Dim;i++) for (j=1;j<Dim;j++) for (k=0; k<Dim; k++) if (MulLUT[k*Dim+j]==i) DivLUT[i*Dim+j]=k;	//Забиваем таблицу деления

	for (i=0;i<Dim;i++) for (j=0;j<Dim;j++) AddLUT[i*Dim+j]=(i+j)%Dim;	//Забиваем таблицу сложения 
	for (i=0;i<Dim;i++) for (j=0;j<Dim;j++) SubLUT[i*Dim+j]=(Dim+i-j)%Dim;	//Забиваем таблицу вычитания

	for (i=0;i<Dim;i++) GField[i]=0;	//Нуль в степенях
	for (i=1;i<Dim;i++) for (j=2,GField[i*Dim]=1,GField[i*Dim+1]=i; j<Dim; j++) GField[i*Dim+j]=(GField[i*Dim+j-1]*i)%Dim;	//Забиваем таблицу степеней, она же и есть Поле Галуа ЕМНИП

	for (PrimEl=Dim-1; PrimEl>0; PrimEl--)
	{
		int Count[1024]={0};				//ToDo -- get rid of the fuckin' hard-coded size!!!
		for (i=0; i<Dim; i++) Count[ GField[PrimEl*Dim+i] ]++;
		if (GField[PrimEl*Dim]==1&&GField[PrimEl*Dim + Dim-1]==1)
		{
			for (i=2; i<Dim; i++) if (Count[i]!=1) break;
			if (i==Dim) break;
		}
//for (P=0; P<Dim; P++) cout<<Count[P]<<" "; cout<<endl;
	}
	if (!PrimEl) cout<<"I still misunderstand Galois!"<<endl;	//return FALSE
else cout<<"PrimEl = "<<PrimEl<<endl;

cout<<"Galois (Powers):"<<endl;
for (i=0;i<Dim;i++,cout<<endl) for (j=0;j<Dim;j++) cout<<GField[i*Dim+j]<<"	";
cout<<"Multiply:"<<endl;
for (i=0;i<Dim;i++,cout<<endl) for (j=0;j<Dim;j++) cout<<MulLUT[i*Dim+j]<<"	";
cout<<"Divide:"<<endl;
for (i=0;i<Dim;i++,cout<<endl) for (j=0;j<Dim;j++) cout<<DivLUT[i*Dim+j]<<"	";
cout<<"Add:"<<endl;
for (i=0;i<Dim;i++,cout<<endl) for (j=0;j<Dim;j++) cout<<AddLUT[i*Dim+j]<<"	";
cout<<"Subtract:"<<endl;
for (i=0;i<Dim;i++,cout<<endl) for (j=0;j<Dim;j++) cout<<SubLUT[i*Dim+j]<<"	";

}

//Пока всё на "лук-об-тэйблах", потом поглядим, где можно зажабить немножечко оперативки.

Galois::Div (int X, int Y)
{
	return DivLUT[X*Dim + Y];
}

Galois::Mul (int X, int Y)
{
if (X>=Dim) for(;;) flushall();
	return MulLUT[X*Dim + Y];
}

Galois::Add (int X, int Y)
{
	return AddLUT[X*Dim + Y];
}

Galois::Sub (int X, int Y)
{
	return SubLUT[X*Dim + Y];
}

Galois::GFi (int X, int Y)
{
	return GField[X*Dim + Y];
}

Galois::~Galois ()
{
	free(DivLUT);
	free(MulLUT);
	free(AddLUT);
	free(SubLUT);
	free(GField);
}