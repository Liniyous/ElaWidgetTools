#include "GenO.h"

#include <iostream>

#include "GenError.h"
#include "UtWallClock.h"
GenO::GenO(const char* aName)
    : mName(aName != nullptr ? aName : ""), mGenBufXferO(nullptr), mErrorStatus(GenBuf::NoError)
{
}

// Virtual
GenO::~GenO() {}

// Virtual
// A derived send method should either call this function
// or call ClearErrorStatus directly.
int GenO::Send()
{
    ClearOutputErrorStatus();
    return 0;
}

void GenO::SetGenBufXferO(GenBufXferO* aGenBufXferO)
{
    mGenBufXferO = aGenBufXferO;
}

// Virtual
int GenO::HandlePutError(int aStatus, unsigned long aVariableSize)
{
    if (mErrorStatus == GenBuf::NoError)
    {
        mErrorStatus = aStatus;

        if (aStatus > GenBuf::FixFailed)
        {
            if (GenError::ShowError(GenError::cIO_ERRORS))
            {
                const char* msg[] = {"Experienced an internal GenO::HandlePutError",
                                     "Experienced an undefined error",
                                     "Could find no more data to read",
                                     "Has no more room to put",
                                     "Had an allocation error"};

                std::cout << "GenO called HandlePutError."
                          << " Wall Clock: " << UtWallClock::GetWallClockTime()
                          << " GenO: " << mName
                          << " Error: " << msg[-aStatus] << " when trying to read " << aVariableSize << " bytes."
                          << std::endl;
            }
        }
    }

    return 0;
}

int GenO::PutString(const std::string& aString)
{
    unsigned int size = static_cast<unsigned int>(aString.size());
    Put(size);
    if (GetOutputStatus() != GenBuf::NoError)
    {
        return -1;
    }

    if (size > 0)
    {
        int status = mGenBufXferO->PutString(aString);
        if (status < 0)
        {
            if (HandlePutError(status, size))
            {
                if ((status = mGenBufXferO->PutString(aString)) < 0)
                {
                    HandlePutError(GenBuf::FixFailed + status, size);
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

// putValue method for raw data
int GenO::PutRaw(const void* aDataPtr, unsigned long aDataSizeBytes)
{
    if (GetOutputStatus() != GenBuf::NoError)
    {
        return -1;
    }

    if (aDataSizeBytes > 0)
    {
        int status = mGenBufXferO->PutRaw(aDataPtr, aDataSizeBytes);

        // If there is an error, try to handle the error and send again.
        if (status < 0)
        {
            // Handle the errors
            if (HandlePutError(status, aDataSizeBytes))
            {
                // Let's try to put data again
                if ((status = mGenBufXferO->PutRaw(aDataPtr, aDataSizeBytes)) < 0)
                {
                    HandlePutError(GenBuf::FixFailed + status, aDataSizeBytes);
                }
            }
        } // End status < 0
        return status;
    }
    else
    {
        return 0;
    } // End aDataSizeBytes > 0

} // End PutRaw()

int GenO::GetPutBufferIfValid(void*& aBufferPtr, unsigned long aDataSizeBytes)
{
    if (GetOutputStatus() != GenBuf::NoError)
    {
        return -1;
    }

    if (aDataSizeBytes > 0)
    {
        aBufferPtr = nullptr;
        int status = mGenBufXferO->GetBufferIfValid(aBufferPtr, aDataSizeBytes);

        // If there is an error, try to handle the error and send again.
        if (status < 0)
        {
            // Handle the errors
            if (HandlePutError(status, aDataSizeBytes))
            {
                // Let's try to put data again
                if ((status = mGenBufXferO->GetBufferIfValid(aBufferPtr, aDataSizeBytes)) < 0)
                {
                    HandlePutError(GenBuf::FixFailed + status, aDataSizeBytes);
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
