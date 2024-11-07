// EncryptData.cpp
//
// This file uses the input data and key information to encrypt the input data
//

#include "Main.h"

void encryptData_01(char *data, int datalength)
{

	__asm
	{
		movzx eax, byte ptr gPasswordHash[0]  // Load gPasswordHash[0] into eax and zero-extend
		shl eax, 8                            // Shift left by 8 to multiply by 256
		movzx ebx, byte ptr gPasswordHash[1]  // Load gPasswordHash[1] into ebx and zero-extend
		add eax, ebx                          // Combine to form starting_index in eax

		xor ecx, ecx           // Initialize counter x to 0
		mov edx, datalength    // Load datalength into edx
		mov esi, data          // Load pointer to data into esi
		mov edi, eax           // Load starting_index into edi (index for gKey)
		
		xor_loop:
			cmp ecx, edx                   // Compare x with datalength
			jge end_loop                   // If x >= datalength, exit loop
			mov al, byte ptr[esi + ecx]    // Load data[x] into al
			xor al, byte ptr[gkey + edi]   // XOR with gKey[starting_index]
			mov byte ptr[esi + ecx], al    // Store result back in data[x]
			inc ecx                        // Increment x
			jmp xor_loop                   // Repeat loop
		
		end_loop:
	}

	return;
} // encryptData_01


