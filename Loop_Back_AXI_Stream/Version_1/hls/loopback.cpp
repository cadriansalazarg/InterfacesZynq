#include "loopback.h"

void loopback(hls::stream<AXISTREAM32> &input, hls::stream<AXISTREAM32> &output){
//#pragma HLS INTERFACE axis register both port=output
//#pragma HLS INTERFACE axis register both port=input
//#pragma HLS INTERFACE s_axilite port=return
	int buff[SIZE];
	for (int i=0; i<SIZE; i++){
		AXISTREAM32 a;
		a = input.read();
		buff[i] = a.data;
	}

	for (int i=0; i<SIZE;i++){
		AXISTREAM32 a;
		a.data = buff[i];
		a.tlast = (i==SIZE-1)? 1:0;
		output.write(a);
	}
}
