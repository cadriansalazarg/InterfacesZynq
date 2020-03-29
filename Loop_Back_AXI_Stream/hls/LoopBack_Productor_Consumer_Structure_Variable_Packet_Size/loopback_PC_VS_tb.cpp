#include <iostream>
#include <vector>
#include <array>
#include <random>
#include "loopback_PC_VS.hpp"

int main(){

	data_type DataSended[SIZE];
	data_type DataReceived[SIZE];
	hls::stream<AXISTREAM32> inputbuffer;
	hls::stream<AXISTREAM32> outputbuffer;
	unsigned int len_dma;
	
	len_dma = SIZE;
	
	InicializarArregloEntrada: for(int k=0; k<len_dma; k++){
		DataSended[k] = k;
	}
	
	ExecuteNumberOfSteps: for (int j=0; j<NUM_OF_TESTS ; j++){

		EscribirBuffer: for (int i=0; i<len_dma;i++){
			AXISTREAM32 a;
			a.data = DataSended[i];
			a.tlast = (i==len_dma-1)? 1:0;
			inputbuffer.write(a);
		}


		loopback(inputbuffer, outputbuffer, len_dma);

		LeerBuffer: for (int i=0; i<len_dma; i++){
			AXISTREAM32 a;
			a = outputbuffer.read();
			DataReceived[i] = a.data;
		}
	
		Validation: for (int i=0; i<len_dma; i++){
			printf("Paso de simulación %d. Elemento del areglo: %d. Dato teórico: %d. Dato obtenido del hardware: %d.\n", j+1, i, DataSended[i], DataReceived[i]);
			if(DataReceived[i] != DataSended[i]){
				printf("Error en el dato número %d del arreglo",i);
				return 0;
			}
		}
	}
	
	printf("La simulación funcionó sin problemas\n");
	return 0;
}
