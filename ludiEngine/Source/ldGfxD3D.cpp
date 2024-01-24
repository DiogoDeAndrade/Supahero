/******************************************************************************************
* AUTHOR: Diogo de Andrade <diogo.andrade@netvisao.pt>                                    *
*                                                                                         *
* This file, and all files in the LudiEngine is released with the 'I don't give a damn'   *
* licence... This means I'm not responsible for anything that may happen due to the use   *
* of this engine, but you can use it in any way you like it...                            *
* I would just appreciate (although it is not mandatory) if some comments on it could be  *
* made, revisions you make yourself, new filters, etc, etc, etc...                        *
* Credit where credit is due...                                                           *
*                                                                                         *
* COPYRIGHT: Copyright 2002, Diogo Andrade. All rights reserved.                          *
******************************************************************************************/
// ldGfxD3D.cpp: implementation of the ldGfxD3D class.
//
//////////////////////////////////////////////////////////////////////

#ifdef _WIN32

#include "..\HEADERS\ldGfxD3D.h"
#include "..\Headers\ldLogger.h"
#include "..\Headers\LudiEngine.h"
#include "..\Headers\ldMeshD3D.h"
#include "..\Headers\ldLight3DD3D.h"
#include "..\Headers\ldMaterialD3D.h"
#include "..\Headers\ldTextureD3D.h"
#include "..\Headers\ldFontD3D.h"
#include "..\Headers\ldParticleSystemD3D.h"
#include "..\Headers\ldUtils.h"
#include "il\il.h"
#include "il\ilu.h"
#include "il\ilut.h"
#include <d3dx8.h>

// Extra functions needed...

// Window handling procedure
long FAR PASCAL D3DWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	long n;

	switch (message)
	{
		case WM_ACTIVATEAPP:
			return 0L;

		case WM_DESTROY:
			// Clean up and close the app
			PostQuitMessage(0);
			return 0L;

#ifdef _DEBUG
		case WM_KEYDOWN:
			// Handle any non-accelerated key commands

			switch (wParam)
			{
				case VK_F10:
					LD_set_exit(true);
					break;
				case VK_F11: 
					n=51;
					break;
				case VK_F12:
					LD_set_exit(true);
					return 0L;
			}
			break;
#endif

		case WM_SETCURSOR:
			// Turn off the cursor since this is a full-screen app
			SetCursor(NULL);
			return TRUE;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldGfxD3D::ldGfxD3D()
{
	_mainWindow=NULL;
	_windowName=NULL;
	_hInstance=NULL;
	_resX=640;
	_resY=480;
	_bitDepth = 16;
	_windowMode=true;
	_g_pd3dDevice=NULL;
	_g_pD3D=NULL;
    _num_adapters=0;
    _adapter=0L;
	_min_depth_bits=16;
	_min_stencil_bits=0;
}

ldGfxD3D::~ldGfxD3D()
{
	shutdown();
}

bool ldGfxD3D::init(unsigned long res_x,unsigned long res_y,unsigned long bit_depth,bool windowed,
					char *window_name,
					void *platform_param1,void *platform_param2,void *platform_param3)
{
	if (!ldGfx::init(res_x,res_y,bit_depth,windowed,window_name)) return false;

	if (!platform_param1) 
	{
		log("Need the value of HINSTANCE for D3D renderer (init parameter 1)"); 
		return false; 
	}

	_hInstance=*((HINSTANCE*)platform_param1);

	if (!_mainWindow) 
		if (!init_window()) return false;
	if (!init_direct3d()) return false;
	if (!init_device()) return false;

	ilInit();
	iluInit();
	ilutRenderer(ILUT_WIN32);

	return true;
}

void ldGfxD3D::shutdown()
{
	if (_g_pD3D)
	{
		if(_g_pd3dDevice)
		{
			_g_pd3dDevice->Release();
			_g_pd3dDevice=NULL;
		}
		_g_pD3D->Release();
		_g_pD3D=NULL;
	}
}

bool ldGfxD3D::init_window()
{
	HWND		hWnd;	
	WNDCLASS	wc;

	// Set up and register window class
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = D3DWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = _hInstance;
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH )GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = _windowName;
    wc.lpszClassName = _windowName;
    RegisterClass(&wc);

    // Set the window's initial width
    RECT rc;
    SetRect( &rc, 0, 0, _resX, _resY );
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, TRUE );

    // Create the render window
    hWnd = CreateWindow(_windowName, _windowName, WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, (rc.right-rc.left), (rc.bottom-rc.top),
                        NULL, NULL, _hInstance, 0L );

    // Create a window
	if (!hWnd)
	{
		log("Can't create window!");
		return false;
	}

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
    SetFocus(hWnd);

	_mainWindow=hWnd;

	SetCursor(NULL);

	return true;
}

bool ldGfxD3D::init_direct3d()
{
	_g_pD3D=Direct3DCreate8(D3D_SDK_VERSION);

    if (!_g_pD3D)
	{
		log("Can't create Direct3D object!");
		return false;
	}

	return true;
}

bool ldGfxD3D::init_device()
{
    build_device_list();

	if (init_3d_environment())
	{
		set_lighting(true);
		set_ambient_light(0.0f,0.0f,0.0f,0.0f);
		set_blend_mode(LDBLENDMODE_NORMAL);
		set_render_mode(LDRENDERMODE_SOLID);

		_g_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);
		
		return true;
	}

	return false;
}

bool ldGfxD3D::init_3d_environment()
{
    HRESULT					hRet;
	D3DPRESENT_PARAMETERS	d3dpp;

    LD_D3DAdapterInfo* pAdapterInfo=&_adapters[_adapter];
    LD_D3DDeviceInfo*  pDeviceInfo=&pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice];
    LD_D3DModeInfo*    pModeInfo=&pDeviceInfo->modes[pDeviceInfo->dwCurrentMode];

    // Set up the presentation parameters
    ZeroMemory(&d3dpp,sizeof(d3dpp));
    d3dpp.Windowed=pDeviceInfo->bWindowed;
    d3dpp.BackBufferCount=1;
    d3dpp.MultiSampleType=pDeviceInfo->MultiSampleType;
    d3dpp.SwapEffect=D3DSWAPEFFECT_DISCARD;
    d3dpp.EnableAutoDepthStencil=TRUE;
    d3dpp.AutoDepthStencilFormat=pModeInfo->DepthStencilFormat;
    d3dpp.hDeviceWindow=_mainWindow;

    if (_windowMode)
    {
        d3dpp.BackBufferWidth=_resX;
        d3dpp.BackBufferHeight=_resY;
        d3dpp.BackBufferFormat=pAdapterInfo->d3ddmDesktop.Format;
		d3dpp.Windowed=TRUE;
    }
    else
    {
        d3dpp.BackBufferWidth=pModeInfo->Width;
        d3dpp.BackBufferHeight=pModeInfo->Height;
        d3dpp.BackBufferFormat=pModeInfo->Format;
		d3dpp.Windowed=FALSE;
    }

    // Create the device
/*	bool	HWVertexProcessing=bool((pModeInfo->dwBehavior) & D3DCREATE_HARDWARE_VERTEXPROCESSING);
	bool	MixedVertexProcessing=bool((pModeInfo->dwBehavior) & D3DCREATE_MIXED_VERTEXPROCESSING);
	bool	SWVertexProcessing=bool((pModeInfo->dwBehavior) & D3DCREATE_SOFTWARE_VERTEXPROCESSING);
	bool	PureDevice=bool((pModeInfo->dwBehavior) & D3DCREATE_PUREDEVICE);
*/
    hRet=_g_pD3D->CreateDevice(_adapter,pDeviceInfo->DeviceType,
                               _mainWindow,pModeInfo->dwBehavior,&d3dpp,
                               &_g_pd3dDevice);

	if (hRet==D3D_OK)
	{
		return true;
    }
	else
	{

		// If that failed, fall back to the reference rasterizer
		if (pDeviceInfo->DeviceType==D3DDEVTYPE_HAL)
		{
			// Select the default adapter
			_adapter=0L;
			pAdapterInfo=&_adapters[_adapter];

			// Look for a software device
			for( UINT i=0L; i<pAdapterInfo->dwNumDevices; i++ )
			{
				if( pAdapterInfo->devices[i].DeviceType==D3DDEVTYPE_REF )
				{
					pAdapterInfo->dwCurrentDevice=i;
					pDeviceInfo=&pAdapterInfo->devices[i];
					_windowMode=(pDeviceInfo->bWindowed)?(true):(false);
					break;
				}
			}

			// Try again, this time with the reference rasterizer
			if(pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice].DeviceType==D3DDEVTYPE_REF)
			{
				return init_3d_environment();
			}
		}
	}

    return false;
}

bool ldGfxD3D::build_device_list()
{
    const DWORD dwNumDeviceTypes = 2;
    const D3DDEVTYPE DeviceTypes[] = { D3DDEVTYPE_HAL, D3DDEVTYPE_REF };

    BOOL bHALExists = FALSE;
    BOOL bHALIsWindowedCompatible = FALSE;
    BOOL bHALIsDesktopCompatible = FALSE;
    BOOL bHALIsSampleCompatible = FALSE;

    // Loop through all the adapters on the system (usually, there's just one
    // unless more than one graphics card is present).
    for( UINT iAdapter = 0; iAdapter < _g_pD3D->GetAdapterCount(); iAdapter++ )
    {
        // Fill in adapter info
        LD_D3DAdapterInfo* pAdapter  = &_adapters[_num_adapters];
        _g_pD3D->GetAdapterIdentifier( iAdapter, 0, &pAdapter->d3dAdapterIdentifier );
        _g_pD3D->GetAdapterDisplayMode( iAdapter, &pAdapter->d3ddmDesktop );
        pAdapter->dwNumDevices    = 0;
        pAdapter->dwCurrentDevice = 0;

        // Enumerate all display modes on this adapter
        D3DDISPLAYMODE modes[100];
        D3DFORMAT      formats[20];
        DWORD dwNumFormats      = 0;
        DWORD dwNumModes        = 0;
        DWORD dwNumAdapterModes = _g_pD3D->GetAdapterModeCount( iAdapter );

        // Add the adapter's current desktop format to the list of formats
        formats[dwNumFormats++] = pAdapter->d3ddmDesktop.Format;

        for( UINT iMode = 0; iMode < dwNumAdapterModes; iMode++ )
        {
            // Get the display mode attributes
            D3DDISPLAYMODE DisplayMode;
            _g_pD3D->EnumAdapterModes( iAdapter, iMode, &DisplayMode );

            // Filter out low-resolution modes
            if( DisplayMode.Width  < 640 || DisplayMode.Height < 400 )
                continue;
			// Filter resolutions different of the one I want
            if( DisplayMode.Width  < _resX || DisplayMode.Height < _resY )
                continue;
			// Filter bit depth
            switch (_bitDepth)
			{
				case 16:
					if (DisplayMode.Format==D3DFMT_R5G6B5) break;
					if (DisplayMode.Format==D3DFMT_X1R5G5B5) break;
					if (DisplayMode.Format==D3DFMT_A1R5G5B5) break;
					if (DisplayMode.Format==D3DFMT_A4R4G4B4) break;
					if (DisplayMode.Format==D3DFMT_X4R4G4B4) break;
					if (DisplayMode.Format==D3DFMT_A8R3G3B2) break;
					continue;
					break;
				case 32:
					if (DisplayMode.Format==D3DFMT_A8R8G8B8) break;
					if (DisplayMode.Format==D3DFMT_X8R8G8B8) break;
					continue;
					break;
				default:
					continue;
			}
			// Check if the mode already exists (to filter out refresh rates)
            for( DWORD m=0L; m<dwNumModes; m++ )
            {
                if( ( modes[m].Width  == DisplayMode.Width  ) &&
                    ( modes[m].Height == DisplayMode.Height ) &&
                    ( modes[m].Format == DisplayMode.Format ) )
                    break;
            }

            // If we found a new mode, add it to the list of modes
            if( m == dwNumModes )
            {
                modes[dwNumModes].Width       = DisplayMode.Width;
                modes[dwNumModes].Height      = DisplayMode.Height;
                modes[dwNumModes].Format      = DisplayMode.Format;
                modes[dwNumModes].RefreshRate = 0;
                dwNumModes++;

                // Check if the mode's format already exists
                for( DWORD f=0; f<dwNumFormats; f++ )
                {
                    if( DisplayMode.Format == formats[f] )
                        break;
                }

                // If the format is new, add it to the list
                if( f== dwNumFormats )
                    formats[dwNumFormats++] = DisplayMode.Format;
            }
        }

        // Sort the list of display modes (by format, then width, then height)

        // Add devices to adapter
        for( UINT iDevice = 0; iDevice < dwNumDeviceTypes; iDevice++ )
        {
            // Fill in device info
            LD_D3DDeviceInfo* pDevice;
            pDevice                 = &pAdapter->devices[pAdapter->dwNumDevices];
            pDevice->DeviceType     = DeviceTypes[iDevice];
            _g_pD3D->GetDeviceCaps( iAdapter, DeviceTypes[iDevice], &pDevice->d3dCaps );
            pDevice->dwNumModes     = 0;
            pDevice->dwCurrentMode  = 0;
            pDevice->bCanDoWindowed = FALSE;
            pDevice->bWindowed      = FALSE;
            pDevice->MultiSampleType = D3DMULTISAMPLE_NONE;

            // Examine each format supported by the adapter to see if it will
            // work with this device and meets the needs of the application.
            BOOL  bFormatConfirmed[20];
            DWORD dwBehavior[20];
            D3DFORMAT fmtDepthStencil[20];

            for( DWORD f=0; f<dwNumFormats; f++ )
            {
                bFormatConfirmed[f] = FALSE;
                fmtDepthStencil[f] = D3DFMT_UNKNOWN;

                // Skip formats that cannot be used as render targets on this device
                if( FAILED( _g_pD3D->CheckDeviceType( iAdapter, pDevice->DeviceType,
                                                     formats[f], formats[f], FALSE ) ) )
                    continue;

                if( pDevice->DeviceType == D3DDEVTYPE_HAL )
                {
                    // This system has a HAL device
                    bHALExists = TRUE;

                    if( pDevice->d3dCaps.Caps2 & D3DCAPS2_CANRENDERWINDOWED )
                    {
                        // HAL can run in a window for some mode
                        bHALIsWindowedCompatible = TRUE;

                        if( f == 0 )
                        {
                            // HAL can run in a window for the current desktop mode
                            bHALIsDesktopCompatible = TRUE;
                        }
                    }
                }

                // Confirm the device/format for HW vertex processing
                if( pDevice->d3dCaps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT )
                {
                    if( pDevice->d3dCaps.DevCaps&D3DDEVCAPS_PUREDEVICE )
                    {
                        dwBehavior[f] = D3DCREATE_HARDWARE_VERTEXPROCESSING |
                                        D3DCREATE_PUREDEVICE;

                        if( confirm_device( &pDevice->d3dCaps, dwBehavior[f],
                                                      formats[f] ) )
                            bFormatConfirmed[f] = TRUE;
                    }

                    if ( FALSE == bFormatConfirmed[f] )
                    {
                        dwBehavior[f] = D3DCREATE_HARDWARE_VERTEXPROCESSING;

                        if( confirm_device( &pDevice->d3dCaps, dwBehavior[f],
                                                      formats[f] ) )
                            bFormatConfirmed[f] = TRUE;
                    }

                    if ( FALSE == bFormatConfirmed[f] )
                    {
                        dwBehavior[f] = D3DCREATE_MIXED_VERTEXPROCESSING;

                        if( confirm_device( &pDevice->d3dCaps, dwBehavior[f],
                                                      formats[f] ) )
                            bFormatConfirmed[f] = TRUE;
                    }
                }

                // Confirm the device/format for SW vertex processing
                if( FALSE == bFormatConfirmed[f] )
                {
                    dwBehavior[f] = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

                    if( confirm_device( &pDevice->d3dCaps, dwBehavior[f],
                                                  formats[f] ) )
                        bFormatConfirmed[f] = TRUE;
                }

                // Find a suitable depth/stencil buffer format for this device/format
                if( bFormatConfirmed[f] )
                {
                    if( !find_depth_stencil_format( iAdapter, pDevice->DeviceType,
                        formats[f], &fmtDepthStencil[f] ) )
                    {
                        bFormatConfirmed[f] = FALSE;
                    }
                }
            }

            // Add all enumerated display modes with confirmed formats to the
            // device's list of valid modes
            for( DWORD m=0L; m<dwNumModes; m++ )
            {
                for( DWORD f=0; f<dwNumFormats; f++ )
                {
                    if( modes[m].Format == formats[f] )
                    {
                        if( bFormatConfirmed[f] == TRUE )
                        {
                            // Add this mode to the device's list of valid modes
                            pDevice->modes[pDevice->dwNumModes].Width      = modes[m].Width;
                            pDevice->modes[pDevice->dwNumModes].Height     = modes[m].Height;
                            pDevice->modes[pDevice->dwNumModes].Format     = modes[m].Format;
                            pDevice->modes[pDevice->dwNumModes].dwBehavior = dwBehavior[f];
                            pDevice->modes[pDevice->dwNumModes].DepthStencilFormat = fmtDepthStencil[f];
                            pDevice->dwNumModes++;

                            if( pDevice->DeviceType == D3DDEVTYPE_HAL )
                                bHALIsSampleCompatible = TRUE;
                        }
                    }
                }
            }

            // Select any 640x480 mode for default (but prefer a 16-bit mode)
            for( m=0; m<pDevice->dwNumModes; m++ )
            {
                if( pDevice->modes[m].Width==640 && pDevice->modes[m].Height==480 )
                {
                    pDevice->dwCurrentMode = m;
                    if( pDevice->modes[m].Format == D3DFMT_R5G6B5 ||
                        pDevice->modes[m].Format == D3DFMT_X1R5G5B5 ||
                        pDevice->modes[m].Format == D3DFMT_A1R5G5B5 )
                    {
                        break;
                    }
                }
            }

            // Check if the device is compatible with the desktop display mode
            // (which was added initially as formats[0])
            if( bFormatConfirmed[0] && (pDevice->d3dCaps.Caps2 & D3DCAPS2_CANRENDERWINDOWED) )
            {
                pDevice->bCanDoWindowed = TRUE;
                pDevice->bWindowed      = TRUE;
            }

            // If valid modes were found, keep this device
            if( pDevice->dwNumModes > 0 )
                pAdapter->dwNumDevices++;
        }

        // If valid devices were found, keep this adapter
        if( pAdapter->dwNumDevices > 0 )
            _num_adapters++;
    }

    // Return an error if no compatible devices were found
    if( 0L == _num_adapters )
        return false;

    // Pick a default device that can render into a window
    // (This code assumes that the HAL device comes before the REF
    // device in the device array).
    for( DWORD a=0; a<_num_adapters; a++ )
    {
        for( DWORD d=0; d < _adapters[a].dwNumDevices; d++ )
        {
            if( _adapters[a].devices[d].bWindowed )
            {
                _adapters[a].dwCurrentDevice = d;
                _adapter = a;

                return true;
            }
        }
    }

    return false;
}

bool ldGfxD3D::confirm_device(D3DCAPS8* caps,DWORD flags,D3DFORMAT format)
{
    if ((flags & D3DCREATE_HARDWARE_VERTEXPROCESSING) ||
        (flags & D3DCREATE_MIXED_VERTEXPROCESSING))
    {
		return false;
//        if (caps->MaxPointSize<=0.0f) return false;
    }

	return true;
}

bool ldGfxD3D::find_depth_stencil_format( UINT iAdapter, D3DDEVTYPE DeviceType,
														  D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat )
{
    if( _min_depth_bits <= 16 && _min_stencil_bits == 0 )
    {
        if( SUCCEEDED( _g_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16 ) ) )
        {
            if( SUCCEEDED( _g_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D16 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D16;
                return true;
            }
        }
    }

    if( _min_depth_bits <= 15 && _min_stencil_bits <= 1 )
    {
        if( SUCCEEDED( _g_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D15S1 ) ) )
        {
            if( SUCCEEDED( _g_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D15S1 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D15S1;
                return true;
            }
        }
    }

    if( _min_depth_bits <= 24 && _min_stencil_bits == 0 )
    {
        if( SUCCEEDED( _g_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8 ) ) )
        {
            if( SUCCEEDED( _g_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D24X8 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D24X8;
                return true;
            }
        }
    }

    if( _min_depth_bits <= 24 && _min_stencil_bits <= 8 )
    {
        if( SUCCEEDED( _g_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8 ) ) )
        {
            if( SUCCEEDED( _g_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D24S8 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D24S8;
                return true;
            }
        }
    }

    if( _min_depth_bits <= 24 && _min_stencil_bits <= 4 )
    {
        if( SUCCEEDED( _g_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X4S4 ) ) )
        {
            if( SUCCEEDED( _g_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D24X4S4 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D24X4S4;
                return true;
            }
        }
    }

    if( _min_depth_bits <= 32 && _min_stencil_bits == 0 )
    {
        if( SUCCEEDED( _g_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32 ) ) )
        {
            if( SUCCEEDED( _g_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D32 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D32;
                return true;
            }
        }
    }

    return false;
}

void ldGfxD3D::clear(unsigned int clear_buffers,float r,float g,float b,float a,float zVal,unsigned long sVal)
{
	DWORD flags=0;

	if (clear_buffers & LD_FRAME_BUFFER) flags|=D3DCLEAR_TARGET;
	if (clear_buffers & LD_Z_BUFFER) flags|=D3DCLEAR_ZBUFFER;
	if (clear_buffers & LD_STENCIL_BUFFER) flags|=D3DCLEAR_STENCIL;

#ifdef _DEBUG
	HRESULT		hRet=
#endif;

	_g_pd3dDevice->Clear(0,NULL,flags,float_to_colorref(r,g,b,a),zVal,sVal);

#ifdef _DEBUG
	if (hRet!=D3D_OK)
	{
		log("Can't clear the following buffers!");
		if (clear_buffers & LD_FRAME_BUFFER) log("Frame buffer");
		if (clear_buffers & LD_Z_BUFFER) log("Z buffer");
		if (clear_buffers & LD_STENCIL_BUFFER) log("Stencil buffer");
		log("---------------");
	}
#endif
}

void ldGfxD3D::swap_buffers()
{
	HRESULT hRet;

	hRet=_g_pd3dDevice->Present(NULL,NULL,NULL,NULL);
	if (hRet!=D3D_OK)
	{
		switch (hRet)
		{
			case D3DERR_INVALIDCALL: log("Couldn't swap buffers - Invalid call!");
									 break;
			case D3DERR_DEVICELOST: log("Couldn't swap buffers - Device lost!");
									break;
		}
	}
}

ldMesh *ldGfxD3D::new_mesh()
{
	return new ldMeshD3D;
}

ldLight3D *ldGfxD3D::new_light()
{
	return new ldLight3DD3D;
}

ldMaterial *ldGfxD3D::new_material()
{
	return new ldMaterialD3D;
}

ldTexture *ldGfxD3D::new_texture()
{
	return new ldTextureD3D;
}

ldFont *ldGfxD3D::new_font()
{
	return new ldFontD3D;
}

ldParticleSystem *ldGfxD3D::new_particle_system()
{
	return new ldParticleSystemD3D;
}

void ldGfxD3D::set_lighting(bool b)
{
	_g_pd3dDevice->SetRenderState(D3DRS_LIGHTING,(b)?(TRUE):(FALSE));
}

void ldGfxD3D::set_ambient_light(float r,float g,float b,float a)
{
#ifdef _DEBUG
	HRESULT		hRet=
#endif;

	_g_pd3dDevice->SetRenderState(D3DRS_AMBIENT,float_to_colorref(r,g,b,a));

#ifdef _DEBUG
	if (hRet!=D3D_OK)
	{
		log("Can't set ambient color!");
	}
#endif
}

void ldGfxD3D::set_perspective(float fov,float near_plane,float far_plane)
{
	D3DXMATRIX	persp;

	D3DXMatrixPerspectiveFovRH(&persp,deg_to_rad(fov/2.0f),float(_resX)/float(_resY),near_plane,far_plane);

#ifdef _DEBUG
	HRESULT		hRet=
#endif;

	_g_pd3dDevice->SetTransform(D3DTS_PROJECTION,&persp);	

#ifdef _DEBUG
	if (hRet!=D3D_OK)
	{
		log("Can't set perspective matrix (fov=%f,aspect ratio=%f,near plane=%f,far plane=%f)!",fov,float(_resX)/float(_resY),near_plane,far_plane);
	}
#endif
}

void ldGfxD3D::set_camera(Vec3f *pos,Quaternion *ori)
{
	D3DXMATRIX		mat;
	D3DXQUATERNION	quat; quat.x=ori->x; quat.y=ori->y; quat.z=ori->z; quat.w=-ori->w;
	D3DXVECTOR3		vec; vec.x=-pos->x; vec.y=-pos->y; vec.z=-pos->z;

	D3DXMatrixAffineTransformation(&mat,1.0f,&(-vec),&quat,&vec);

#ifdef _DEBUG
	HRESULT		hRet=
#endif;

	_g_pd3dDevice->SetTransform(D3DTS_VIEW,&mat);

#ifdef _DEBUG
	if (hRet!=D3D_OK)
	{
		log("Can't set view matrix!");
	}
#endif
}

void ldGfxD3D::set_world_transformation(int world_trans_id,Vec3f *pos,Quaternion *ori)
{
	D3DXMATRIX		mat;
	D3DXQUATERNION	quat; quat.x=ori->x; quat.y=ori->y; quat.z=ori->z; quat.w=ori->w;
	D3DXVECTOR3		vec; vec.x=pos->x; vec.y=pos->y; vec.z=pos->z;

	D3DXMatrixAffineTransformation(&mat,1.0f,NULL,&quat,&vec);

#ifdef _DEBUG
	HRESULT		hRet=
#endif;

	_g_pd3dDevice->SetTransform(D3DTS_WORLDMATRIX(world_trans_id),&mat);

#ifdef _DEBUG
	if (hRet!=D3D_OK)
	{
		log("Can't set world matrix:");
	}
#endif
}

void ldGfxD3D::begin_render()
{
	// Set the lights necessary for rendering
	ldLight3DD3D			*tmp;
	ldLight3DList::iterator	i;
	unsigned long			j=0;

	for (i=_lights.begin(); i!=_lights.end(); ++i)
	{
		// Enable lights
		tmp=(ldLight3DD3D*)*i;
		if (tmp) 
		{ 
			if (tmp->is_enable())
			{
				tmp->set(j); 
				j++; 
			}
		}
	}
	for (; j<8; j++)
	{
		// Disable other lights
		_g_pd3dDevice->LightEnable(j,FALSE);
	}

}

void ldGfxD3D::end_render()
{
	// Do nothing for now
}

void ldGfxD3D::set_blend_mode(LDBlendMode mode)
{
	_blend_mode=mode;

	switch (mode)
	{
		case LDBLENDMODE_NONE:
			_g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
			break;
		case LDBLENDMODE_NORMAL:
			_g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
			_g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
			_g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
			break;
		case LDBLENDMODE_ADD:
			_g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
			_g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
			_g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
			break;
	}
}

void ldGfxD3D::set_render_mode(LDRenderMode mode)
{
	switch (mode)
	{
		case LDRENDERMODE_POINT:
			_g_pd3dDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_POINT);
			break;
		case LDRENDERMODE_WIREFRAME:
			_g_pd3dDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
			break;
		case LDRENDERMODE_SOLID:
			_g_pd3dDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
			break;
	}
}

struct ldSpriteStruct {
	float	x,y,z,w;
	DWORD	color;
	float	tu,tv;
};

void ldGfxD3D::draw_sprite(Vec2f *pos,float size_x,float size_y,Vec4f *color,float u1,float v1,float u2,float v2,ldTexture *texture)
{
	// Should have some sort of batching mechanism
	ldSpriteStruct	tmp[4];
	DWORD			c=float_to_colorref(color);

	tmp[0].x=pos->x; tmp[0].y=pos->y+size_y; tmp[0].z=0.0f; tmp[0].w=1.0f;
	tmp[0].color=c;
	tmp[0].tu=u1; tmp[0].tv=v2;
	tmp[1].x=pos->x; tmp[1].y=pos->y; tmp[1].z=0.0f; tmp[1].w=1.0f;
	tmp[1].color=c;
	tmp[1].tu=u1; tmp[1].tv=v1;
	tmp[2].x=pos->x+size_x; tmp[2].y=pos->y+size_y; tmp[2].z=0.0f; tmp[2].w=1.0f;
	tmp[2].color=c;
	tmp[2].tu=u2; tmp[2].tv=v2;
	tmp[3].x=pos->x+size_x; tmp[3].y=pos->y; tmp[3].z=0.0f; tmp[3].w=1.0f;
	tmp[3].color=c;
	tmp[3].tu=u2; tmp[3].tv=v1;

	HRESULT hRet;
	
	hRet=_g_pd3dDevice->BeginScene();
	if (hRet==D3D_OK)
	{
		((ldTextureD3D*)texture)->set(0);
		hRet=_g_pd3dDevice->SetRenderState(D3DRS_ZENABLE,FALSE);
		hRet=_g_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
		hRet=_g_pd3dDevice->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
		hRet=_g_pd3dDevice->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
		hRet=_g_pd3dDevice->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
		hRet=_g_pd3dDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
		hRet=_g_pd3dDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
		hRet=_g_pd3dDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
		hRet=_g_pd3dDevice->SetVertexShader(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0));
		hRet=_g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,&tmp,sizeof(ldSpriteStruct));
		hRet=_g_pd3dDevice->SetTexture(0,NULL);
		hRet=_g_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);
		hRet=_g_pd3dDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
		hRet=_g_pd3dDevice->EndScene();

	}	
}

#endif // _WIN32