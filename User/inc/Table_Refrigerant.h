#ifndef		REF_H
#define		REF_H


typedef struct
{
	const float p_kpa;
	const SHORT temp_01;
}ST_REF_P_T;


typedef struct
{
	const SHORT high_total;
	const SHORT low_total;
	const ST_REF_P_T * ref_high;
	const ST_REF_P_T * ref_low;
}ST_REF_HL;


typedef enum
{
	R_11 = 0,
	R_12,
	R_13,
	R_22,
	R_23,
	R_32,
	R_113,
	R_114,
	R_115,
	R_116,
	R_123,
	R_124,
	R_125,
	R_134A,
	R_236FA,
	R_245FA,
	R_290,
	R_401A,
	R_401B,
	R_402A,
	R_402B,
	R_403B,
	R_404A,
	R_406A,
	R_407A,
	R_407B,
	R_407C,
	R_408A,
	R_409A,
	R_410A,
	R_413A,
	R_414A,
	R_414B,
	R_416A,
	R_417A,
	R_417C,
	R_420A,
	R_421A,
	R_421B,
	R_422A,
	R_422B,
	R_422C,
	R_422D,
	R_424A,
	R_427A,
	R_428A,
	R_434A,
	R_437A,
	R_438A,
	R_448A,
	R_449A,
	R_450A,
	R_452A,
	R_452B,
	R_453A,
	R_454B,
	R_458A,
	R_500,
	R_501,
	R_502,
	R_503,
	R_507A,
	R_508B,
	R_513A,
	R_600,
	R_600A,
	R_601,
	R_601A,
	R_744,
	R_1233ZD,
	R_1234YF,
	R_1234ZE,
	REF_TOTAL
}EN_REF_DEFINE;


#endif
