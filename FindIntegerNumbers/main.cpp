#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct Data
{
	int A;
	int B;
	int C;
	int D;
};

typedef struct DataArray
{
	Data *Data;
	int length;
};

void AddData(DataArray *data, int a, int b, int c, int d)
{
	data->Data[data->length].A = a;
	data->Data[data->length].B = b;
	data->Data[data->length].C = c;
	data->Data[data->length].D = d;
	data->length++;
	//Ideally it should alloc a bigger block of memory every run since 
	//allocating memory for every block is IO heavy.
	data->Data = (Data*)realloc(data->Data, (data->length + 1) * sizeof(Data));
}

void WriteData(Data *data, int length) 
{
	for (int i = 0; i < length; i++) 
	{
		printf("a=%4d b=%4d c=%d d=%4d\n", data[i].A, data[i].B, data[i].C, data[i].D);
	}
}

DataArray NewDataArray() 
{
	DataArray data;
	data.length = 0;
	data.Data = (Data*)malloc(sizeof(Data));
	return data;
}

void ClearDataArray(DataArray* data)
{
	data->length = 0;
	free(data->Data);
	data->Data = (Data*)malloc(sizeof(Data));
}

bool DataArrayEqual(DataArray* data1, DataArray* data2) {
	bool equal = data1->length == data2->length;
	int match = 0;
	if (!equal) {
		return false;
	}
	for (int i = 0; i < data1->length; i++) {
		for (int j = 0; j < data2->length; j++) {
			if (data1->Data[i].A == data2->Data[j].A &&
				data1->Data[i].B == data2->Data[j].B &&
				data1->Data[i].C == data2->Data[j].C &&
				data1->Data[i].D == data2->Data[j].D)
			{
				match++;
			}
		}
	}
	return match == data1->length;
}


int main() {
	DataArray data = NewDataArray();
	DataArray data2 = NewDataArray();
	//Find positive numbers where the result is lower than 1000
	//where a³ + b³ = c³ + d³
	//Method 1
	int resultLeft = 0;
	int resultRight = 0;
	float time1 = (float)clock() / CLOCKS_PER_SEC;
	for (int i = 0; i < 1000; i++) {
		for (int j = 0; j < 1000; j++) {
			resultLeft = pow(i, 3) + pow(j, 3);
			if (resultLeft > 1000) {
				break;
			}
			for (int k = 0; k < 1000; k++) {
				for (int l = 0; l < 1000; l++) {
					resultRight = pow(k, 3) + pow(l, 3);
					if (resultRight > 1000)
					{
						break;
					}
					else if (resultLeft == resultRight)
					{
						AddData(&data, i, j, k, l);
					}
				}
			}
		}
	}
	time1 = (float)clock() / CLOCKS_PER_SEC - time1;
	printf(" Found %d matches\n", data.length);
//	WriteData(data.Data, data.length);
//	ClearDataArray(&data);

	//Method 2
	float time2 = (float)clock() / CLOCKS_PER_SEC;
	for (int i = 0; i < 1000; i++) {
		for (int j = 0; j < 1000; j++) {
			resultLeft = pow(i, 3) + pow(j, 3);
			if (resultLeft > 1000) {
				break;
			}
			if (i != j) {
				AddData(&data2, i, j, i, j);
			}
			AddData(&data2, i, j, j, i);
		}
	}
	time2 = (float)clock() / CLOCKS_PER_SEC - time2;
	printf(" Found %d matches\n", data2.length);
//	WriteData(data.Data, data.length);
	printf("Method2 took %f ticks while Method1 took %f ticks\n Values Match:%d\n", time2, time1, DataArrayEqual(&data, &data2));
	system("pause");
}