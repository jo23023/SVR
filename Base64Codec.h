// Base64Codec.h: interface for the CBase64Codec class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASE64CODEC_H__EDFE50F8_8B25_4A07_A1A0_7B96A5B4BC66__INCLUDED_)
#define AFX_BASE64CODEC_H__EDFE50F8_8B25_4A07_A1A0_7B96A5B4BC66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBase64Codec  
{
public:
	CBase64Codec();
	virtual ~CBase64Codec();

	unsigned int Decode(char* const in, unsigned char* const outBuffer, unsigned bufferSize, BOOL bTrimTrailingZeros);
	char* Encode(const char *src);
	unsigned char* EncodeEx(const unsigned char *src, DWORD dwSize = 0);

protected:
	void initBase64DecodeTable();

	char m_DecodeTable[256];
	BOOL m_bTableInit;
};

#endif // !defined(AFX_BASE64CODEC_H__EDFE50F8_8B25_4A07_A1A0_7B96A5B4BC66__INCLUDED_)
