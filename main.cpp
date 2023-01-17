#define UNICODE

#pragma comment(linker,"/opt:nowin98")
#include<windows.h>

#define IDU_BUTTON1 100
TCHAR szClassName[]=TEXT("window");

#define N624 624
#define M397 397
#define MATRIX_A 0x9908b0dfUL
#define UPPER_MASK 0x80000000UL
#define LOWER_MASK 0x7fffffffUL

static unsigned long mt[N624];
static int mti=N624+1;

void init_genrand(unsigned long s)
{
	mt[0]=s&0xffffffffUL;
	for(mti=1;mti<N624;mti++)
	{
		mt[mti]=(1812433253UL*(mt[mti-1]^(mt[mti-1]>>30))+mti);
		mt[mti]&=0xffffffffUL;
	}
}

void init_by_array(unsigned long init_key[],int key_length)
{
	int i,j,k;
	init_genrand(19650218UL);
	i=1;j=0;
	k=(N624>key_length?N624:key_length);
	for(;k;k--)
	{
		mt[i]=(mt[i]^((mt[i-1]^(mt[i-1]>>30))*1664525UL))+init_key[j]+j;
		mt[i]&=0xffffffffUL;
		i++;j++;
		if(i>=N624){mt[0]=mt[N624-1];i=1;}
		if(j>=key_length)j=0;
	}
	for(k=N624-1;k;k--)
	{
		mt[i]=(mt[i]^((mt[i-1]^(mt[i-1]>>30))*1566083941UL))-i;
		mt[i]&=0xffffffffUL;
		i++;
		if(i>=N624){mt[0]=mt[N624-1];i=1;}
	}
	mt[0]=0x80000000UL;
}

unsigned long genrand_int32(void)
{
	unsigned long y;
	static unsigned long mag01[2]={0x0UL, MATRIX_A};
	if(mti>=N624)
	{
		int kk;
		if(mti==N624+1)
			init_genrand(5489UL);
		for(kk=0;kk<N624-M397;kk++)
		{
			y=(mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
			mt[kk]=mt[kk+M397]^(y>>1)^mag01[y&0x1UL];
		}
		for(;kk<N624-1;kk++)
		{
			y=(mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
			mt[kk]=mt[kk+(M397-N624)]^(y>>1)^mag01[y&0x1UL];
		}
		y=(mt[N624-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
		mt[N624-1]=mt[M397-1]^(y>>1)^mag01[y & 0x1UL];
		mti=0;
	}
	y=mt[mti++];
	y^=(y>>11);
	y^=(y<<7)&0x9d2c5680UL;
	y^=(y<<15)&0xefc60000UL;
	y^=(y>>18);
	return y;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	static HWND hButton,hEdit;
	switch(msg)
	{
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDU_BUTTON1:
			{
				TCHAR buf[11];
				int i;
				unsigned long init[4]={0x123,0x234,0x345,0x456},length=4;
				init_by_array(init,length);
				LockWindowUpdate(hEdit);
				SetWindowText(hEdit,0);
				for(i=0;i<1000;i++)
				{
					wsprintf(buf,TEXT("%10lu "),genrand_int32());
					SendMessage(hEdit,EM_REPLACESEL,0,(LPARAM)buf);
					if(i%8==7)
					{
						SendMessage(hEdit,EM_REPLACESEL,0,(LPARAM)TEXT("\r\n"));
					}
				}
				LockWindowUpdate(0);
			}
			break;
		}
		break;
		case WM_CREATE:
			hButton=CreateWindow(TEXT("BUTTON"),TEXT("óêêîÇê∂ê¨Ç∑ÇÈ"),WS_CHILD|WS_VISIBLE,10,10,200,30,hWnd,(HMENU)IDU_BUTTON1,((LPCREATESTRUCT)lParam)->hInstance,NULL);
			hEdit=CreateWindowEx(WS_EX_CLIENTEDGE,TEXT("EDIT"),NULL,WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL|ES_WANTRETURN|ES_MULTILINE|ES_AUTOVSCROLL|ES_AUTOHSCROLL,0,50,300,200,hWnd,NULL,((LPCREATESTRUCT)lParam)->hInstance,NULL);
			SetFocus(hButton);
			break;
		case WM_SIZE:
			MoveWindow(hEdit,0,50,LOWORD(lParam),HIWORD(lParam)-50,FALSE);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return(DefWindowProc(hWnd,msg,wParam,lParam));
	}
	return(0L);
}

EXTERN_C void __cdecl WinMainCRTStartup()
{
	MSG msg;
	HINSTANCE hInstance=GetModuleHandle(0);
	WNDCLASS wndclass={CS_HREDRAW|CS_VREDRAW,WndProc,0,0,hInstance,0,LoadCursor(0,IDC_ARROW),(HBRUSH)(COLOR_WINDOW+1),0,szClassName};
	RegisterClass(&wndclass);
	HWND hWnd=CreateWindow(szClassName,TEXT("Mersenne Twister Ç≈óêêîÇê∂ê¨Ç∑ÇÈ"),WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,0,CW_USEDEFAULT,0,0,0,hInstance,0);
	ShowWindow(hWnd,SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while(GetMessage(&msg,0,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ExitProcess(msg.wParam);
}

#if _DEBUG
void main(){}
#endif
