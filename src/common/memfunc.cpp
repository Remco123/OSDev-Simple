#include <common/memfunc.h>

using namespace myos::common;

int MemoryFunctions::memcmp(const void* aptr, const void* bptr, size_t size) {
	const unsigned char* a = (const unsigned char*) aptr;
	const unsigned char* b = (const unsigned char*) bptr;
	for (size_t i = 0; i < size; i++) {
		if (a[i] < b[i])
			return -1;
		else if (b[i] < a[i])
			return 1;
	}
	return 0;
}

void* MemoryFunctions::memcpy(void* dstptr, const void* srcptr, size_t size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	for (size_t i = 0; i < size; i++)
		dst[i] = src[i];
	return dstptr;
}

void* MemoryFunctions::memmove(void* dstptr, const void* srcptr, size_t size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	if (dst < src) {
		for (size_t i = 0; i < size; i++)
			dst[i] = src[i];
	} else {
		for (size_t i = size; i != 0; i--)
			dst[i-1] = src[i-1];
	}
	return dstptr;
}

void* MemoryFunctions::memset(void *dest,int val,size_t n) {
	uint32_t num_dwords = n/4;
	uint32_t num_bytes = n%4;
	uint32_t *dest32 = (uint32_t*)dest;
	uint8_t *dest8 = ((uint8_t*)dest)+num_dwords*4;
	uint8_t val8 = (uint8_t)val;
	uint32_t val32 = val|(val<<8)|(val<<16)|(val<<24);
	uint32_t i;

	for (i=0;i<num_dwords;i++) {
		dest32[i] = val32;
	}
	for (i=0;i<num_bytes;i++) {
		dest8[i] = val8;
	}
	return dest;
}

size_t MemoryFunctions::strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}