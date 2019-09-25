




// This is a small vcom application that will send a hexcode to a microcontroller , when run by the eye-detection program
// Author : Mohamed yousry 
// Date of creation : 9/25/2019



//Includes

#include "stdafx.h"


// *****************************************************************************************************************************


#define UART_WORD 0X55                   // Byte that will be sent to the microcontroller 
#define DEBUG_MSG_LENGTH 255             // MAX LENGTH OF DEBUG MESSAGE ON SPRINTF
#define COM_PORT_NUMBERS 256             // NUMBER OF ASSIGNED COM PORTS 
#define COM_PORT_PATH_LENGTH 15          // Length of path in bytes 




//Uart settings

#define BAUD_RATE    9600
#define BYTE_SIZE    8
#define PARITY_NO    0
#define PARITY_ODD   1
#define PARITY_EVEN  2
#define PARITY_MARK  3
#define PARITY_SPACE 4
#define STOP_BITS_1  0
#define STOP_BITS_15 1
#define STOP_BITS_2  2


uint8_t path_format(uint8_t * pathptr, uint8_t buff_size , LPCOMMCONFIG pcommconfig);


int _tmain(int argc, _TCHAR* argv[])
{

	HANDLE vcom_handle;                      // Handle to comport 
	DWORD num_written;                       // holds returned words written 

	uint8_t path[COM_PORT_PATH_LENGTH];	     // HOLDS COM PORT NAME 

	const uint8_t uart_word = UART_WORD;	 // Word to send 
	uint8_t sprintf_ptr[DEBUG_MSG_LENGTH];	 // Debug msg for sprintf 


	COMMCONFIG commconfig; 


	path_format(path, sizeof(path) , (LPCOMMCONFIG) &commconfig);


	commconfig.dcb.BaudRate = BAUD_RATE   ; 
	commconfig.dcb.ByteSize = BYTE_SIZE   ;
	commconfig.dcb.Parity   = PARITY_NO   ;
	commconfig.dcb.StopBits = STOP_BITS_1 ;

	

	vcom_handle = CreateFile(path, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL) ; 


	// Try and open a handle to the port 

	if ((vcom_handle == INVALID_HANDLE_VALUE) || (vcom_handle == 0) || (vcom_handle == ERROR_ALREADY_EXISTS ) ) {

			sprintf_s(sprintf_ptr, sizeof(sprintf_ptr), "Error Code : %d", GetLastError());

			return 0;
	
		}

	// Handle Created - Now changing Config


	SetCommConfig(vcom_handle, &commconfig, sizeof(commconfig));

	if (!WriteFile(vcom_handle, &uart_word, (DWORD)1, &num_written, NULL)){

		sprintf_s(sprintf_ptr, sizeof(sprintf_ptr) , "Error Code : %d", GetLastError());

		 return 0 ; 

		}

	//Successfully written 

	CloseHandle(vcom_handle);

	exit((int)GetLastError());

	return 0;
}





uint8_t path_format(uint8_t * pathptr, uint8_t buff_size, LPCOMMCONFIG pcommconfig){


	DWORD i; 
	DWORD size;

	

	for (i = 1; i <= COM_PORT_NUMBERS; i++){



		sprintf_s(pathptr, buff_size, "COM%d", i);
	
		size = sizeof( pcommconfig );


		if (GetDefaultCommConfig(pathptr, pcommconfig, &size ) || (size > sizeof(pcommconfig)) ){


			if (i <= 9){

				sprintf_s(pathptr, buff_size, "COM%d", i);
			}

			else if ((i > 9) && (i <= COM_PORT_NUMBERS)){

				sprintf_s(pathptr, buff_size, "\\\\.\\COM%d", i);

			}

			return 1;
		}



	}



}

