/*
All CopyRights Reserved.
Created by unmanarc.com. emailme to: unma@unmanarc.com
this source are under GNU/GPL (General Public Licence)
http://urcs.unmanarc.com/
http://urcs.sourceforge.net/

by unmanarc.
thanks to all.
enjoy ;)
*/
#include "stdafx.h"
#include "funcs.h"
#include <direct.h>
#include "MD5Checksum.h"
#include "Tlhelp32.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

char returned[512];

Cfns::Cfns()
{ //Constructor
}
Cfns::~Cfns()
{ //destructor
}

BOOL Cfns::KillProcess(unsigned long pid)
{
    HANDLE            hProcess;
    hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pid );
    if (hProcess) 
	{
        hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pid );
        if (hProcess == NULL) 
		{
            return FALSE;
        }

        if (!TerminateProcess( hProcess, 1 )) 
		{
			CloseHandle( hProcess );
            return FALSE;
        }

        CloseHandle( hProcess );
        return TRUE;
    }
    return FALSE;
}

int Cfns::strpointer(char *pointer, char pnt)
{
	int point=(int)strlen(pointer);
	while(point>0)
	{
		if (pointer[point]==pnt)
			return point;
		point--;
	}
	return 0;
}

int Cfns::strrpointer(char *pointer, char pnt)
{
	int point=(int)strlen(pointer);
	while(point>0)
	{
		if (pointer[strlen(pointer)-point]==pnt)
			return point;
		point--;
	}
	return 0;
}

int Cfns::charhextointa(char dsi)
{
	if(dsi=='0')
		return 0;
	if(dsi=='1')
		return 1;
	if(dsi=='2')
		return 2;
	if(dsi=='3')
		return 3;
	if(dsi=='4')
		return 4;
	if(dsi=='5')
		return 5;
	if(dsi=='6')
		return 6;
	if(dsi=='7')
		return 7;
	if(dsi=='8')
		return 8;
	if(dsi=='9')
		return 9;
	if(dsi=='A')
		return 10;
	if(dsi=='B')
		return 11;
	if(dsi=='C')
		return 12;
	if(dsi=='D')
		return 13;
	if(dsi=='E')
		return 14;
	if(dsi=='F')
		return 15;
	return -1;
}


int Cfns::charhextoint(char *dsi)
{
	int value=0;
	int valuea=charhextointa(dsi[0]);
	int valueb=charhextointa(dsi[1]);
	if (valuea<0 || valueb<0)
		return -1;
	else
	{
		value=valuea*16;
		value=value+valueb;
	}
	return value;
}

int Cfns::find32(char *strfnd, int pos32)
{
	//ejemplo:string hola 
	//pos32=1
	int mow=0;
	int i=0;
	int n=(int)strlen(strfnd);
	while(mow<n && pos32!=i)
	{
		if (strfnd[mow]==32 || strfnd[mow]==10 || strfnd[mow]==13 || strfnd[mow]==0)
			i++;
		mow++;
	}
	return mow;
}
void Cfns::filterstring(char *strfil)
{
	//Modify the data to be redeable by interpreter...

	//First, reduce 32 characters at commencing...
	while (strfil[0]==32)
		strcpy(strfil,strfil+1);
	//upstring the first word...
	int r=0;
	int t=(int)strlen(strfil);
	while(r<t && strfil[r]!=32 && r<512)
	{
		char mox[1];
		mox[0]=strfil[r];
		strupr(mox);
		strfil[r]=mox[0];
        r++;		
	}

}
void Cfns::chrcat(char *strc, char add)
{
	int m=(int)strlen(strc);
	strc[m]=add;
	strc[m+1]=0;
}
void Cfns::denter(char *crecpt)
{
	int ste=(int)strlen(crecpt);
	while (ste>=0)
	{
		if (crecpt[ste]==10 || crecpt[ste]==13)
			crecpt[ste]=0;
		ste--;
	}
}
void Cfns::dep32(char *depu)
{
	char outstr[512]="";
	int mos=(int)strlen(depu);
	int i=0;
	int u=0;
	char m=0;
	while (i<mos)
	{
		if (m==32 && depu[i]==32)
		{
		}
		else
		{
			outstr[u]=depu[i];
			u++;
		}
		m=depu[i];
		i++;
	}
	outstr[u]=0;
	strcpy(depu,outstr);
}
void Cfns::renter(char *crecpt)
{
	char ncrec[512];
	memset(ncrec,0,512);
	int ste=(int)strlen(crecpt);
	int u=0;
	for (int i=0; i<ste;i++)
	{
        ncrec[u]=crecpt[i];
		if (crecpt[i]=='\n')
		{
			u++;
            ncrec[u]='\r';
		}
		u++;
	}
	strcpy(crecpt,ncrec);
}
char *Cfns::deparg(char *depu, char *arg, BOOL nax)
{
	//nax is an alone tag
	//first step... search argument position
	// se busca arg dentro de depu, entonces, si es encontrado, 
	// y nax es falso, devuelve el string que hay luego...
	// si nax es verdadero, devuelve ""
	// si nax es verdadero, pero 
	char *pdest;
	int  result, r, mw;
	char res[512];
	char newln[512];
	char rspnl[5];
	strcpy(rspnl,arg);

	pdest = strstr( depu, arg );
	if (pdest==NULL)
	{
		strcpy(returned,rspnl);
		return returned;
	}

	result = (int) (pdest - depu) ;

	// result contain a position of arg...
	strcpy(res,depu+result);
	if (nax)
		r=find32(res,1);
	else
		r=find32(res,2);

	res[r-1]=0;
	mw=(int)strlen(res);

	//now res contain an argument... will now dep original string
	//depuring res:
	strcpy(res, res+strlen(arg)+1);
	//depuring depu:
	strcpy(newln,depu);
	newln[result]=0;
	strcat(newln,depu+result+mw+1);
	strcpy(depu,newln);

	strcpy(returned,res);
	return returned;
}

char *Cfns::convert(char *instr, con_v mx[SERVER_CONNECTIONS], int xlogon)
{
	char regret[4096];
	memset(&regret,0,4096);
	int w=strlen(instr);
	int i=0;
	while (i<w)
	{
        if (instr[i]=='$')
		{
			//Convert the format to string
			i++;
			if (instr[i]=='$')
				chrcat(regret,'$');
			if (instr[i]=='s')
				chrcat(regret,32);
			if (instr[i]=='C')
				chrcat(regret,033);
			if (instr[i]=='d')
			{
				char buff[_MAX_PATH];
			   /* Get the current working directory: */
				_getcwd( buff, _MAX_PATH );
				strcat(regret,buff);
			}
			if (instr[i]=='x')
			{
				char muh[2];
				memset(&muh,0,2);
				muh[0]=instr[i+1];
				muh[1]=instr[i+2];
				i=i+2;
				int vl=charhextoint(muh);
				if (vl>=0 && vl<256)
                    chrcat(regret,(char)vl);
			}
			if (instr[i]=='u')
			{
				//username...
				strcat(regret,mx[xlogon].c_User);
			}
			if (instr[i]=='n')
			{
				chrcat(regret,10);
				chrcat(regret,13);
			}
		}
		else
		{
			if (instr[i]!=10 && instr[i]!=13)
				chrcat(regret,instr[i]);
		}
		i++;
	}
	strcpy(returned,regret);
	return returned;
}


char *Cfns::depstring(char *dep)
{
	int mx=(int)strlen(dep);
	while(mx>0)
	{
		if (dep[mx]==32)
			dep[mx]=0;
		mx--;
	}
	return (dep);
}

BOOL Cfns::cmpfirstword(char *WordVariable,char *WordStatic)
{
    char meta[1024];
	BOOL rsp=FALSE;
	int r=0;
	int t=strlen(WordVariable);
	memset(&meta,0,1024);
	while(r<t && WordVariable[r]!=32 && r<512 && WordVariable[r]!=13 && WordVariable[r]!=10)
	{
		meta[r]=WordVariable[r];
        r++;
	}
    if(!strcmp(meta,WordStatic))
		rsp=TRUE;
	else
		rsp=FALSE;

	return rsp;
}


time_t Cfns::TimetFromFt(FILETIME pft) 
{
 	unsigned long h,l,pl;
 	unsigned long dh,dl,r,t;
 
 	/* Subtract offset from 1601 to 1970 in 100nsec */
 	h = pft.dwHighDateTime;
 	pl = l = pft.dwLowDateTime;
 	h += 0xfe624e9e;
 	l += 0xe4fbaeb0;
 	if (l < pl)	// carry
 		h++;
 
 	if (h >= 10000000)	/* Oops! */
 		return (time_t)-1;	/* Error */
 	
 	/* Truncate from 100nsec to 1 second using long division */
 	dh = 10000000 >> 1;		/* Divisor * 0x80000000 */
 	dl = 10000000 << 31;
 	for (t = 0, r = 0x80000000; r != 0; r >>= 1) {
 		if (dh < h || (dh == h && dl < l)) {	/* If dividor is less than dividend */
 			t |= r;	/* Accumulate result */
   			pl = l; /* Subtract divisor */
 			h -= dh;
 			l -= dl;
 			if (l > pl)	// carry
 				h--;
 		}
 		dl >>= 1;	/* Divide divisor by 2 */
 		dl |= (dh << 31);
 		dh >>= 1;
 	}
 	if (l >= 5000000)	/* Round */
 		t++;
 
 	return (time_t) t;
}
char *Cfns::md5sum(char *value)
{
	strcpy(returned,CMD5Checksum::GetMD5((BYTE*)value,strlen(value)));
	return returned;
}
