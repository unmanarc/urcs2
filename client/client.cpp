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
#include "client.h"
#include "cliente.h"
#include "conio.h"
#include "Consola.h"
#include "MD5Checksum.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;



void getpass(char *pass, int tm)
{
	memset(pass,0,tm);
	int i=0,ch=0;
	while ( i<tm && ch!='\n' && ch!='\r')
	{
		ch = _getch();
		printf("*");
		pass[i] = (char)ch;
		i++;
	}
}



int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
		char s_ip[256];
		char s_port[20];
		unsigned int port;
		// Check for arguments
		if (argc<2)
		{
            //none args...
			strcpy(s_ip,"127.0.0.1");
			strcpy(s_port,"3360");
		}
		else
		{
			// for args, >=2 
			if (argc<3)
			{
				// for 1 arg (ip)
				strncpy(s_ip,argv[1],256);
				strcpy(s_port,"3360");
			}
			else
			{
				// for 2 or more args.
				strncpy(s_ip,argv[1],256);
				strncpy(s_port,argv[2],20);
			}
		}
		port=atoi(s_port);
		//Args completed-...
		printf("Connecting to: %s, at port %u\n",s_ip,port);
/*
	--------------> The client start here

*/
		//body of client-->
		CUclient cln;
		if (cln.connectto(s_ip,port)>0) //Receive socket to communicate to server
		{
			//Start communication.
			int t=cln.start_client();
			if (t==1) //Started communication successfully
			{
				char bf[4096]; //buffer
				int tm=1; // lenght of raw data received.
				while(tm>=0) //loop receive data
				{
					tm=cln.receive(bf);
					if (tm<=4096) //mode for print data
						printf(bf);
					if (tm==4097) //mode for request line
					{
						fgets(bf,4096,stdin); //get line
						tm=cln.senddata(bf); // Send a null-terminated string to server
					}
					if (tm==4098) // mode for request password
					{
		                char line[512];
						memset(&line,0,512);
						getpass(line,512);
						tm=cln.senddata(line);
					}
					if (tm==4100) // mode for request password merged.
					{
		                char line[512];
						char hash[256],germen[128]="";

						memset(&hash,0,256);
						memset(&germen,0,128);
						memset(&line,0,512);
						getpass(line,512);
						cln.receive(germen);
						
						line[strlen(line)-1]=0;
						sprintf(hash,"%s%s",
								CMD5Checksum::GetMD5((BYTE*)line,(UINT)strlen(line)),
                                germen);
						strcpy(hash,CMD5Checksum::GetMD5((BYTE*)hash,(UINT)strlen(hash)));
						tm=cln.senddata(hash);
					}
					if (tm==5000) //mode for request line
					{
						clrscr();
					}
					if (tm==5001) //mode for request file
					{
						tm=cln.recvfile();
					}
					if (tm==5002) //mode for request file
					{
						tm=cln.sendfile();
					}
					// from 5003-5019 (text color)
					if (tm<5020 && tm>5002)
					{
						//color...
						int color=tm-5003;
						textcolor(color);
					}
					// from 5020-5036 (text color)
					if (tm>=5020 && tm<=5036)
					{
						//color...
						int color=tm-5020;
						textbackground(color);
					}
					if (tm==5037) //Borrar hasta el final de l�nea
						clreol();
					if (tm==5038) //Eliminar la l�nea actual
						delline();
					if (tm==5039) //Insertar una nueva l�nea
						insline();
					if (tm==5040) //Set Windows Title
					{
						tm=cln.receive(bf);	//Receive title
                        settitle(bf);
					}
					//from 6000 to 8000 is for x,y...
					if (tm>=6000 && tm<=8000) //poner posici�n XY
					{
						tm=tm-6000;
						int y=tm/80;
						int x=tm%80;
						gotoxy(x+1,y+1);
					}
				}
				cln.closesock();
            }
			else
			{
				if(t==-1)
				{
					fprintf(stderr,"error: server closes communication\n"); //End with... cannot connect to server
					return -10;
				}
				if(t==-2)
				{
					fprintf(stderr,"incompatible protocol or socket error\n");
					return -11;
				}
				if(t==-3)
				{
					fprintf(stderr,"Server is full\n");
					return -12;
				}
			}
		}
		else
			fprintf(stderr,"error: cannot connect to server\n"); //End with... cannot connect to server
	}
	return nRetCode;
}
