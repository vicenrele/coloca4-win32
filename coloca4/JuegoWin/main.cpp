
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "resource.h"

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);  //Procedimiento de ventana 
BOOL CALLBACK DlgProc(	//Procedimiento de di�logo
    HWND,   // manipulador del cuadro de di�logo
    UINT,   // mensaje
    WPARAM, // primer par�metro del mensaje
    LPARAM  // segundo par�metro del mensaje
   );

int ganador(int );

struct _juego {
	int jugador;
	int tablero[7][7];
	int salir;
}juego;

struct _config {
	COLORREF colorFondo, colorP1, colorP2;
	int fondo;
	int actual;
	int elegirColor;
	int primera_partida;

} config;


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	//definici�n de los par�metros de la ventana Juego
	static char szAppName[] = "Juego" ;
	HWND        hwnd ;
	MSG         msg ;
	WNDCLASSEX  wndclass ;
	wndclass.cbSize        = sizeof (wndclass) ;
	wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = WndProc ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance     = hInstance ;
	wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;//////////
	wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
	wndclass.lpszMenuName  = MAKEINTRESOURCE (ID_MENU);   /////////
	wndclass.lpszClassName = szAppName;
	wndclass.hIconSm       = LoadIcon (NULL, IDI_ASTERISK) ;///////////

	//registro de la ventana
	RegisterClassEx (&wndclass) ;

	//creacion de la ventana
	hwnd = CreateWindow (szAppName,		// Nombre clase de ventana
					"Coloca 4",  // Barra del t�tulo
					WS_OVERLAPPEDWINDOW, // Estilo de ventana
					CW_USEDEFAULT,       // Posici�n x inicial
					CW_USEDEFAULT,       // Posici�n y inicial
					CW_USEDEFAULT,       // Tama�o x inicial
					CW_USEDEFAULT,       // Tama�o y inicial
					NULL,                // Handle de ventana padre
					NULL,                // Handle a men� de ventana
					hInstance,           // Handle de instancia
					NULL) ;              // Par�metros de creaci�n
	
	//Variables de necesaria inicializaci�n al principio
	config.actual=IDC_ROJAS_BLANCAS;
	config.elegirColor=1; //Empieza eligiendo color (scroll) el jugador 1
	config.primera_partida=0;  //La primera partida tiene los colores por defecto

	//Se muestra y repinta
	ShowWindow (hwnd, iCmdShow) ;
	UpdateWindow (hwnd) ;

	//Mensajes
	while (GetMessage (&msg, NULL, 0, 0))  //Devuelve si hay mensajes en la cola y los saca a msg, si es de tipo QUIT sale
	{         
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}          
	return msg.wParam ;  //Para los mensajes de error
}     

//procedimiento de la ventana
LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{     
     HDC			hdc;
	 PAINTSTRUCT	ps;
	 LPPOINT		*pt;
	 RECT			rect,rectaux;
	 HBRUSH			hbrush, hbrushRed, hbrushWhite;
	 HPEN			hpen;
	 int			ancho, alto;
	 int			i, j;
	 int			Xposition, Yposition;
	 int			opcion, msg;  
	 char			mens[100];


     switch (iMsg)
	 {

		  case WM_CLOSE:
				msg = MessageBox (hwnd, "�Desea cerrar coloca 4?   ", "Salir", MB_ICONWARNING | MB_OKCANCEL);
				if(msg != IDCANCEL){
					break;
				}
			  return 0;

		  case WM_CREATE: //La ventana lo recibe despu�s de ser creada pero antes de ser visible
			  
			  if(!config.primera_partida){  //Colores por defecto
				config.colorFondo=RGB(255, 255, 255);  //Blanco
		  
				config.colorP1=RGB(255, 0, 0); //Rojo jugador 1
				config.colorP2=RGB(255, 255, 255);  //Blanco jugador 2
			  }

			  
					for(i=0;i<7;i++){
						for(j=0;j<7;j++){
							juego.tablero[i][j]=0; //Posici�n vac�a del tablero
							
						}
					}

					for(j=0;j<7;j++){
							juego.tablero[6][j]=6; //Posici�n v�lida del tablero
							
					}

					juego.jugador=1;
					juego.salir=0;
				return 0;

		  case WM_PAINT:	//se recibe cuando se recibe una petici�n para pintar
				hdc = BeginPaint (hwnd, &ps);
				GetClientRect (hwnd, &rect);

				ancho = (rect.right-rect.left)/9;   //Divisi�n de la ventana en rect�ngulos 
				alto = (rect.bottom-rect.top)/9;

				for(i=1;i<8;i++){  //Solo para los rect�ngulos del centro
					for(j=1;j<8;j++){
						hbrush = CreateSolidBrush (RGB(0,0,255)); //Fondo azul
						SelectObject(hdc, hbrush);						  
						// 8 y 2 para las separaciones entre rect�ngulos.
						Rectangle (hdc,  (j*ancho)+8, (i*alto)+2,(j+1)*ancho, (i+1)*alto); //( ,Xizq, Yarrib, Xdcha, Yabaj)
						DeleteObject (hbrush);

					}
				}	

				for(i=1;i<8;i++){
					for(j=1;j<8;j++){
						if(juego.tablero[i-1][j-1]!=0)
						{
							if(juego.tablero[i-1][j-1]==1){	//jugador 1
								//FICHAS ROJAS								
								hbrushRed = CreateSolidBrush (config.colorP1); 
								SelectObject(hdc, hbrushRed);							 
								Ellipse (hdc,  (j*ancho)+14, (i*alto)+8,(j+1)*ancho-6, (i+1)*alto-6);
								DeleteObject (hbrushRed);

							}
							else if(juego.tablero[i-1][j-1]==2){	//jugador 2
								//FICHAS BLANCAS 
								hbrushWhite = CreateSolidBrush (config.colorP2); 
								SelectObject(hdc, hbrushWhite);
								Ellipse (hdc,  (j*ancho)+14, (i*alto)+8,(j+1)*ancho-6, (i+1)*alto-6);
								DeleteObject (hbrushWhite);
							}	
						}
					}
				}
				
				EndPaint (hwnd, &ps) ;
				return 0 ;

		  case WM_LBUTTONUP:	//click del boton izquierdo
				hdc = BeginPaint (hwnd, &ps);  //valida toda la ventana.
				GetClientRect (hwnd, &rect);
			    hdc = GetDC (hwnd) ;

				Xposition = LOWORD(lParam);  //lParam cambia dependiendo del mensaje, en este caso: WM_LBUTTONUP
				Yposition = HIWORD(lParam);  //parte alta de la palabra

				ancho = (rect.right-rect.left)/9;
				alto = (rect.bottom-rect.top)/9;
 
					for(i=1;i<8;i++){
						for(j=1;j<8;j++){
							if(Xposition>=((j*ancho)+8) && Yposition>=((i*alto)+2) && Xposition<=((j+1)*ancho) 
								&& Yposition<=((i+1)*alto) && (juego.tablero[i-1][j-1]==6) && juego.salir==0)
							{
								if(juego.jugador==1){	//jugador 1
									juego.tablero[i-1][j-1]=1;									
									if((i-2)>=0){juego.tablero[i-2][j-1]=6;} //Marca lugar v�lido "6" pero nunca fuera de la matriz
									InvalidateRect (hwnd,NULL,0);  //Invalida la regi�n (NULL:toda) del rect hasta que se vuelva a repintar cuando windows envie WM_PAINT
									if(ganador(juego.jugador)==TRUE){
										sprintf(mens, "Gana Jugador 1 (Premio: En configuraci�n puede crear un color para sus fichas)  ");
										MessageBox (hwnd, mens, "Enhorabuena!!", MB_ICONWARNING | MB_OK);
										config.elegirColor=1;
										config.primera_partida=1;
										juego.salir=1;
									}
									juego.jugador=2;
									return 0;
								}
								if(juego.jugador==2){	//jugador 2
									juego.tablero[i-1][j-1]=2;	
									if((i-2)>=0){juego.tablero[i-2][j-1]=6;}
									InvalidateRect (hwnd,NULL,0);
									if(ganador(juego.jugador)==TRUE){
										sprintf(mens, "Gana Jugador 2 (Premio: En configuraci�n puede crear un color para sus fichas)  ");
										MessageBox (hwnd, mens, "Enhorabuena!!", MB_ICONWARNING | MB_OK);
										config.elegirColor=2;
										config.primera_partida=1;  //A partir de aqu� no es primera partida. No se reinician los colores.
										juego.salir=1;
									}
									juego.jugador=1;
									return 0;
								}	
							}
						
						}
					}
							
				return 0;

		  case WM_COMMAND:
				opcion = LOWORD (wParam);

				switch (opcion) {
					case ID_JUEGO_SALIR:
						SendMessage (hwnd, WM_CLOSE, 0, 0);
						return 0;

					case ID_AYUDA_INTRUCCIONES:
						MessageBox (hwnd, "El jugador que antes haga una l�nea de 4 gana. Las fichas se colocan de abajo a arriba.   ",
									"Instrucciones", MB_ICONEXCLAMATION |MB_OK);
						return 0;

					case ID_JUEGO_NUEVO:						
						SendMessage (hwnd, WM_CREATE, 0, 0);				
						InvalidateRect (hwnd,NULL,0);
						return 0;

					case ID_JUEGO_CONFIG:
						if (DialogBox ((HINSTANCE)GetWindowLong (hwnd, GWL_HINSTANCE), 
							MAKEINTRESOURCE(ID_DLG), hwnd, DlgProc)) {
								SetClassLong (hwnd, GCL_HBRBACKGROUND, (long)GetStockObject (config.fondo));
								InvalidateRect (hwnd, NULL,TRUE);
						}
						return 0;

				}
				return 0;
		  
		  case WM_DESTROY:	//procedimiento del mensaje WM_DESTROY
				PostQuitMessage (0) ;				
				return 0 ;

	 }          
	 return DefWindowProc (hwnd, iMsg, wParam, lParam) ;  //Asegura que todos los mensajes son procesados
}    

int ganador(int jugador){
	int i, j;

	//comprobaci�n de las filas y columnas
	for(i=0;i<7;i++){
		for(j=0;j<7;j++){
			if(juego.tablero[i][j]==jugador && juego.tablero[i][j+1]==jugador 
				&& juego.tablero[i][j+2]==jugador && juego.tablero[i][j+3]==jugador && (j+3)<7){							
				return true;
			}

			if(juego.tablero[i][j]==jugador && juego.tablero[i+1][j]==jugador 
				&& juego.tablero[i+2][j]==jugador && juego.tablero[i+3][j]==jugador && (i+3)<7){
				return true;
			}
		}
	}
	//comprobaci�n de las diagonales	
	for(i=0;i<7;i++){
		for(j=0;j<7;j++){
			if(juego.tablero[i][j]==jugador && juego.tablero[i+1][j+1]==jugador 
				&& juego.tablero[i+2][j+2]==jugador && juego.tablero[i+3][j+3]==jugador && (i+3)<7 && (j+3)<7){
				return true;
			}

			if(juego.tablero[i][6-j]==jugador && juego.tablero[i+1][6-j-1]==jugador 
				&& juego.tablero[i+2][6-j-2]==jugador && juego.tablero[i+3][6-j-3]==jugador && (i+3)<7 && (6-j-3)>=0){
				return true;
			}
		}
	}
	return false;
}

BOOL CALLBACK DlgProc (HWND hdlg, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
	HWND  hwnd_dlg;
	static struct _config configDLG;
	static int color[3];
	int	  i, retorno, ret, indi;
	int   back[3] = {WHITE_BRUSH, GRAY_BRUSH, BLACK_BRUSH};
	char  cadena[4];

	switch (iMsg) {
		case WM_INITDIALOG:
			configDLG = config;
			CheckRadioButton (hdlg, IDC_ROJAS_BLANCAS, 
								IDC_NEGRAS_AMARILLAS,config.actual); //�ltimo param indica la combinaci�n de fichas en curso
			
			if(config.elegirColor==1){ //El jugador 1 puede crear un color
				color[0] = GetRValue(configDLG.colorP1);
				color[1] = GetGValue(configDLG.colorP1);
				color[2] = GetBValue(configDLG.colorP1);
			}
			if(config.elegirColor==2){
				color[0] = GetRValue(configDLG.colorP2);
				color[1] = GetGValue(configDLG.colorP2);
				color[2] = GetBValue(configDLG.colorP2);
			}


			for (i=0; i<3; i++) {
				hwnd_dlg = GetDlgItem (hdlg, IDC_SBROJO+i);
				SetScrollRange (hwnd_dlg, SB_CTL, 0, 255, FALSE);
				SetScrollPos (hwnd_dlg, SB_CTL, color[i] , TRUE);

				hwnd_dlg = GetDlgItem (hdlg, IDC_STATICR+i);
				wsprintf (cadena, "%d", color[i]);
				SetWindowText (hwnd_dlg, cadena);
			}

			hwnd_dlg = GetDlgItem (hdlg, IDC_FONDO);

			SendMessage (hwnd_dlg, CB_ADDSTRING, 0, (LPARAM)"Blanco"); 
			SendMessage (hwnd_dlg, CB_ADDSTRING, 0, (LPARAM)"Gris");
			SendMessage (hwnd_dlg, CB_ADDSTRING, 0, (LPARAM)"Negro");

			switch (configDLG.fondo) {
				case WHITE_BRUSH : 
					//Tercer par�metro especifica info adicional al mensaje (distinci�n de color)
					SendMessage (hwnd_dlg, CB_SETCURSEL, (WPARAM) 0, 0);
					break;

				case GRAY_BRUSH : 
					SendMessage (hwnd_dlg, CB_SETCURSEL, (WPARAM) 1, 0);
					break;

				case BLACK_BRUSH : 
					SendMessage (hwnd_dlg, CB_SETCURSEL, (WPARAM) 2, 0);
					break;
			}	
			return TRUE;
				

		case WM_HSCROLL:
			hwnd_dlg = (HWND) lParam;  //manipulador de la barra de desplazamiento
			retorno = GetWindowLong (hwnd_dlg, GWL_ID); //Recupera el identificador de la ventana (GWL_ID)
														
			indi = retorno - IDC_SBROJO;

			switch (LOWORD (wParam)) {
				case SB_PAGEDOWN :
					color[indi] = min (255, color[indi]+15);
					break;

				case SB_PAGEUP :
					color[indi] = max (0, color[indi]-15);
					break;

				case SB_LINEDOWN :
					color[indi] = min (255, color[indi]+1);
					break;

				case SB_LINEUP :
					color[indi] = max (0, color[indi]-1);
					break;

				case SB_THUMBTRACK:
					color[indi] = HIWORD(wParam); //Posicion de la caja de desplazamiento
					break;

				default: 
					return FALSE;
			}

			if(config.elegirColor==1) //El jugador 1 puede crear un color
				configDLG.colorP1 = RGB (color[0], color[1], color[2]);
			if(config.elegirColor==2) //El jugador 2 puede crear un color
				configDLG.colorP2 = RGB (color[0], color[1], color[2]);

			SetDlgItemInt (hdlg, IDC_STATICR+indi, color[indi], TRUE);
			SetScrollPos (hwnd_dlg, SB_CTL, color[indi], TRUE);
			return TRUE;

		case WM_COMMAND :
			switch (LOWORD (wParam)) {
				case IDOK :
					hwnd_dlg = GetDlgItem (hdlg, IDC_FONDO);
					ret = SendMessage (hwnd_dlg, CB_GETCURSEL, (WPARAM) 0, 0);
					configDLG.fondo = back[ret];

					config = configDLG;

					EndDialog (hdlg, TRUE);
					return TRUE;
		
				case IDCANCEL:
					EndDialog (hdlg, FALSE);
					return TRUE;

				case IDC_ROJAS_BLANCAS:
					configDLG.actual= IDC_ROJAS_BLANCAS;
					configDLG.colorP1=RGB(255,0,0);
					configDLG.colorP2=RGB(255,255,255);
										
					return TRUE;

				case IDC_NEGRAS_AMARILLAS:
					configDLG.actual= IDC_NEGRAS_AMARILLAS;
					configDLG.colorP1=RGB(0,0,0);
					configDLG.colorP2=RGB(255,255,0);
					
					return TRUE;
			}
			return FALSE;
	}
	return FALSE;
}
