#pragma once

#ifndef KRUSKALDATATYPES_H
#define KRUSKALDATATYPES_H


#define Infinity 0

typedef struct NodePair_s
{
	int a;
	int b;
	int weight;
} NodePair;

static int NodePairComparator(NodePair a, NodePair b)
{
	if (a.weight == Infinity)
		return 1;
	else
	if (b.weight == Infinity)
		return -1;
	else
		return a.weight - b.weight;
}

static int NodePairComparatorRaw(NodePair a, NodePair b)
{
    return a.weight - b.weight;
}



typedef struct CVectorNodePair_s
{
	int size;
	int capacity;
	NodePair *data;

} CVectorNodePair;


#endif // KRUSKALDATATYPES_H
