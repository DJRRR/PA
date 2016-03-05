#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	int   a_t=a;
	int   b_t=b;
	long long  result=a_t*b_t;
	FLOAT fin;
	fin=result>>16;
	return fin;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	  FLOAT fin;
	  fin=(a/b)<<16;
	  return fin;
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
	int val_1=temp_i&0x7fff;
	val_1=val_1|0x800000;
	int val_2=((val_1>>(7-exp)))&0x7fff0000;
	val_1=(val_1<<(exp-7))&0xffff;
	int F=val_1+val_2;
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

