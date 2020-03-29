/***************************** Include Files *********************************/
#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include "xparameters.h"
#include "xaxidma.h"
#include "xdebug.h"
#include "xloopback.h"
#include "xscugic.h"

/**************************** Type Definitions *******************************/

#define DMA_DEV_ID		XPAR_AXIDMA_0_DEVICE_ID
#define SIZE 64 // This variable define the size of the dma channel.Dont be greater than the depth of the bus_local define in the directive.tcl file

/*************************** Global variables *******************************/


const unsigned int num_tests = 1000000;
unsigned int dma_length = SIZE;

int input_bffr[SIZE];  // Deben ser globales porque sino algunas veces en el SDK se cuelga el DMA y lee valores erroneos
int output_bffr[SIZE]; // Deben ser globales porque sino algunas veces en el SDK se cuelga el DMA y lee valores erroneos
/************************** Function Prototypes ******************************/

int XAxiDma_SimplePollExample(u16 DeviceId);
void XLoopBackStart(void *InstancePtr);

/************************** Variable Definitions *****************************/
/*
 * Device instance definitions
 */
XAxiDma AxiDma;

XLoopback xloopback_dev;

XLoopback_Config xloopback_config = {
	0,
	XPAR_LOOPBACK_0_S_AXI_AXILITES_BASEADDR
};

//Interrupt Controller Instance
XScuGic ScuGic;

volatile static int RunExample = 0;
volatile static int ResultExample = 0;


int main()
{
	int Status;


	xil_printf("\r\n--- Entering main() --- \r\n");

	/* Run the poll example for simple transfer */
	Status = XAxiDma_SimplePollExample(DMA_DEV_ID);

	if (Status != XST_SUCCESS) {
		xil_printf("XAxiDma_SimplePoll Example Failed\r\n");
		return XST_FAILURE;
	}

	xil_printf("Successfully ran XAxiDma_SimplePoll Example\r\n");

	xil_printf("--- Exiting main() --- \r\n");

	return XST_SUCCESS;

}

int XAxiDma_SimplePollExample(u16 DeviceId)
{
	XAxiDma_Config *CfgPtr;
	int Status;
	unsigned int j,i;

	for (j=0; j<dma_length;j++) // initialization input array
		input_bffr[j] = j;

	CfgPtr = XAxiDma_LookupConfig(DeviceId);
	if (!CfgPtr) {
		xil_printf("No config found for %d\r\n", DeviceId);
		return XST_FAILURE;
	}

	Status = XAxiDma_CfgInitialize(&AxiDma, CfgPtr);
	if (Status != XST_SUCCESS) {
		xil_printf("Initialization failed %d\r\n", Status);
		return XST_FAILURE;
	}

	if(XAxiDma_HasSg(&AxiDma)){
		xil_printf("Device configured as SG mode \r\n");
		return XST_FAILURE;
	}

	Status = XLoopback_CfgInitialize(&xloopback_dev,&xloopback_config);
	if(Status != XST_SUCCESS){
		xil_printf("IP Initialization failed\n");
		return XST_FAILURE;
	}

	/* Disable interrupts, we use polling mode
	 */
	XAxiDma_IntrDisable(&AxiDma, XAXIDMA_IRQ_ALL_MASK,
						XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&AxiDma, XAXIDMA_IRQ_ALL_MASK,
						XAXIDMA_DMA_TO_DEVICE);

	XLoopback_Set_len_dma(&xloopback_dev, dma_length);



	for (j=0; j<num_tests;j++){ // Execute different number of tests
		XLoopBackStart(&xloopback_dev);

		Xil_DCacheFlushRange((unsigned int)input_bffr,dma_length*sizeof(int));
		Xil_DCacheFlushRange((unsigned int)output_bffr,dma_length*sizeof(int));

		Status = XAxiDma_SimpleTransfer(&AxiDma, (unsigned int) output_bffr, dma_length*sizeof(int),
			XAXIDMA_DEVICE_TO_DMA);

		if (Status != XST_SUCCESS) {
			return XST_FAILURE;
		}

		Status = XAxiDma_SimpleTransfer(&AxiDma, (unsigned int) input_bffr, dma_length*sizeof(int),
			XAXIDMA_DMA_TO_DEVICE);

		if (Status != XST_SUCCESS) {
			return XST_FAILURE;
		}

		while ((XAxiDma_Busy(&AxiDma,XAXIDMA_DEVICE_TO_DMA)) ||
				(XAxiDma_Busy(&AxiDma,XAXIDMA_DMA_TO_DEVICE))) {
		}

		for (i=0; i<dma_length;i++){ // Validation
			if(output_bffr[i] != input_bffr[i])
				return XST_FAILURE;
		}
	}

	return XST_SUCCESS;
}


void XLoopBackStart(void *InstancePtr){
	XLoopback *pExample = (XLoopback *)InstancePtr;
	XLoopback_InterruptEnable(pExample,1);
	XLoopback_InterruptGlobalEnable(pExample);
	XLoopback_Start(pExample);
}