#include "GenError.h"

unsigned int GenError::mSuppressErrorMask = 0;

// static
void GenError::SetSuppressErrorMask(unsigned int aSuppressErrorMask)
{
    mSuppressErrorMask = aSuppressErrorMask;
}

// static
unsigned int GenError::GetSuppressErrorMask()
{
    return mSuppressErrorMask;
}

// static
bool GenError::ShowError(unsigned int aErrorMask)
{
    bool showError = true;

    if (mSuppressErrorMask & aErrorMask)
    {
        showError = false;
    }
    return showError;
}
