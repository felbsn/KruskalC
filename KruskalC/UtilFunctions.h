#pragma once


#include "KruskalDataTypes.h"
#include "CVector.h"
#include <math.h>

int ParseLineToArray(char* input, int len, int** output);
int* FixedLineToArray(char* input, int len, int arrLen);

int ** MatrixReader(int* dimensions, const char* fileLoc)
{
	FILE * fi = fopen(fileLoc, "r");
	if (fi)
	{
		int * * mx;
		char buffer[512];


		fgets(buffer, 512, fi);

		int len = strlen(buffer);


		int* firstLine = 0;
		int arrSize = ParseLineToArray(buffer, len - 1, &firstLine);
		if (arrSize > 0)
		{
			*dimensions = arrSize;
		    mx = (int**)malloc(sizeof(int*) *  arrSize);
			int i;
			mx[0] = firstLine;

			for ( i = 1; i < arrSize; i++)
			{
				fgets(buffer, 512, fi);
				mx[i] = FixedLineToArray(buffer, strlen(buffer)-1, arrSize);
			}
		}

		fclose(fi);



		return mx;
	}
	else
	{
		return 0;
	}
}

int ParseLineToArray( char* input , int len, int** output)
{
	CVectorInt vec;
	VectorInit(vec);
	int  lineIndex = 0;
	int  arrayIndex = 0;

	VectorAppend(vec, 0);
	while (lineIndex < len)
	{
		if (input[lineIndex] == ',')
		{
			arrayIndex++;
			VectorAppend(vec, 0);
		}
		else
		{
			vec.data[arrayIndex] *= 10;
			vec.data[arrayIndex] += input[lineIndex] - '0';
		}
		lineIndex++;
	}

	*output = vec.data;
	return vec.size;
}

int* FixedLineToArray( char* input ,int len   ,int arrLen )
{
	int* arr = (int*)malloc(sizeof(int) * arrLen);
	int  lineIndex = 0;
	int  arrayIndex = 0;
	arr[0] = 0;
	while (lineIndex < len)
	{
		if (input[lineIndex] == ',' )
		{
			arrayIndex++;
			arr[arrayIndex] = 0;
		}
		else
		{
			arr[arrayIndex] *= 10;
			arr[arrayIndex] += input[lineIndex] - '0';
		}
		lineIndex++;
	}
	return arr;

}

void PrintMatrix(int** mx, int row, int column)
{
    int i;
    int j;


    printf(" %2d" ,0);
    for (j = 0; j < column; j++)
    {
		    printf("|%2d", j);
    }
    printf("\n");



	for (i = 0; i < row; i++)
	{
	    printf("|%2d", i);

		for (j = 0; j < column; j++)
		{
			if(mx[i][j] == 0)
			printf(" %2s", "-");
			else
			printf(" %2d", mx[i][j] );
		}
		printf("\n");
	}
}



CVectorInt * SolveNodeList(const CVectorInt vecList, int setCount);

CVectorNodePair MatrixToNodePairs(int** mx, int dimension)
{
	CVectorNodePair vec;
	VectorInit(vec);
	int i, j;
	for ( i = 0; i < dimension; i++)
	{
		for ( j = i+1; j < dimension; j++)
		{
			NodePair np;
			np.a = i;
			np.b = j;
			np.weight = mx[i][j];
			VectorAppend(vec, np);
		}
	}


	//VectorBubleSort(vec, NodePairComparator);
	VectorSort(vec, NodePairComparator);



	NodePair inf;
	int indexInfinity;
	inf.weight = Infinity;

	VectorLinearSearchF(vec , inf ,indexInfinity , NodePairComparatorRaw);
    printf("Generated Paths %d \n" , indexInfinity);
	if(indexInfinity > 0)
    {
        VectorResize(vec  ,indexInfinity);
    }
    system("pause");

	for ( i = 0; i < vec.size; i++)
	{
		printf(" a:%2d b:%2d  weight:%5d \n", vec.data[i].a, vec.data[i].b, vec.data[i].weight == Infinity ? -1 : vec.data[i].weight);
	};
	return vec;

}


void printColor( int id, int col)
{

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;

	/* Save current attributes */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;

	SetConsoleTextAttribute(hConsole, saved_attributes | col);
	printf("%2d" , id);
	SetConsoleTextAttribute(hConsole, saved_attributes);


}


// no path collection
int DeductionWorker(const CVectorNodePair vecPairs , CVectorInt vecList, int setCount , int maxSetSize, int targetSetCount)
{
	int i = 0;
	while (i < vecPairs.size &&  setCount > targetSetCount)
	{
		if (vecList.data[vecPairs.data[i].a] != vecList.data[vecPairs.data[i].b])
		{
			int altIndex = 0;
			int setElementCount = 0;
			while ((altIndex  < vecList.size) && (setElementCount <= maxSetSize))
			{
				if (vecList.data[altIndex] == vecList.data[vecPairs.data[i].b] || vecList.data[altIndex] == vecList.data[vecPairs.data[i].a])
				{
					setElementCount++;
				}
				altIndex++;
			}
			if (setElementCount <= maxSetSize)
			{
				int targetID;
				int repID;
				if (vecList.data[vecPairs.data[i].b] < vecList.data[vecPairs.data[i].a])
				{
					targetID = vecList.data[vecPairs.data[i].a];
					repID = vecList.data[vecPairs.data[i].b];
				}
				else
				{
					repID = vecList.data[vecPairs.data[i].a];
					targetID = vecList.data[vecPairs.data[i].b];

				}
				printf("A:%2d  B:%2d\n", vecPairs.data[i].a , vecPairs.data[i].b);
				printf("total count > %d  >>  replaced ! target %d to >> %d\n", setElementCount, targetID, repID);
				printf("pair id test  <%3d> \n", i);
				printf("\n");
				VectorReplace(vecList, targetID, repID);
				setCount--;
			}
		}

		// increments
		i++;
	}
	return setCount;
}

CVectorInt DeductionMethod(const CVectorNodePair vecPairs , int elements , int targetSetCount  , int* outSetCount)
{
	// base vector set id list

	int setCount;
	int leapCount;

	int firstSetCountTarget;
	int maxSetSize;
	int i;

	CVectorInt vecList;

	if (elements  % targetSetCount == 0)
	{
		maxSetSize = elements / targetSetCount;
		leapCount = 0;
	}
	else
	{
		leapCount = elements % (targetSetCount - 1);
		maxSetSize = ((elements - leapCount) / (targetSetCount - 1));
	}


	firstSetCountTarget = elements / maxSetSize;
	setCount = elements;

	VectorInit(vecList);
	VectorResize(vecList, elements);

	for (i = 0; i < elements; i++)
	{
		vecList.data[i] = i + 1;
	}

	//initial process
	setCount = DeductionWorker(vecPairs, vecList, setCount, maxSetSize, targetSetCount);
	// process leap pairs
	setCount = DeductionWorker(vecPairs, vecList, setCount, leapCount, targetSetCount);

	*outSetCount = setCount;
	return vecList;

}

CVectorInt* GenerataSets(const CVectorNodePair vecPairs ,int elements , int targetSetCount)
{


	int totalSetCounts;
	CVectorInt vecList = DeductionMethod(vecPairs, elements, targetSetCount , &totalSetCounts);

	CVectorInt* pairList = SolveNodeList(vecList, totalSetCounts);

	return pairList;

}



// fixed path
int DeductionWorkerPathFixed(const CVectorNodePair vecPairs, CVectorInt vecList, int setCount, int maxSetSize, int targetSetCount, CVectorInt2* vecPath , int balanced)
{

	int i = 0;
	while (i < vecPairs.size &&  setCount > targetSetCount)
	{
		if (vecList.data[vecPairs.data[i].a] != vecList.data[vecPairs.data[i].b])
		{
			int altIndex = 0;
			int setElementCount = 0;
			if(balanced)
            {
			while ((altIndex  < vecList.size) && (setElementCount <= maxSetSize))
			{
				if (vecList.data[altIndex] == vecList.data[vecPairs.data[i].b] || vecList.data[altIndex] == vecList.data[vecPairs.data[i].a])
				{
					setElementCount++;
				}
				altIndex++;
			}
            if (setElementCount <= maxSetSize) {i++ ; continue;}
			}
			{
				int targetID;
				int repID;
				if (vecList.data[vecPairs.data[i].b] < vecList.data[vecPairs.data[i].a])
				{
					targetID = vecList.data[vecPairs.data[i].a];
					repID = vecList.data[vecPairs.data[i].b];
				}
				else
				{
					repID = vecList.data[vecPairs.data[i].a];
					targetID = vecList.data[vecPairs.data[i].b];

				}
				printf("A:%2d  B:%2d\n", vecPairs.data[i].a, vecPairs.data[i].b);
				printf("total count > %d  >>  replaced ! target %d to >> %d\n", setElementCount, targetID, repID);
				printf("pair id test  <%3d> \n", i);
				printf("\n");


				int2 add;
				add.y = i;
				add.x = vecPairs.data[i].a;
				VectorAppend(*vecPath, add);
				add.x = vecPairs.data[i].b;
				VectorAppend(*vecPath, add);

				VectorReplace(vecList, targetID, repID);
				setCount--;
			}
		}

		// increments
		i++;
	}
	return setCount;
}
CVectorInt DeductionMethodWPath(const CVectorNodePair vecPairs, int elements, int targetSetCount, int* outSetCount, CVectorInt2* vecPath , int balance)
{
	// base vector set id list

	int setCount;
	int leapCount;

	int firstSetCountTarget;
	int maxSetSize;
	int i;

	CVectorInt vecList;

	if (elements  % targetSetCount == 0)
	{
		maxSetSize = elements / targetSetCount;
		leapCount = 0;
	}
	else
	{
		leapCount = elements % (targetSetCount - 1);
		maxSetSize = ((elements - leapCount) / (targetSetCount - 1));
	}

	//path
	VectorInit(*vecPath);

	firstSetCountTarget = elements / maxSetSize;
	setCount = elements;

	VectorInit(vecList);
	VectorResize(vecList, elements);

	for (i = 0; i < elements; i++)
	{
		vecList.data[i] = i + 1;
	}

	//initial process
	setCount = DeductionWorkerPathFixed(vecPairs, vecList, setCount, maxSetSize, targetSetCount , vecPath , balance);
	// process leap pairs
	setCount = DeductionWorkerPathFixed(vecPairs, vecList, setCount, leapCount, targetSetCount , vecPath , balance);

	*outSetCount = setCount;
	return vecList;

}
CVectorInt* GenerataSetsWPaths(const CVectorNodePair vecPairs, int elements, int targetSetCount , CVectorInt2* vecPath ,int * outSetCount , int balance)
{
	int totalSetCounts;
	CVectorInt vecList = DeductionMethodWPath(vecPairs, elements, targetSetCount, &totalSetCounts  , vecPath , balance);

	CVectorInt* pairList = SolveNodeList(vecList, totalSetCounts);
    *outSetCount = totalSetCounts;
	return pairList;

}


CVectorInt * SolveNodeList(const CVectorInt vecList  , int setCount)
{
	CVectorInt *pairList = (CVectorInt*)malloc(sizeof(CVectorInt) * setCount);
	CVectorInt vecSymbol;
	VectorInit(vecSymbol);

	int i;
	for ( i = 0; i < setCount; i++)
	{
		VectorInit(pairList[i]);
	}

	for (i = 0; i < vecList.size; i++)
	{
		int findAt;
		VectorLinearSearch(vecSymbol, vecList.data[i], findAt);
		if (findAt < 0)
		{
			VectorAppend(vecSymbol, vecList.data[i]);
			findAt = vecSymbol.size - 1;
		}
		VectorAppend(pairList[findAt], i);
	}

	return pairList;
}
