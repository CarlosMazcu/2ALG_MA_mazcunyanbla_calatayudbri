// comparative.c
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//
// File for comparative of ADTs

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "EDK_MemoryManager/edk_memory_manager.h"
#include "EDK_MemoryManager/edk_platform_types.h"
#include "common_def.h"
#include "adt_stack.h"
#include "adt_queue.h"
#include "adt_vector.h"
#include "adt_list.h"
#include "adt_dllist.h"

const int kReserve_ = 10000; 
void* data[10000];
Vector* vector;
DLList* list_;


void TESTBASE_generateDataForComparative() {
	//TODO some stuff
}

void calculateTimeForFunctionInsert() {
	LARGE_INTEGER frequency;				// ticks per second
	LARGE_INTEGER  time_start, time_end;    // ticks in interval
	double elapsed_time = 0.0f;
	u32 repetitions = 500;
	list_ = DLList_create(10000);
	DLList* list2_ = DLList_create(500);
	// Create a vector with capacity for 10000 elements

	// Generate random data to fill the vector
	for (int i = 0; i < 10000; ++i) {
		data[i] = (void*)(intptr_t)rand(); // Example of generating random data
	}


	///////////////////////////////////////////////////////////////////////
	// Frequency: ticks per second
	QueryPerformanceFrequency(&frequency);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Meassurement time
	// start timer
	QueryPerformanceCounter(&time_start);
	// execute function to meassure 'repetitions' times
	for (u32 rep = 0; rep < repetitions; ++rep) {
	//	vector->ops_->insertFirst(vector, data[rep], sizeof(int));
		//int uwu = rand() % 10000;
		//list_->ops_->concat(list_,data[rep],sizeof(int), uwu);
		list_->ops_->concat(list_, list2_);
	}
	//printf("CONCAT DONETE");
	// stop timer
	QueryPerformanceCounter(&time_end);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// compute the elapsed time in microseconds
	elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
	printf("Elapsed time: %f ms\n", elapsed_time);
	///////////////////////////////////////////////////////////////////////
	// compute the average time
	double average_time = elapsed_time / repetitions;
	printf("Average time: %f ms\n", average_time);
	///////////////////////////////////////////////////////////////////////
}

void calculateTimeForFunctionExtract() {
	LARGE_INTEGER frequency;				// ticks per second
	LARGE_INTEGER  time_start, time_end;    // ticks in interval
	double elapsed_time = 0.0f;
	u32 repetitions = 10000;

	// Create a vector with capacity for 10000 elements

	// Generate random data to fill the vector
	//for (int i = 0; i < 10000; ++i) {
	//	data[i] = (void*)(intptr_t)rand(); // Example of generating random data
	//}


	///////////////////////////////////////////////////////////////////////
	// Frequency: ticks per second
	QueryPerformanceFrequency(&frequency);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Meassurement time
	// start timer
	QueryPerformanceCounter(&time_start);
	// execute function to meassure 'repetitions' times
	for (u32 rep = 0; rep < repetitions; ++rep) {
		int uwu = rand() % 10000;
		vector->ops_->extractAt(vector,uwu);
	}
	// stop timer
	QueryPerformanceCounter(&time_end);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// compute the elapsed time in microseconds
	elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
	printf("Elapsed extract time: %f ms\n", elapsed_time);
	///////////////////////////////////////////////////////////////////////
	// compute the average time
	double average_time = elapsed_time / repetitions;
	printf("Average extract time: %f ms\n", average_time);
	///////////////////////////////////////////////////////////////////////
}


int main(int argc, char** argv) {
	srand(time(NULL));
	TESTBASE_generateDataForComparative();
	calculateTimeForFunctionInsert();
	printf("\n");
	//calculateTimeForFunctionExtract();
	//MM->status();
	printf("Press ENTER to continue\n");
	getchar();
	MM->destroy();
	return 0;
}