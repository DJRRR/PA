#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
/*	long long int a_t=a&0x7fffffff;
	long long int b_t=b&0x7fffffff;
	int flag_a=a&0x80000000;
	int flag_b=b&0x80000000;
	long long int result=a_t*b_t;
	FLOAT fin;
	if(flag_a==flag_b){
		fin=result>>16;
	}
	else{
		fin=-(result>>16);
	}
	return fin;*/
	long long tmp=(long long)a*(long long)b;
	if(tmp>=0){
		return (FLOAT)(tmp>>16);
	}
	else{
		return (FLOAT)-(-tmp>>16);
	}
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	return (a<<8)/(b>>8);
}

FLOAT f2F(float a) {
/*	float temp=a;
	FLOAT fin;
	int temp_i=*((int *)&temp);
	int flag=(temp_i>>31)&1;
	int exp=((temp_i>>23)&0xff)-127;
	int F_val=((temp_i)&0x7fffff)|0x800000;
//	int first_pos=24;
	int off_judge=exp-7;
	if(off_judge==0){
		fin=F_val|(flag<<31);
	}
	else if(off_judge<0){
		fin=(F_val>>(-off_judge))|(flag<<31);
	}
	else{
		fin=(F_val<<off_judge)|(flag<<31);
	}
	return fin;*/
	float temp=a;
//	FLOAT fin;
	int temp_i=*((int *)&temp);
	int flag=(temp_i>>31)&1;
	int exp=((temp_i>>23)&0xff)-127;
	int f2=(temp_i&0x7fff)|0x800000;
	int offset=23-exp;
	int t=((f2>>offset)<<16)&0x7fff0000;
	f2=((f2<<(32-offset))>>16)&0x0000ffff;
	int F=f2+t;
	if(flag==1){
	return -F;
	}
	else{
	return F;
	}


}

FLOAT Fabs(FLOAT a) {
	FLOAT fin;
	if(a<0){
		fin=-a;
	}
	else{
		fin=a;
	}
	return fin;
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

