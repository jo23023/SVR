// Base64Codec.cpp: implementation of the CBase64Codec class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Base64Codec.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBase64Codec::CBase64Codec()
{
    m_bTableInit = FALSE;
}

CBase64Codec::~CBase64Codec()
{

}

void CBase64Codec::initBase64DecodeTable()
{
	int i;
	
	for (i = 0; i < 256; ++i)
		m_DecodeTable[i] = (char)0x80;
	// default value: invalid
	
	for(i = 'A'; i <= 'Z'; ++i)
		m_DecodeTable[i] = 0 + (i - 'A');
	for(i = 'a'; i <= 'z'; ++i)
		m_DecodeTable[i] = 26 + (i - 'a');
	for(i = '0'; i <= '9'; ++i)
		m_DecodeTable[i] = 52 + (i - '0');
	
	m_DecodeTable[(unsigned char)'+'] = 62;
	m_DecodeTable[(unsigned char)'/'] = 63;
	m_DecodeTable[(unsigned char)'='] = 0;
}

unsigned int CBase64Codec::Decode(char* const in, unsigned char* const outBuffer, unsigned bufferSize, BOOL bTrimTrailingZeros)
{
	
	if(FALSE == m_bTableInit)
	{
		initBase64DecodeTable();
		m_bTableInit = TRUE;
	}
	
	unsigned char* out = (unsigned char*)malloc(strlen(in)+1);	// ensures we have enough space
	unsigned int k = 0;
	int const jMax = strlen(in) - 3;
	
	// in case "in" is not a multiple of 4 bytes (although it should be)
	for(int j = 0; j < jMax; j += 4)
	{
		char inTmp[4], outTmp[4];
		for(int i = 0; i < 4; ++i)
		{
			inTmp[i] = in[i+j];
			outTmp[i] = m_DecodeTable[(unsigned char)inTmp[i]];
			if((outTmp[i]&0x80) != 0)
				outTmp[i] = 0; // pretend the input was 'A'
		}
		
		out[k++] = (outTmp[0]<<2) | (outTmp[1]>>4);
		out[k++] = (outTmp[1]<<4) | (outTmp[2]>>2);
		out[k++] = (outTmp[2]<<6) | outTmp[3];
	}
	
	if(bTrimTrailingZeros)
	{
		while(k > 0 && out[k-1] == '\0') --k;
	}
	
	if(k > bufferSize)
	{
		free(out);
		return k;
	}
	else
	{
		memmove(outBuffer, out, k);
		free(out);
		return k;
	}


	return 0;
}

char* CBase64Codec::Encode(const char *src)
{
	static const char base64[] ="ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
								"abcdefghijklmnopqrstuvwxyz" \
								"0123456789+/";
	char *str, *dst;
	size_t l;
	int t, r;
	
	l = strlen(src);
	if ((str = (char*)malloc(((l + 2) / 3) * 4 + 1)) == NULL)
		return (NULL);
	dst = str;
	r = 0;
	
	while (l >= 3) {
		t = (src[0] << 16) | (src[1] << 8) | src[2];
		dst[0] = base64[(t >> 18) & 0x3f];
		dst[1] = base64[(t >> 12) & 0x3f];
		dst[2] = base64[(t >> 6) & 0x3f];
		dst[3] = base64[(t >> 0) & 0x3f];
		src += 3; l -= 3;
		dst += 4; r += 4;
	}
	
	switch (l) {
	case 2:
		t = (src[0] << 16) | (src[1] << 8);
		dst[0] = base64[(t >> 18) & 0x3f];
		dst[1] = base64[(t >> 12) & 0x3f];
		dst[2] = base64[(t >> 6) & 0x3f];
		dst[3] = '=';
		dst += 4;
		r += 4;
		break;
	case 1:
		t = src[0] << 16;
		dst[0] = base64[(t >> 18) & 0x3f];
		dst[1] = base64[(t >> 12) & 0x3f];
		dst[2] = dst[3] = '=';
		dst += 4;
		r += 4;
		break;
	case 0:
		break;
	}
	
	*dst = 0;
	return (str);

}

unsigned char* CBase64Codec::EncodeEx(const unsigned char *src, DWORD dwSize)
{
	static const char base64[] ="ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
								"abcdefghijklmnopqrstuvwxyz" \
								"0123456789+/";
	unsigned char *str = NULL, *dst = NULL;
	size_t l = 0;
	int t = 0, r = 0;
	
	l = dwSize == 0? strlen((char*)src): dwSize;
	if ((str = (unsigned char*)malloc(((l + 2) / 3) * 4 + 1)) == NULL)
		return (NULL);
	dst = str;
	r = 0;
	
	while (l >= 3) {
		t = (src[0] << 16) | (src[1] << 8) | src[2];
		dst[0] = base64[(t >> 18) & 0x3f];
		dst[1] = base64[(t >> 12) & 0x3f];
		dst[2] = base64[(t >> 6) & 0x3f];
		dst[3] = base64[(t >> 0) & 0x3f];
		src += 3; l -= 3;
		dst += 4; r += 4;
	}
	
	switch (l) {
	case 2:
		t = (src[0] << 16) | (src[1] << 8);
		dst[0] = base64[(t >> 18) & 0x3f];
		dst[1] = base64[(t >> 12) & 0x3f];
		dst[2] = base64[(t >> 6) & 0x3f];
		dst[3] = '=';
		dst += 4;
		r += 4;
		break;
	case 1:
		t = src[0] << 16;
		dst[0] = base64[(t >> 18) & 0x3f];
		dst[1] = base64[(t >> 12) & 0x3f];
		dst[2] = dst[3] = '=';
		dst += 4;
		r += 4;
		break;
	case 0:
		break;
	}
	
	*dst = 0;
	return (str);
	
}
