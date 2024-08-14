// ValidationResultControl.cpp : Implementation of CValidationResultControl
#include "pch.h"
#include "ValidationResultControl.h"


// CValidationResultControl
STDMETHODIMP CValidationResultControl::GetControlInfo(CONTROLINFO* pCI)
{
	pCI->cb = sizeof(CONTROLINFO);
	pCI->hAccel = NULL;
	pCI->cAccel = 0;
	pCI->dwFlags = 0;
	return S_OK;
}
