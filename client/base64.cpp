#include "StdAfx.h"
#include "base64.h"

static char* _cpBase64Encoding =   "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";


Cbase64::Cbase64(void)
{
}

Cbase64::~Cbase64(void)
{
}

void Cbase64::Base64Encode( char* cpInput, char* cpOutput )
{
 int nIdx[ 4 ];  // will contain the indices of coded letters in 
                  // _cpBase64Encoding string; valid values [0..64]; the value
                 // of 64 has special meaning - the padding symbol

 // process the data (3 bytes of input provide 4 bytes of output)
 while ( '\0' != *cpInput )
 {
   nIdx[0] = ((*cpInput) & 0xFC)>>2;
   nIdx[1] = ((*cpInput) & 0x03)<<4;
   cpInput++;
   if ( '\0' != *cpInput )
   {
     nIdx[1] |= ((*cpInput) & 0xF0)>>4;
     nIdx[2]  = ((*cpInput) & 0x0F)<<2;
     cpInput++;
     if ( '\0' != (*cpInput) )
     {
       nIdx[2] |= ((*cpInput) & 0xC0) >> 6;
       nIdx[3]  = (*cpInput) & 0x3F;
       cpInput++;
     }
     else
       nIdx[3] = 64;
   }
   else
   { // refer to padding symbol '='
     nIdx[2] = 64;
     nIdx[3] = 64;
   }

   *(cpOutput+0) = *(_cpBase64Encoding + nIdx[0]);
   *(cpOutput+1) = *(_cpBase64Encoding + nIdx[1]);
   *(cpOutput+2) = *(_cpBase64Encoding + nIdx[2]);
   *(cpOutput+3) = *(_cpBase64Encoding + nIdx[3]);
   cpOutput += 4;
 }
 
 // set this to terminate output string
 *cpOutput = '\0';

 return;
}

void Cbase64::Base64Decode( char* cpInput, char* cpOutput )
{
 int nIdx[ 4 ];  // will contain the indices of coded letters in 
                  // _cpBase64Encoding string; valid values [0..64]; the value
                 // of 64 has special meaning - the padding symbol

 // process the data (3 bytes of input provide 4 bytes of output
 while ( '\0' != *cpInput )
 {
   nIdx[ 0 ] = nIdx[ 1 ] = nIdx[ 2 ] = nIdx[ 3 ] = 64;
   nIdx[0] = (strchr( _cpBase64Encoding, (*cpInput) ) - _cpBase64Encoding);
   cpInput++;
   if ( '\0' != *cpInput )
   {
     nIdx[1] = (strchr( _cpBase64Encoding, (*cpInput) ) - _cpBase64Encoding);
     cpInput++;
     if ( '\0' != (*cpInput) )
     {
       nIdx[2] = (strchr( _cpBase64Encoding, (*cpInput) ) - _cpBase64Encoding);
       cpInput++;
       if ( '\0' != (*cpInput) )
       {
         nIdx[3] = (strchr( _cpBase64Encoding, (*cpInput) ) - _cpBase64Encoding);
         cpInput++;
       }
     }
   }

   if ( nIdx[3] == 64 ) nIdx[3] = 0;
   if ( nIdx[2] == 64 ) nIdx[2] = 0;
   if ( nIdx[1] == 64 ) nIdx[1] = 0;

   *(cpOutput+0) = (char)((nIdx[0]<<2) | (nIdx[1]>>4));
   *(cpOutput+1) = (char)((nIdx[1]<<4) | (nIdx[2]>>2));
   *(cpOutput+2) = (char)((nIdx[2]<<6) | nIdx[3]);
   cpOutput += 3;
 }
 
 // set this to terminate output string
 *cpOutput = '\0';

 return;
}
