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
/*
			1. Como se va a manejar una conexion?

				Inicialmente se debe cambiar el modo de interpretación de URCS
				a modo -TCP Bouncing-, para ello, enviaremos el comando:
                
					send(proxyUsed,"TCPBOUNCE",80,0); 

				De esta forma, el ambiente normal del proxy, pasará a ser un nuevo
				ambiente de interpretación con un protocolo bien definido.

				El servidor debera responder:
					
					send(proxyUsed,"OKBOUNCE",80,0); 

				En caso de que el bouncer este trabajando.

			2. Inicialización de la conexión:

				Básicamente, esto corresponde a la inicialización que debe tener el
				bouncer como tal. El TCP-Bouncer, debe saber a donde conducirá este
				tunnel, de modo que se debe enviar cierta información referente a 
				ello:

					-2 bytes correspondientes al unsigned short del remote port.
					-1 byte correspondiente al tamaño del DNS (n)
					-n bytes correspondientes al DNS remoto.
				

				Una vez ensamblado el paquete, y enviado el paquete de inciialización,
				se debe recibir un ACK de que se recibio correctamente la inicialización
				del TCP bouncer. luego de ello, el sistema estará listo para el intercambio de
				paquetes.

				la estructura del ACK sera:

					send(proxyUsed,"INTL",80,0); 
					
			3. Comandos de conexión, streaming.

				El siguiente protocolo funcionará de forma de streams encapsulados.
				
				-1 byte de flags:
					XXXX:00X1 : Comenzar una conexión (SYN)
					XXXX:0101 : SYN Aknowledge (SYN/ACK)
					XXXX:010X : Agragar data a una conexión (PSH)
					XXXX:001X : Pedir terminar una conexión (FIN/PSH)
					XXXX:1000 : Keep Alive (KAL)
					XXXX:1010 : Terminar todas las conexiones.
					XXXX:       Reserved area.
					XXX1:001X : Pedir replica de terminación de la conexión (FIN/ACK)
					XX10:001X : Pedir terminación inmediata de la conexión (FIN)


				-4 bytes correspondientes al identificador de conexión
				-2 bytes del tamaño de la data (n)
				-n bytes de datos

				a. El SYN/ACK tendrá una especificación distinta, ya que en el identificador
				   SYN se le envia el identificador del cliente, y la respuesta debe contenerlo
				   como referencia para saber que conexion se usara, se le sustituye los (n) bytes de
				   data por 4 bytes obligatorios respondiendo con el identificador del cliente.
*/


#include "StdAfx.h"
#include ".\bouncing.h"
#include "Server.h"

CUserver server;

typedef struct LISTENNER
{
	SOCKET Proxy;
	SOCKET rProxy;
	SOCKET TheProxy;
	char rcon[512];
	unsigned short rport;
} LISTENNER;


typedef struct DISPATCHER
{
	SOCKET sProxy;
	SOCKET srProxy;
	SOCKET TheProxy;
	bool bProtected;
	bool connection_actived;
	unsigned short nAcumuladoSize;
	char cAcumulado[4000];
} DISPATCHER;


typedef struct MANAGER
{
	SOCKET ConnectionFromClient;
	SOCKET ConnectionToURCSProxie;

} MANAGER;

Cbouncing::Cbouncing(void)
{
}
Cbouncing::~Cbouncing(void)
{
}
UINT KALDispatcher(LPVOID pParam)
{
	// Abrir el manejador de buffer aqui.
	DISPATCHER*	ts = (DISPATCHER*)pParam;
	// Aqui se aceptan conexiones.
	// Que ocurre si se va
	char header[7];
	header[0]=0x8;
    
	while (send(ts->TheProxy,header,7,0)==7)
	{
		Sleep(30000);
	}
	return 1;
}

UINT reverseConnectionDispatcher(LPVOID pParam)
{
	// Abrir el manejador de buffer aqui.
	DISPATCHER*	ts = (DISPATCHER*)pParam;
	// Aqui se aceptan conexiones.
	// Que ocurre si se va
	while (!(!ts->connection_actived && ts->nAcumuladoSize==0))
	{
		Sleep(380);
		while (ts->bProtected) {}
		ts->bProtected=true;
		if (ts->nAcumuladoSize>0)
		{
			char header[7];
			header[0]=0x4;
			memcpy(header+1,&ts->srProxy,4); //Se debe decirle que se le haga llegar al cliente correcto.
			memcpy(header+5,&ts->nAcumuladoSize,2);
			
			// Aqui se envia lo que recibimos del remote endpoint por la via del proxy hasta el cliente
			// Al proxy se le debe enviar el comando de enviar esta información al socket especificado.
			send(ts->TheProxy,header,7,0); 
			send(ts->TheProxy,ts->cAcumulado,ts->nAcumuladoSize,0);
			ts->nAcumuladoSize=0;
		}
		ts->bProtected=false;
   	}
	// Se debe finalizar la conexion del otro lado.
	char headerx[7];
	headerx[0]=0x2;
	memcpy(headerx+1,&ts->srProxy,4); //Se debe decirle que se le haga llegar al cliente correcto.
	headerx[5]=0x0;
	headerx[6]=0x0;
	send(ts->TheProxy,headerx,7,0);
	return 1;
}
UINT reverseConnection(LPVOID pParam)
{
	// Abrir el manejador de buffer aqui.
	LISTENNER*	ts = (LISTENNER*)pParam;
	// Aqui se aceptan conexiones.
	DISPATCHER *_param = new DISPATCHER;
	_param->nAcumuladoSize=0;
	_param->connection_actived=true;
	_param->sProxy=ts->Proxy;      // SOCKET de conexion con el remoteEndpoint. 
	_param->srProxy=ts->rProxy;    // SOCKET de conexion con el cliente (no se debe usar)
	_param->TheProxy=ts->TheProxy; // SOCKET de conexion con el nodo intermedio (URCSBP protocol)
	_param->bProtected=false;
	AfxBeginThread(reverseConnectionDispatcher,_param);

	char receptor[1];
	while (recv2(ts->Proxy,receptor,1,0)==1) //Recibir datos del Endpoint y meterlos en el buffer para que el dispatcher lo envie al otro proxy
	{
		while (_param->bProtected) {}
		_param->bProtected=true;
		_param->cAcumulado[_param->nAcumuladoSize]=255-receptor[0];
		_param->nAcumuladoSize++;
		if (_param->nAcumuladoSize == 2048)
		{
			//Despachar automaticamente.
			char header[7];
			header[0]=0x4;
			memcpy(header+1,&_param->srProxy,4); //Se debe decirle que se le haga llegar al cliente correcto.
			memcpy(header+5,&_param->nAcumuladoSize,2);
			
			// Aqui se envia lo que recibimos del remote endpoint por la via del proxy hasta el cliente
			// Al proxy se le debe enviar el comando de enviar esta información al socket especificado.
			send(_param->TheProxy,header,7,0); 
			send(_param->TheProxy,_param->cAcumulado,_param->nAcumuladoSize,0);
			_param->nAcumuladoSize=0;
		}
		_param->bProtected=false;
		//while (_param->nAcumuladoSize==2048) {} //Esperar a que el dispatcher vacie el buffer.
   	}
	_param->connection_actived=false;

	// Se debe esperar a que todo el buffer haya sido transmitido para finalizar una posible conexion.
	
	// para que enviar 2 closesockets?
	while (_param->nAcumuladoSize!=0) {} //Esperar a que el dispatcher vacie el buffer.
	return 1;
}



UINT manageConnection(LPVOID pParam)
{
	// Abrir el manejador de buffer aqui.
	MANAGER*	ts = (MANAGER*)pParam;
	// Comenzar un Keep-Alive cada 30 segundos.
	DISPATCHER *_param = new DISPATCHER;
	_param->TheProxy=ts->ConnectionToURCSProxie; // SOCKET de conexion con el nodo intermedio (URCSBP protocol)
	AfxBeginThread(KALDispatcher,_param);

	char tcpbounceheader[7];
	while (recv2(ts->ConnectionToURCSProxie,tcpbounceheader,7,0)==7)
	{
		if ((tcpbounceheader[0]&0x0F)==0x0A) return 2; //XXXX:1010 terminar de aceptar paquetes.
		if ((tcpbounceheader[0]&0x0C)==0x04)		   // XXXX:01XX Verificar si hubo push.
		{
			SOCKET sockUsed;	
			unsigned short datalenght;
			memcpy(&sockUsed,tcpbounceheader+1,4);
			memcpy(&datalenght,tcpbounceheader+5,2);
			if (datalenght>0 && datalenght<4096)
			{
				//Recibir la data que fue especificada.
				char data_temp[4096];
				recv2(ts->ConnectionToURCSProxie,data_temp,datalenght,0);
				for (unsigned short k=0;k<datalenght;k++) data_temp[k]=255-data_temp[k];
				//Enviarla al socket especificado.
				send(sockUsed,data_temp,datalenght,0);
			}
		}
		if ((tcpbounceheader[0]&0x0F)==0x05) // XXXX:0101 SYN/ACK
		{
			// Se recibe el SYN/ACK de una conexión, se debe abrir un receptor
			// local para enviar la data requerida
			LISTENNER *_param = new LISTENNER;
	        SOCKET remoteSocket;
			memcpy(&remoteSocket,tcpbounceheader+1,4); // Se recibe el remote socket.
			char client_id[4];
			recv2(ts->ConnectionToURCSProxie,client_id,4,0); // Se recibe el cliente usado.
			memcpy(&_param->Proxy,client_id,4); //Contiene la nueva conexión realizada con el cliente.
			_param->TheProxy = ts->ConnectionToURCSProxie; //Contiene la conexión realizada el proxy (por donde viajan los datos)
			_param->rProxy = remoteSocket; // Contiene el socket destination en el otro proxy. (no se debe usar)
			AfxBeginThread(reverseConnection,_param); // Se reciben los datos del cliente y se envian al proxy que lo enviara al remoteEndpoint.
		}
		if ((tcpbounceheader[0]&0xFE)==0x22) // 0010:001X Terminar una conexión.
		{
			SOCKET stab;
			memcpy(&stab,tcpbounceheader+1,4);
			WSASendDisconnect(stab,NULL);
//			closesocket(stab);
		}
		else if ((tcpbounceheader[0]&0xFE)==0x12) // 0001:001X Responder la replica de una terminación para que se termine de verdad
		{
            tcpbounceheader[0]=tcpbounceheader[0]+0x10;
			send(ts->ConnectionToURCSProxie,tcpbounceheader,7,0);
		}
		else if ((tcpbounceheader[0]&0xFE)==0x02) // 0000:001X Pedir replica de una terminación
		{
            tcpbounceheader[0]=tcpbounceheader[0]+0x10;
			send(ts->ConnectionToURCSProxie,tcpbounceheader,7,0);
        }

	}
	return 1;
}
UINT acceptRemoteConnections(LPVOID pParam)
{
	// Abrir el manejador de buffer aqui.
	LISTENNER*	ts = (LISTENNER*)pParam;
	// Aqui se aceptan conexiones.
	MANAGER *_param = new MANAGER;
	_param->ConnectionToURCSProxie = ts->Proxy;
	AfxBeginThread(manageConnection,_param); //Despacharle la conexion entrante a manageConnection
	int mant=7;
	while (mant==7)
	{
		char ip[128];
		_param->ConnectionFromClient = server.s_accept(ip);
        if (_param->ConnectionFromClient==0) return -1;
		// Se debe enviar los comandos proxy para que el sistema regrese los valores de inicialización.

		// Una nueva conexión entrante, se debe enviar los comandos para comenzar 
		// una nueva conexión saliente desde el proxy y pasar a los manejadores de buffer.
		char header[7];
		header[0]=0x1; //SYN
		memcpy(header+1,&_param->ConnectionFromClient,4);
		header[5]=0x0; 
		header[6]=0x0; 
		mant=send(_param->ConnectionToURCSProxie,header,7,0);
		//Una vez enviado los comandos de conexion, se esta listo para recibir nueva conexion.
		/*if (xverbose)
		{
			char newcon[512];
			_snprintf(newcon,512,"TCP FORWARDING: %s->%s:%u",ip,ts->rcon,ts->rport);
			MessageBox(NULL,newcon,"connection",0);
			printf("%s",newcon);
		}*/

	}
	return 1;
}

int Cbouncing::listener(unsigned short localPort, 
						SOCKET proxyUsed, 
						char *remoteEndPoint, 
						unsigned short remotePort)
{
	//////////////////////////////////////////////////////
	//////// Start proxy command for RSocket     /////////
	//////////////////////////////////////////////////////

	// 0: No imprime nada.
	// 1: Imprime estado de paquetes enviados / Conexiones
	// 2: Imprime contenido de los paquetes enviados/recibidos en formato HEX.

	if (server.startserver(localPort)<=0) return -1; //Cannot listen
	// Hacerle detach al listener en otro proceso.
	char prx_used[80]="TCPBOUNCE";
	send(proxyUsed,prx_used,80,0); 
	if (recv2(proxyUsed,prx_used,80,0)<80 || strcmp(prx_used,"OKBOUNCE"))
	{
		server.closeserver();
        return -2;
	}
	// Se envia los parametros iniciales de la conexion:
	// como fueron especificados en la especificación del protocolo.
	char initial_connection[260];
	char x = (char)strlen(remoteEndPoint);
	memcpy(initial_connection,&remotePort,2);
	initial_connection[2]=x;
	strncpy(initial_connection+3,remoteEndPoint,x);
	send(proxyUsed,initial_connection,x+3,0);
	// Se recibe un ACK
	if (recv2(proxyUsed,prx_used,80,0)<80 || strcmp(prx_used,"INTL"))
	{
		server.closeserver();
        return -3;
	}
	// La conexión esta lista para pasar a la subtarea.

	LISTENNER *_param = new LISTENNER;
	_param->Proxy = proxyUsed;
	strncpy(_param->rcon,remoteEndPoint,512);
	_param->rport=remotePort;
    AfxBeginThread(acceptRemoteConnections,_param);
	return 1;
}

int Cbouncing::proxy_manager(SOCKET proxyUsed)
{
	// Se supone que ya se envio el comando OKBOUNCE y que se tiene acceso
	// a ser proxy.

	// Obtener los primeros 3 bytes 
	char header[3];
	char x;
	recv2(proxyUsed,header,3,0);
	memcpy(&gremotePort,header,2);
	x=header[2];
	memset(&gremoteEndPoint,0,sizeof(gremoteEndPoint));
	recv2(proxyUsed,gremoteEndPoint,x,0);
	// Los datos estan llenos, enviamos la señal ACKNOWLEDGE INTL
	send(proxyUsed,"INTL",80,0);
	// Nuestro servidor esta listo para interpretar aqui todo lo que nos envien y reenviarlo
	// Al destinatario.

	DISPATCHER *_param = new DISPATCHER;
	_param->TheProxy=proxyUsed; // SOCKET de conexion con el nodo intermedio (URCSBP protocol)
	AfxBeginThread(KALDispatcher,_param);

	//Aqui recibir las conexiones.
	char tcpbounceheader[7];
	while (recv2(proxyUsed,tcpbounceheader,7,0)==7)
	{
		if ((tcpbounceheader[0]&0x0F)==0x0A) return 2; //XXXX:1010 terminar de aceptar paquetes.
		if ((tcpbounceheader[0]&0x0D)==0x01) //XXXX:00X1 comenzar una conexion
		{
			struct hostent *hentry;
			struct sockaddr_in Server_add;

			SOCKET Sock = socket( AF_INET, SOCK_STREAM, 0 ); //open socket
			// Connect Server
			if ((hentry = gethostbyname(gremoteEndPoint)) == NULL) 
			{
				char resp[7];
				resp[0]=0x2; //FIN
				memcpy(resp+1,tcpbounceheader+1,4); // Socket del cliente (no se debe usar)
				resp[5]=0;
				resp[6]=0;
				send(proxyUsed,resp,7,0); //Se finaliza la conexion con el host.
				continue;
			}
			else
			{
				// Make Socket
				memset((char *)&Server_add, 0, sizeof(Server_add));
				memcpy(&Server_add.sin_addr.s_addr, *(hentry->h_addr_list),sizeof(Server_add.sin_addr.s_addr));
				//host are copied to Server_add
				Server_add.sin_port = htons(gremotePort);
				Server_add.sin_family = AF_INET;
				// Connect Server
				if( connect( Sock, (struct sockaddr *) &Server_add, sizeof( Server_add ) ) == 0 )
				{
                    // Una vez inicializado el socket, enviar el socket inmediatamente como ACK
					char resp[7];
					char resp_pb[4];
					memcpy(resp_pb,tcpbounceheader+1,4); // Socket del cliente (no se debe usar)
					resp[0]=0x5; //SYN/ACK
					memcpy(resp+1,&Sock,4);
					resp[5]=0;
					resp[6]=0;
					send(proxyUsed,resp,7,0); //Se manda un acknowledge.
					send(proxyUsed,resp_pb,4,0); //Se manda el cliente que fue usado para pedir el acknowledge.
					// Se abre un proceso para la recepción inversa.
					LISTENNER *_param = new LISTENNER;
					_param->Proxy = Sock; //Contiene la nueva conexión realizada con el remoteEndpoint.
					_param->TheProxy = proxyUsed; //Contiene la conexión realizada el proxy (por donde viajan los datos)
					memcpy(&_param->rProxy,tcpbounceheader+1,4); // Socket del cliente (no se debe usar)
					AfxBeginThread(reverseConnection,_param);
				}
			}
		}
		
		if ((tcpbounceheader[0]&0x0C)==0x04) // XXXX:01XX Verificar si hubo push.
		{
			SOCKET sockUsed;
			unsigned short datalenght;
			memcpy(&sockUsed,tcpbounceheader+1,4);
			memcpy(&datalenght,tcpbounceheader+5,2);
            if (datalenght>0 && datalenght<4096)
			{
				//Recibir la data que fue especificada.
				char data_temp[4096];
				recv2(proxyUsed,data_temp,datalenght,0);
				//Enviarla al socket especificado.
				for (unsigned short k=0;k<datalenght;k++) data_temp[k]=255-data_temp[k];
				send(sockUsed,data_temp,datalenght,0);
			}

		}
		if ((tcpbounceheader[0]&0xFE)==0x22) // 0010:001X Terminar una conexión.
		{
			SOCKET stab;
			memcpy(&stab,tcpbounceheader+1,4);
			WSASendDisconnect(stab,NULL);
//			closesocket(stab);
		}
		else if ((tcpbounceheader[0]&0xFE)==0x12) // 0001:001X Responder la replica de una terminación para que se termine de verdad
		{
            tcpbounceheader[0]=tcpbounceheader[0]+0x10;
			send(proxyUsed,tcpbounceheader,7,0);
		}
		else if ((tcpbounceheader[0]&0xFE)==0x02) // 0000:001X Pedir replica de una terminación
		{
            tcpbounceheader[0]=tcpbounceheader[0]+0x10;
			send(proxyUsed,tcpbounceheader,7,0);
        }

	}
	return 1;
}

void Cbouncing::proxy_deny(SOCKET proxyUsed)
{
	send(proxyUsed,"NOBOUNCE",80,0);
}
void Cbouncing::proxy_accept(SOCKET proxyUsed)
{
    send(proxyUsed,"OKBOUNCE",80,0);
}
