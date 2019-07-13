//слово "mess" тут не просто так :) Не все хабровчане умеют одинаково хорошо объяснять.
BOOL BerleMess (int SyndLen, short* Syndrome, Galois* GF);	//VERBOTEN!!! Соглашения о передаче объектов не существует. Последний параметр катит только при сборке всех сырцов одним компилятором.

//Попробуем тут написать работоспособную версию, спасибо АнглоВики.
BOOL BerleMassey (short* Locator, int* NErrs, short* Syndrome, int SyndLen, Galois* GF);	//VERBOTEN!!! Соглашения о передаче объектов не существует. Последний параметр катит только при сборке всех сырцов одним компилятором.

BOOL Forney (short* ErrMask, int Size, short* Locator, int LocLength, Galois* GF);