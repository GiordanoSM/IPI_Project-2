#include "sorting.h"

namespace sort {

void InsertionSort (int* sorted_values, int size)
{
	int start = 0, end = size - 1;
	int current = start + 1;
	int aux, swap;

	while (current <= end) // Percorre todo o array
	{
		aux = current;
		while (aux > start)
		{
			if (sorted_values[aux] < sorted_values[aux-1]) // Compara com os valores anteriores
			{
				swap = sorted_values[aux];
				sorted_values[aux] = sorted_values[aux-1];
				sorted_values[aux-1] = swap;

				aux = aux - 1;
			}

			else 
				aux = start;
		}
		current ++;
	}
} 

}