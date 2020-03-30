#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#ifdef NOMINMAX
    #include <windows.h>
#else
    #define NOMINMAX
#include <windows.h>
    #undef NOMINMAX
#endif

#include "targetver.h"

#include <windows.ui.xaml.media.dxinterop.h>

#include <dxgi.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Windows.Graphics.Display.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/windows.UI.Xaml.h>
#include <winrt/windows.UI.Xaml.controls.h>



