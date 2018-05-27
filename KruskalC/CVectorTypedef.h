#pragma once
#ifndef CVECTORTYPEDEF_H
#define CVECTORTYPEDEF_H


typedef struct int2_s
{
	int x;
	int y;
} int2;

typedef struct CVectorInt_s
{
	int size;
	int capacity;
	int *data;

} CVectorInt;

typedef struct CVectorInt2_s
{
	int size;
	int capacity;
	int2 *data;
}  CVectorInt2;





#endif // !CVECTORTYPEDEF_H
