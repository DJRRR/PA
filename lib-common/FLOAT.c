#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	//nemu_assert(0);
	//return 0;
	FLOAT temp;
	if (a<b) {
		temp=a;
		a=b;
		b=temp;
	}
	return (a>>6)*(b>>2)>>8;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	//nemu_assert(0);
	//return 0;
	FLOAT c=(a/b)<<16;
	FLOAT d=(a%b);
	int i;
	for(i=0;i<16;i++){
		c+=((d<<1)/b)<<(15-i);
		d=(d<<1)%b;
	}
	return c;
}

FLOAT f2F(float a) {
	//nemu_assert(0);
	//return 0;
	void *f;
	f=&a;
	unsigned int x=*(unsigned int *)f;
	int sign=(x>>31)&1;
	int expo=(x>>23)&0xff;
	expo=(expo<<24)>>24;
	int mant=(x<<9)>>9;
	expo=expo-127-7;

	FLOAT result;
	result=(1<<23)|mant;
	if (expo>0){
		result<<=expo;
	}
	else{
		result>>=-expo;
	}
	if (sign) result=-result;
	return result;

}

FLOAT Fabs(FLOAT a) {

	//nemu_assert(0);
	//return 0;
	void *f=&a;
	unsigned int x=*(unsigned int*)f;
	if (!!(x>>31)) x=~x+1;
	return x;

}

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

