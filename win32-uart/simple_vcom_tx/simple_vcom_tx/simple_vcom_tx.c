




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

#define APP_BAUD_RATE    CBR_9600
#define APP_BYTE_SIZE    8
#define APP_PARITY       NOPARITY
#define APP_STOP_BITS    ONESTOPBIT

#define PARTIY_NO    NO_PARITY
#define PARITY_ODD   ODDPARITY
#define PARITY_EVEN  EVENPARITY
#define PARITY_MARK  MARKPARITY
#define PARITY_SPACE SPACEPARITY
#define STOP_BITS_1  ONESTOPBIT
#define STOP_BITS_15 ONE5STOPBITS
#define STOP_BITS_2  TWOSTOPBITS


uint8_t path_format(uint8_t * pathptr, uint8_t buff_size , LPCOMMCONFIG pcommconfig);


int _tmain(int argc, _TCHAR* argv[])
{

	HANDLE vcom_handle;                      // Handle to comport 
	DWORD num_written;                       // holds returned words written 

	uint8_t path[COM_PORT_PATH_LENGTH];	     // HOLDS COM PORT NAME 

	const uint8_t uart_word = UART_WORD;	 // Word to send 
	uint8_t sprintf_ptr[DEBUG_MSG_LENGTH];	 // Debug msg for sprintf 


	COMMCONFIG commconfig; 
	DCB dcb;


	path_format(path, sizeof(path) , (LPCOMMCONFIG) &commconfig);


	vcom_handle = CreateFile(path, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL) ; 


	// Try and open a handle to the port 

	if ((vcom_handle == INVALID_HANDLE_VALUE) || (vcom_handle == 0) || (vcom_handle == ERROR_ALREADY_EXISTS ) ) {

			sprintf_s(sprintf_ptr, sizeof(sprintf_ptr), "Error Code : %d", GetLastError());

			return 0;
	
		}

	// Handle Created 

	//GetCommState now

	GetCommState(vcom_handle, &dcb);

	dcb.BaudRate = APP_BAUD_RATE ;
	dcb.ByteSize = APP_BYTE_SIZE ;
	dcb.StopBits = APP_STOP_BITS ;
	dcb.Parity	 = APP_PARITY    ; 

	SetCommState(vcom_handle, &dcb);



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

	DWORD size;

	

	for (DWORD i = 1; i <= COM_PORT_NUMBERS; i++){



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

