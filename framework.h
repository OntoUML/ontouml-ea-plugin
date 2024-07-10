#pragma once

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED

#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit


#define ATL_NO_ASSERT_ON_DESTROY_NONEXISTENT_WINDOW

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <atlsafe.h>

//////////
// This #import is *not* about C++ '22 modules. It is an old Microsoft specific way to transpile
// type libraries into C++ consumable headers. The COM #import statement is still (somewhat)
// supported. It requires the Microsoft specific extensions to be enabled *and* the concurrent
// build to be disabled. The latter can be easily achieved by putting the #import's in the
// precompiled header, as header precompilation runs on concurrency disabled by definition.
//////////
#import "EA.tlb" no_namespace raw_interfaces_only

//////////
// Workaround for #import bug in VS2022. It generates the .tlh file, but forgets to emit the
// corresponding #include
//////////
#include "EA.tlh"