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




Cfns::Cfns()
{ //Constructor
}
Cfns::~Cfns()
{ //destructor
}

char *Cfns::extview(char *filename)
{
	int lg=strrpointer(filename, '.');
	//search back for .
	strncpy(returned,filename+lg+1,12);
	returned[12]=0;
	return returned;
}
long Cfns::GetFreeDiskSpaceInKB(LPSTR pFile) 
{     
	DWORD dwFreeClusters, dwBytesPerSector, dwSectorsPerCluster, dwClusters;    
	char RootName[MAX_PATH];     
	LPSTR ptmp;    //required arg     
	ULARGE_INTEGER ulA, ulB, ulFreeBytes;  
	GetFullPathName(pFile, sizeof(RootName), RootName, &ptmp);
	if (RootName[0] == '\\' && RootName[1] == '\\') 
	{  
		ptmp = &RootName[2]; 
		while (*ptmp && (*ptmp != '\\')) 
		{     
			ptmp++; 
		} 
		if (*ptmp) 
		{     // advance past the third backslash     
			ptmp++; 
		}     
	} 
	else 
	{ 
		// path must be drv:\path 
		ptmp = RootName;     
	}      
	while (*ptmp && (*ptmp != '\\')) 
	{ 
		ptmp++;     
	}     // found a backslash ?     
	if (*ptmp) 
	{ // skip it and insert null ptmp++; 
		*ptmp = '\0';     
	}
	HINSTANCE h = LoadLibraryA("kernel32.dll");     
	if (h) 
	{ 
		typedef BOOL (WINAPI *MyFunc)(LPCSTR RootName, PULARGE_INTEGER pulA, PULARGE_INTEGER pulB, PULARGE_INTEGER pulFreeBytes); 
		MyFunc pfnGetDiskFreeSpaceEx = (MyFunc)GetProcAddress(h, "GetDiskFreeSpaceExA"); 
		FreeLibrary(h); 
		if (pfnGetDiskFreeSpaceEx) 
		{     
			if (!pfnGetDiskFreeSpaceEx(RootName, &ulA, &ulB, &ulFreeBytes)) 
				return -1;     
			return (long)(ulFreeBytes.QuadPart / 1024); 
		}     
	}      
	if (!GetDiskFreeSpace(RootName, &dwSectorsPerCluster, &dwBytesPerSector, &dwFreeClusters, &dwClusters)) 
		return (-1);     

	return(MulDiv(dwSectorsPerCluster * dwBytesPerSector,    dwFreeClusters,    1024)); 
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
	size_t point=(int)strlen(pointer);
	for(size_t po=0;po<point;po++)
	{
		if (pointer[po]==pnt) return (int)po;
	}
	return -1;
}

int Cfns::strrpointer(char *pointer, char pnt)
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
int Cfns::filterstring(char *strfil)
{
	//Modify the data to be redeable by interpreter...

	//First, reduce 32 characters at commencing...
	while (strfil[0]==32 && strfil[0]!=0)
		strcpy(strfil,strfil+1);
	//upstring the first word...
	int r=0;
	int t=(int)strlen(strfil);
	while(r<t && strfil[r]!=32 && r<COMMAND_LINE) //replace first word to upstring
	{
		char mox[2];
		mox[1]=0;
		mox[0]=strfil[r];
		strupr(mox);
		strfil[r]=mox[0];
        r++;		
	}
	return 0;
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

void Cfns::denter2(char *crecpt)
{
	int ste=(int)strlen(crecpt);
	int summer=0;
	for (int u=0;u<=ste;u++)
	{
		if (crecpt[u]!=10 && crecpt[u]!=13)
		{
			crecpt[summer]=crecpt[u];
			summer++;
		}
	}
}
void Cfns::dep32(char *depu,size_t depusize)
{
	char outstr[512]="";
	size_t mos=strlen(depu),i=0,u=0,m=0;
	while (i<mos && u<sizeof(outstr))
	{
		if (m==32 && depu[i]==32){ } //Do nothing.
		else
		{
			outstr[u]=depu[i];
			u++;
		}
		m=depu[i];
		i++;
	}
	outstr[u]=0;
	strncpy(depu,outstr,depusize);
}
void Cfns::renter(char *crecpt,size_t sd)
{
	char ncrec[512];
	memset(ncrec,0,512);
	int ste=(int)strlen(crecpt);
	int u=0;
	for (int i=0;i<ste && u<sizeof(ncrec) ;i++)
	{
        ncrec[u]=crecpt[i];
		if (crecpt[i]=='\n')
		{
			u++;
            ncrec[u]='\r';
		}
		u++;
	}
	strncpy(crecpt,ncrec,sd);
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
	char res[COMMAND_LINE];
	char newln[COMMAND_LINE];
	char rspnl[5];

	strncpy(rspnl,arg,5);
	pdest = strstr( depu, arg );
	if (pdest==NULL)
	{
		strncpy(returned,rspnl,COMMAND_LINE); //devolver el argumento si no encuentra nada...
		return returned;
	}
	result = (int) (pdest - depu) ;
	// result contain a position of arg...
	strncpy(res,depu+result,COMMAND_LINE);
	if (nax)
		r=find32(res,1);
	else
		r=find32(res,2);

	if (res[r-1]==32)
		res[r-1]=0;
	else
		res[r]=0;
	mw=(int)strlen(res);
	//now res contain an argument... will now dep original string
	//depuring res:
	strncpy(res,res+strlen(arg)+1,sizeof(res));
	//depuring depu:
	strncpy(newln,depu,sizeof(newln));
	newln[result]=0;
	strncat(newln,depu+result+mw+1,sizeof(newln)-strlen(newln));
	strncpy(depu,newln,sizeof(depu));
	strncpy(returned,res,COMMAND_LINE);
	return returned;
}
char *Cfns::convert_vars(char *instr, con_v mx[SERVER_CONNECTIONS], int xlogon)//Transform % lang.
{
	char regret[COMMAND_LINE];
	memset(&regret,0,COMMAND_LINE);
	int w=(int)strlen(instr);
	int i=0;
	int o=0;
	while (i<w && o<COMMAND_LINE)
	{
		if (instr[i]=='#')
		{
			//Convert the format to string
			i++;
			if (instr[i]=='#')
			{
				regret[o]='#';
				o++;
			}
			else
			{
				//have var into... replacing...
				char nameofvar[128];
				int pty=0;
				strncpy(nameofvar,instr+i,128);
				pty=strpointer(nameofvar,'#');
				if (pty<0) //not end founded... releasing...
				{
					i--;
					if (instr[i]!=10 && instr[i]!=13)
					{
						regret[o]=instr[i];
						o++;
					}
				}
				else
				{
					int tmr=0;
					int u;
					i=i+pty;
					nameofvar[pty]=0;
					//searching for this name...
					tmr=mx[xlogon].m_mem.getmemsize(nameofvar); //get size
					u=tmr+o;
					if (tmr>0 && u<COMMAND_LINE)
					{
						strncat(regret,mx[xlogon].m_mem.getmem(nameofvar),tmr);
						o=u;
					}
				}
			}
		}
		else
		{
			if (instr[i]!=10 && instr[i]!=13)
			{
				regret[o]=instr[i];
				o++;
			}
		}
		i++;
	}
	regret[o]=0;
	strncpy(returned,regret,COMMAND_LINE);
	return returned;
}

char *Cfns::convert(char *instr, con_v mx[SERVER_CONNECTIONS], int xlogon)
{

	char regret[COMMAND_LINE];
	memset(&regret,0,COMMAND_LINE);
	int w=(int)strlen(instr);
	int i=0;
	int o=0;
	while (i<w && o<COMMAND_LINE)
	{
        if (instr[i]=='$')
		{
			//Convert the format to string
			i++;
			if (instr[i]=='$')
			{
				regret[o]='$';
				o++;
			}
			if (instr[i]=='s')
			{
				regret[o]=' ';
				o++;
			}
			if (instr[i]=='C')
			{
				regret[o]=33;
				o++;
			}
			if (instr[i]=='d')
			{
				char buff[_MAX_PATH];
			   /* Get the current working directory: */
				_getcwd( buff, _MAX_PATH );
				int u=o+(int)strlen(buff);
				if (u<COMMAND_LINE)
				{
					strncat(regret,buff,sizeof(regret)-strlen(regret));
					o=u;
				}
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
				{
					regret[o]=(char)vl;
					o++;
				}
			}
			if (instr[i]=='u')
			{
				//username...
				int u=o+(int)strlen(mx[xlogon].c_User);
				if (u<COMMAND_LINE)
				{
					strncat(regret,mx[xlogon].c_User,sizeof(regret)-strlen(regret));
					o=u;
				}
			}
			if (instr[i]=='n')
			{
				int u=o+2;
				if (u<COMMAND_LINE)
				{
					regret[o]=10;
					regret[o+1]=13;
					o=o+2;
				}
			}
		}
		else
		{
			if (instr[i]!=10 && instr[i]!=13)
			{
				regret[o]=instr[i];
				o++;
			}
		}
		i++;
	}
	strncpy(returned,regret,COMMAND_LINE);
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
bool Cfns::getboolarg(char *commandline, char *arg)
{
	//Search for argument.
	char strtsrch[16];
	strcpy(strtsrch," -");
	strncat(strtsrch,arg,13);
	size_t b=strlen(strtsrch);
	char *pdest = strstr(commandline,strtsrch);
	if (pdest==NULL) return false;
	else
	{
		//extract string from commandline. and return true
        memset(&returned,0,COMMAND_LINE);
		int a=0;
		a=(int)(pdest-commandline);
		if (commandline[a+b]!=' ' && commandline[a+b]!=0)
			return false;
		
        strncpy(returned,commandline,a);
		strncat(returned,commandline+a+b,sizeof(returned)-strlen(returned));
		strncpy(commandline,returned,strlen(commandline));
		return true;
	}
}
void Cfns::getzeroarg(char *commandline)
{
	size_t g=0;
	//buscar la posicion del primer /32
	while (g<strlen(commandline) && commandline[g]!=32)	g++;
	if (g==strlen(commandline)) strcpy(commandline,"");
	else strncpy(commandline,commandline+g+1,strlen(commandline));
}

char *Cfns::getchararg(char *commandline, char *arg)
{
	//Search for argument.
	char strtsrch[256];
	strcpy(strtsrch," -");
	strncat(strtsrch,arg,253);
	size_t b=strlen(strtsrch);
	char *pdest = strstr(commandline,strtsrch);
	if (pdest==NULL) 
		return "";
	else
	{
		//extract string from commandline. and return true
        memset(&returned,0,COMMAND_LINE);
		int a=0;
		a=(int)(pdest-commandline);
		if (commandline[a+b]!=' ')
			return "";
		static char displacement[COMMAND_LINE];
		strncpy(displacement,commandline+a+b+1,COMMAND_LINE);
		for (size_t g=0;g<COMMAND_LINE && g<strlen(displacement);g++)
		{
			if(displacement[g]==32)
				displacement[g]=0;
		}
        strncpy(returned,commandline,a);
		strcat(returned,commandline+a+b+strlen(displacement)+1);
		strncpy(commandline,returned,strlen(commandline));
		return displacement;
	}
	return "";
}

BOOL Cfns::cmpfirstword(char *WordVariable,char *WordStatic)
{
    char meta[COMMAND_LINE];
	BOOL rsp=FALSE;
	int r=0;
	int t=(int)strlen(WordVariable);
	memset(&meta,0,COMMAND_LINE);
	while(r<t && WordVariable[r]!=32 && r<COMMAND_LINE && WordVariable[r]!=13 && WordVariable[r]!=10)
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


char *Cfns::firstword(char *WordVariable)
{
    static char meta[COMMAND_LINE];
	BOOL rsp=FALSE;
	int r=0;
	int t=(int)strlen(WordVariable);
	memset(&meta,0,COMMAND_LINE);
	while(r<t && WordVariable[r]!=32 && r<COMMAND_LINE && WordVariable[r]!=13 && WordVariable[r]!=10)
	{
		meta[r]=WordVariable[r];
        r++;
	}
	return meta;
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
	//memset(&returned,0,COMMAND_LINE);
	strcpy(returned,CMD5Checksum::GetMD5((BYTE*)value,(UINT)strlen(value))); //checksum Md5 is 32 bytes in hex.
	return returned;
}
