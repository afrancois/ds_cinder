// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=216d5a271bd5a8df2c5d8db3f44c4b66c98c5c4e$
//

#ifndef CEF_LIBCEF_DLL_CPPTOC_VIEWS_VIEW_CPPTOC_H_
#define CEF_LIBCEF_DLL_CPPTOC_VIEWS_VIEW_CPPTOC_H_
#pragma once

#if !defined(BUILDING_CEF_SHARED)
#error This file can be included DLL-side only
#endif

#include "include/capi/views/cef_browser_view_capi.h"
#include "include/capi/views/cef_button_capi.h"
#include "include/capi/views/cef_panel_capi.h"
#include "include/capi/views/cef_scroll_view_capi.h"
#include "include/capi/views/cef_textfield_capi.h"
#include "include/capi/views/cef_view_capi.h"
#include "include/capi/views/cef_window_capi.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_button.h"
#include "include/views/cef_panel.h"
#include "include/views/cef_scroll_view.h"
#include "include/views/cef_textfield.h"
#include "include/views/cef_view.h"
#include "include/views/cef_window.h"
#include "libcef_dll/cpptoc/cpptoc_ref_counted.h"

// Wrap a C++ class with a C structure.
// This class may be instantiated and accessed DLL-side only.
class CefViewCppToC
    : public CefCppToCRefCounted<CefViewCppToC, CefView, cef_view_t> {
 public:
  CefViewCppToC();
};

#endif  // CEF_LIBCEF_DLL_CPPTOC_VIEWS_VIEW_CPPTOC_H_
