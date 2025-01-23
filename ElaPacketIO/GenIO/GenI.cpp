#include "GenI.h"
#include "GenError.h"
#include "UtWallClock.h"
#include <algorithm>
#include <iostream>
#include <string>
GenI::GenI(const char* aName)
    : mName(aName != nullptr ? aName : ""), mGenBufXferI(nullptr), mErrorStatus(GenBuf::NoError)
{
}

// Virtual
GenI::~GenI() {}

// Virtual
// A derived Receive method should either call this function
// or call ClearErrorStatus directly.
int GenI::Receive(int /* aWaitTimeInMicroSec */)
{
    ClearInputErrorStatus();
    return 0;
}

void GenI::SetGenBufXferI(GenBufXferI* aGenBufXferI)
{
    mGenBufXferI = aGenBufXferI;
}

// Virtual
int GenI::HandleGetError(int aStatus, unsigned long aVariableSize)
{
    if (mErrorStatus == GenBuf::NoError)
    {
        mErrorStatus = aStatus;

        if (aStatus > GenBuf::FixFailed)
        {
            if (GenError::ShowError(GenError::cIO_ERRORS))
            {
                const char* msg[] = {"Experienced an internal GenI::HandleGetError",
                                     "Experienced an undefined error",
                                     "Could find no more data to read",
                                     "Has no more room to put",
                                     "Had an allocation error"};

                std::cout << "GenI called HandleGetError. "
                          << "Wall Clock: " << UtWallClock::GetWallClockTime()
                          << " GenI: " << mName
                          << " Error: " << msg[-aStatus] << " when trying to read " << aVariableSize << " bytes."
                          << std::endl;
            }
        }
    }

    return 0;
}

int GenI::GetString(std::string& aString)
{
    unsigned int size;
    Get(size);
    if (GetInputStatus() != GenBuf::NoError)
    {
        return -1;
    }

    if (size > 0)
    {
        int status = mGenBufXferI->GetString(aString, size);
        if (status < 0)
        {
            if (HandleGetError(status, size))
            {
                if ((status = mGenBufXferI->GetString(aString, size)) < 0)
                {
                    HandleGetError(GenBuf::FixFailed + status, size);
                }
            }
            return status >= 0 ? status + 4 : status;
        }
        else
        {
            return status + 4;
        }
    }
    else
    {
        return 4;
    }
}

// getValue method for raw data
int GenI::GetRaw(void* aDataPtr, unsigned long aDataSizeBytes)
{
    if (GetInputStatus() != GenBuf::NoError)
    {
        return -1;
    }

    if (aDataSizeBytes > 0)
    {
        int status = mGenBufXferI->GetRaw(aDataPtr, aDataSizeBytes);

        // If there is an error, try to handle the error and send again
        if (status < 0)
        {
            if (HandleGetError(status, aDataSizeBytes))
            {
                // Let's try to put data again
                if ((status = mGenBufXferI->GetRaw(aDataPtr, aDataSizeBytes)) < 0)
                {
                    HandleGetError(GenBuf::FixFailed + status, aDataSizeBytes);
                }
            }
        } // End status < 0
        return status;
    }
    else
    {
        return 0;
    } // End aDataSizeBytes > 0

} // End GetRaw()

int GenI::GetGetBufferIfValid(const void*& aBufferPtr, unsigned long aDataSizeBytes)
{
    if (GetInputStatus() != GenBuf::NoError)
    {
        return 0;
    }

    if (aDataSizeBytes > 0)
    {
        aBufferPtr = nullptr;
        int status = mGenBufXferI->GetBufferIfValid(aBufferPtr, aDataSizeBytes);

        // If there is an error, try to handle the error and send again
        if (status < 0)
        {
            if (HandleGetError(status, aDataSizeBytes))
            {
                // Let's try to put data again
                if ((status = mGenBufXferI->GetBufferIfValid(aBufferPtr, aDataSizeBytes)) < 0)
                {
                    HandleGetError(GenBuf::FixFailed + status, aDataSizeBytes);
                }
            }
        }
        return status;
    }
    else
    {
        return 0;
    }
}
