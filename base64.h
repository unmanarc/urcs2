#pragma once

class Cbase64
{
public:
	Cbase64(void);
	void Base64Encode( char* cpInput, char* cpOutput );
	void Base64Decode( char* cpInput, char* cpOutput );

	~Cbase64(void);
};
