#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "CVector.h"
#include "UtilFunctions.h"
#include "ShellHandler.h"

#include <windows.h>


int dimensions;
int** mx = 0;
int targetCount = 0;
int generatedCount = 0;
CVectorNodePair vecPairs;
CVectorInt2 pathList;
CVectorInt* pairList;


int printmatrix(CVectorString param)
{
	PrintMatrix(mx, dimensions, dimensions);
	return 1;
}

int readmatrix(CVectorString param)
{
	if (param.size > 0)
	{
		mx = MatrixReader(&dimensions, param.data[1]);
		if (mx == 0)
		{
			printf("=> file cannot be opened !\n");
		}
		else
		{
			printf("=> matrix readed succesfully!\n");
		}



	}
	else
		printf("=> invalid usage \n");

	return 1;

}


int calculatesets(CVectorString param)
{
	if (mx == 0)
	{
		printf("matrix needs to be generated \n");
		return 0;
	}
	if(param.size > 2)
    {
        		 targetCount = atoi(param.data[1]);
		if (targetCount > 0  || targetCount < dimensions)
		{

			vecPairs = MatrixToNodePairs(mx, dimensions);

			pairList = GenerataSetsWPaths(vecPairs, dimensions, targetCount, &pathList , &generatedCount , 1);
			printf("=> sets genereated succesfully!\n");
			//generatedCount =

		}else{
		    targetCount = 0;
           printf("=> invalid size for mx: %2d X %2d !\n" , dimensions , dimensions);
		}


    }else
	if (param.size > 1)
	{
		 targetCount = atoi(param.data[1]);
		if (targetCount > 0  || targetCount < dimensions)
		{

			vecPairs = MatrixToNodePairs(mx, dimensions);

			pairList = GenerataSetsWPaths(vecPairs, dimensions, targetCount, &pathList , &generatedCount , 0);
			printf("=> sets genereated succesfully!\n");
			//generatedCount =

		}else{
		    targetCount = 0;
           printf("=> invalid size for mx: %2d X %2d !\n" , dimensions , dimensions);
		}
	}else
	 printf("=> invalid size for mx: %2d X %2d !\n" , dimensions , dimensions);


	return 1;
}


int getsetdistance(CVectorString param)
{
    if(param.size > 2)
    {
        int setA = atoi(param.data[1])-1;
        int setB = atoi(param.data[2])-1;

        printf(" distance betweeen %2d - %2d : " ,setA +1, setB+1 );

        int i;
        int minDist  = 999;
        for(i = 0; i  < pairList[setA].size ; i++ )
        {
            int j ;
            for( j = 0 ; j < pairList[setB].size ; j++)
            {
                if(mx[pairList[setA].data[i]][pairList[setB].data[j]])
                minDist = min(minDist ,mx[pairList[setA].data[i]][pairList[setB].data[j]] );

            }
        }
        if(minDist == 999)
        printf(" no transaction\n" );
            else
        printf(" minimum between %2d \n" , minDist);


    }else
    printf("=>invalid parameter \n");

	return 0;

}


int printsets(CVectorString param)
{
    if(!pairList)
    {
        printf("sets needs to be generated \n");
        return 0;
    }
    if(param.size  == 1)
    {
			int i;
			printf(" generated count is = > %d " , generatedCount);
			for (i = 0; i < generatedCount; i++)
			{
				printf("\n__________SET %2d__________\n", i + 1);
				printf("\n=========COUNT %2d============\n", pairList[i].size);
				int j;
				int k;
				for (j = 0; j < pairList[i].size; j++)
				{
					printf(" > > > > > > > > > > > >{%2d}\n", pairList[i].data[j]);

					int emptyTail = 1;
					for (k = 0; k < pathList.size; k++)
					{
						if (pairList[i].data[j] == pathList.data[k].x)
						{
							int dataIndex = pathList.data[k].y;
							printf(" => a:%2d b:%2d weight:%2d |\n", vecPairs.data[dataIndex].a, vecPairs.data[dataIndex].b, vecPairs.data[dataIndex].weight);
							emptyTail = 0;
						}
					}
					if (emptyTail)
					{

						printf(" => no transaction !\n");
					}

				}

			}
			printf("\n__________________________\n");


    }else{

        printf("  vec size %d generated count is = > %d " , param.size, generatedCount);
                int i = atoi(param.data[1]) -1;

                if( i >= generatedCount && i >= 0 )
                {
                    printf(" out of bounds \n");
                    return 0;
                }
        		printf("\n__________SET %2d__________\n", i + 1);
				printf("\n=========COUNT %2d============\n", pairList[i].size);
				int j;
				int k;
				for (j = 0; j < pairList[i].size; j++)
				{
					printf(" > > > > > > > > > > > >{%2d}\n", pairList[i].data[j]);

					int emptyTail = 1;
					for (k = 0; k < pathList.size; k++)
					{
						if (pairList[i].data[j] == pathList.data[k].x)
						{
							int dataIndex = pathList.data[k].y;
							printf(" => a:%2d b:%2d weight:%2d |\n", vecPairs.data[dataIndex].a, vecPairs.data[dataIndex].b, vecPairs.data[dataIndex].weight);
							emptyTail = 0;
						}
					}
					if (emptyTail)
					{

						printf(" => no transaction !\n");
					}

                }
               printf("\n__________________________\n");
    }
    return 1;
}


int getweight(CVectorString param)
{
	if (mx)
	{
		if(param.size > 1)
		{
		int x = atoi(param.data[1]);
		int y = atoi(param.data[2]);

		if (x >= dimensions || y >= dimensions)
		{
			return 2;
		}
		else
		{
			if (mx[x][y] > 0)
			{
				printf("a:%2d  b:%2d   weight:%2d \n ", x, y , mx[x][y]);
			}
			else
				printf("a:%2d  b:%2d   weight: - \n ", x, y);
		}

		}else
			printf("=> invalid usage \n");

	}else
		printf("matrix needs to be generated \n");
	return 1;
}

int getpath(CVectorString param)
{
	if (mx || vecPairs.size > 0)
	{
			if (param.size > 0)
			{
				int index = atoi(param.data[1]);
				if(index < vecPairs.size)
				printf("Path[%3d] a:%2d  b:%2d   weight:%2d \n ", index, vecPairs.data[index].a, vecPairs.data[index].b, vecPairs.data[index].weight);
				else
					printf("=> out of bounds \n");

			}
			else
			{
				printf("=> invalid usage \n");
				return 1;
			}
	}
	else
	{
		printf("matrix | vecPaths needs to be generated \n");
	}
	return 1;
}

int main(int argc, char * argv[])
{

	system("color f0");
	InitInlineShell();
	ShellAddCommand("printmatrix", printmatrix);
	ShellAddCommand("readmatrix", readmatrix);
	ShellAddCommand("calculatesets", calculatesets);
	ShellAddCommand("getweight", getweight);
	ShellAddCommand("printsets", printsets);
	ShellAddCommand("getpath", getpath);
	ShellAddCommand("getsetdistance" , getsetdistance);






	return RunConsoleQuery();

}
