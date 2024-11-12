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

void encryptData_02(char *data, int datalength)
{
	__asm {
		movzx eax, byte ptr [gPasswordHash]  // Load gPasswordHash[0] into eax and zero-extend
		shl eax, 8                            // Shift left by 8 to multiply by 256
		movzx ebx, byte ptr [gPasswordHash+1] // Load gPasswordHash[1] into ebx and zero-extend
		add eax, ebx                          // Combine to form starting_index in eax
		xor ecx, ecx           // Initialize counter x to 0
		mov edx, [ebp+0xC]       // Load datalength into edx
		mov esi, [ebp+8]       // Load pointer to data into esi
		mov edi, eax           // Load starting_index into edi (index for gKey)

		xor_loop_2:
			cmp ecx, edx                   // Compare x with datalength
			jge end_loop_2                   // If x >= datalength, exit loop
			xor eax, eax
			mov al, byte ptr[esi + ecx]    // Load data[x] into al
			xor al, byte ptr[gkey + edi]  // XOR with gKey[starting_index]

			mov al, [gEncodeTable+eax] // al = table[al]


			mov ah, al
			and ah, 0xF0
			shr ah, 4
			and al, 0xF
			shl al, 4
			or  al, ah // abcd efgh -> efgh abcd

			mov ah, al
			and ah, 0xCC // 1100
			shr ah, 2
			and al, 0x33 // 0011
			shl al, 2
			or  al, ah // efgh abcd -> ghef cdab

			mov ah, al
			and ah, 0xAA // 1010
			shr ah, 1
			and al, 0x55 //0101
			shl al, 1
			or  al, ah // ghef cdab -> hgfe dcba


			mov ah, cl //temp = cl
			mov cl, al
			and cl, 0x3 //0011
			ror al, cl
			mov cl, ah
			
			mov ah, al
			and ah, 0x88 //1000
			shr ah, 3
			and al, 0x77 //0111
			shl al, 1
			or  al, ah

			xor al, 0xC3 //1100 0011
			
			mov byte ptr[esi + ecx], al    // Store result back in data[x]
			inc ecx                        // Increment x
			jmp xor_loop_2                   // Repeat loop

		end_loop_2:
	}
}
