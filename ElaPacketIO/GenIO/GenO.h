#ifndef GENO_H
#define GENO_H

#include "ElaPacketIO_Export.h"

#include <string>

#include "GenBufXferO.h"

class ELA_PACKETIO_EXPORT GenO
{
public:
    /// Destructor
    virtual ~GenO();

    /// The Puts for built in types.
    /// \param  aValue The variable to be passed to the send buffer. The variable can be any built-in non
    ///         pointer type or a char* or a const char*
    //  Goofy microsoft compiler will not let function templates be defined
    //  outside the class definition, so they are defined here.
    template <class T>
    void Put(T aValue)
    {
        int status = mGenBufXferO->Put(aValue);
        if (status < 0)
        {
            if (HandlePutError(status, sizeof(T)))
            {
                if ((status = mGenBufXferO->Put(aValue)) < 0)
                {
                    HandlePutError(GenBuf::FixFailed + status, sizeof(T));
                }
            }
        }
    }

    /// Specialization methods for putting character arrays.
    //@{
    void Put(const char* aValue);
    void Put(char* aValue);
    //@}

    /// putValue a vector of a built in type in the send buffer.
    /// \warning  This function will put an extra 8 bytes in the send buffer
    ///           to describe the length of the vector.  Remember to account
    ///           for the these extra 8 bytes when computing the length of
    ///           your message.
    /// \return   A negative number if there is an error, else returns the number of bytes put in the send buffer..
    template <class T, class A>
    int PutVector(const std::vector<T, A>& aVec)
    {
        unsigned int size = static_cast<unsigned int>(aVec.size());
        Put(size);
        if (GetOutputStatus() != GenBuf::NoError)
        {
            return -1;
        }

        // Use pad just in case we are dealing with doubles and then we will
        // stay on a double word boundary.  Assuming we were called when
        // on a double word boundary.
        unsigned int pad = 0;
        Put(pad);
        if (GetOutputStatus() != GenBuf::NoError)
        {
            return -1;
        }

        if (size > 0)
        {
            int status = mGenBufXferO->PutVector(aVec);
            if (status < 0)
            {
                if (HandlePutError(status, sizeof(T) * size))
                {
                    if ((status = mGenBufXferO->PutVector(aVec)) < 0)
                    {
                        HandlePutError(GenBuf::FixFailed + status, sizeof(T) * size);
                    }
                }
                return status >= 0 ? status + 8 : status;
            }
            else
            {
                return status + 8;
            }
        }
        else
        {
            return 8;
        }
    }

    /// putValue an array of built in types into the send buffer.
    /// \return A negative number if there is an error, else returns the number of bytes put in the send buffer.
    template <class T>
    int PutArray(const T aValue[], unsigned int aArrayLen)
    {
        int status = mGenBufXferO->PutArray(aValue, aArrayLen);
        if (status < 0)
        {
            if (HandlePutError(status, sizeof(T) * aArrayLen))
            {
                if ((status = mGenBufXferO->PutArray(aValue, aArrayLen)) < 0)
                {
                    HandlePutError(GenBuf::FixFailed + status, sizeof(T) * aArrayLen);
                }
            }
        }
        return status;
    }

    /// putValue a string in the send buffer.
    /// \warning  This function will put an extra 4 bytes to the send buffer
    ///           to describe the length of the string.  Remember to account
    ///           for the these extra 4 bytes when computing the length of
    ///            your message.
    /// \return A negative number if there is an error, else returns the number of bytes put in the send buffer.
    int PutString(const std::string& aString);

    /// putValue method for raw data.  No byte swapping is performed.  This function is not used by most users.
    /// \param aDataPtr        A pointer to the user data that this function will copy to the send buffer
    /// \param aDataSizeBytes  Number of bytes this function will copy to the send buffer.
    /// \return A negative number if there is an error, else returns the number of bytes sent.
    int PutRaw(const void* aDataPtr, unsigned long aDataSizeBytes);

    /// putValue method for raw data.  No byte swapping is performed.  No data is physically
    /// moved, but the "put" pointers are incremented if there is room for the data to fit.
    /// This function is not used by most users.
    /// \param aBufferPtr      Function defines a pointer to the send buffer.  The user is responsible for
    ///                        getting the data into the send buffer.
    /// \param aDataSizeBytes  Number of bytes user will put in the send buffer.
    /// \return If there is an error, returns a negative number and aBufferPtr is set to zero,
    ///         else returns the number of bytes that are expected to be put in the send buffer and
    ///         a valid aBufferPtr.
    int GetPutBufferIfValid(void*& aBufferPtr, unsigned long aDataSizeBytes);

    /// send the data.
    /// \retval  >= 0    Data sent.  This value could optionally be the number of bytes sent.
    /// \retval <   0    An error has occurred.
    virtual int Send();

    /// Set the putValue offset.  This function is not normally used, but can be handy if the
    /// user wants to overide some existing data already placed in the send buffer.
    /// \param aOffsetBytes Should always be a positive number representing the offset from
    ///                     the position as defined by the aSeekdir parameter.
    /// \param aSeeDir      Position from which aOffsetBytes is applied.  Possible choices are:
    /// <table border="0" cellspacing="0" cellpadding="0"><tr><td width="40"></td><td width="170">
    ///                      <i> GenBuf::FromBeg     </i></td><td> Bytes past the beginning      </td></tr>
    ///     <tr><td></td><td><i> GenBuf::FromCur     </i></td><td> Bytes past current position   </td></tr>
    ///     <tr><td></td><td><i> GenBuf::FromDataEnd </i></td><td> Bytes before the data end     </td></tr>
    ///     <tr><td></td><td><i> GenBuf::FromPhyEnd  </i></td><td> Bytes before the physical end </td></tr>
    /// </table>
    /// \return A positive number if successful else returns a negative one if there is an error
    int SetPutOffset(long aOffsetBytes, GenBuf::SeekDirType aSeekDir = GenBuf::FromBeg);

    /// getValue the putValue offset.  This function is not normally used, but can be handy if the
    /// user wants to know statistics about the send buffer.
    /// \param aSeeDir      Position from which offset is computed.  Possible choices are:
    /// <table border="0" cellspacing="0" cellpadding="0"><tr><td width="40"></td><td width="170">
    ///                      <i> GenBuf::FromBeg     </i></td><td> Bytes past the beginning      </td></tr>
    ///     <tr><td></td><td><i> GenBuf::FromCur     </i></td><td> Always zero                   </td></tr>
    ///     <tr><td></td><td><i> GenBuf::FromDataEnd </i></td><td> Bytes before the data end     </td></tr>
    ///     <tr><td></td><td><i> GenBuf::FromPhyEnd  </i></td><td> Bytes before the physical end </td></tr>
    /// </table>
    /// \return Offset value.  Offset from current location is always zero.
    long GetPutOffset(GenBuf::SeekDirType aSeekDir = GenBuf::FromBeg) const;

    /// getValue the current output status.
    /// \return Status flag defined in GenBuf.
    GenBuf::IOStatus GetOutputStatus() const;

protected:
    // This class is meant to be a base class
    explicit GenO(const char* aName);

    // Set the pointer to the send buffer.
    // \param AGenBufXferO A pointer to a buffer.  This class does not take ownership of the pointer.
    // \warning <b>
    // CAUTION  CAUTION  CAUTION  CAUTION  CAUTION  CAUTION
    // The GenBufXferO pointer must be set before this class is used.
    // The GenBufXferO pointer is here as an optimization.  In a
    // more pure OO environment, the pointer would be returned from
    // a virtual function in a derived class.</b>
    void SetGenBufXferO(GenBufXferO* aGenBufXferO);

    // getValue the send buffer.
    // \return A pointer to the send buffer.
    GenBufXferO* GetGenBufXferO();

    // Clear any output error flags.
    void ClearOutputErrorStatus();

    // Give the derived class a chance to handle an error in a putValue() method.
    // \return Returns a positive number if the error was handled so another putValue
    // can be tried, else returns zero.
    virtual int HandlePutError(int aStatus, unsigned long aVariableSize);

private:
    std::string mName;
    GenBufXferO* mGenBufXferO;
    int mErrorStatus;

    // Must use protected constructor
    GenO();

    // Disallow copy and assignment
    GenO(const GenO& aSrc);
    GenO& operator=(const GenO& aRhs);
};

inline int GenO::SetPutOffset(long aOffsetBytes, GenBuf::SeekDirType aSeekDir)
{
    return mGenBufXferO->SetPutOffset(aOffsetBytes, aSeekDir);
}

inline long GenO::GetPutOffset(GenBuf::SeekDirType aSeekDir) const
{
    return mGenBufXferO->GetPutOffset(aSeekDir);
}

inline GenBuf::IOStatus GenO::GetOutputStatus() const
{
    if (mErrorStatus < GenBuf::FixFailed)
    {
        return static_cast<GenBuf::IOStatus>(mErrorStatus - GenBuf::FixFailed);
    }
    else
    {
        return static_cast<GenBuf::IOStatus>(mErrorStatus);
    }
}

inline GenBufXferO* GenO::GetGenBufXferO()
{
    return mGenBufXferO;
}

inline void GenO::ClearOutputErrorStatus()
{
    mErrorStatus = GenBuf::NoError;
}

inline void GenO::Put(const char* aValue)
{
    unsigned long length;
    int status = mGenBufXferO->Put(aValue, length);
    if (status < 0)
    {
        if (HandlePutError(status, length))
        {
            if ((status = mGenBufXferO->Put(aValue, length)) < 0)
            {
                HandlePutError(GenBuf::FixFailed + status, length);
            }
        }
    }
}

inline void GenO::Put(char* aValue)
{
    Put(const_cast<const char*>(aValue));
}

// Define inline methods that allow manipulations similar to iostreams.
template <class T>
inline GenO& operator<<(GenO& aGenO, const T& aValue)
{
    aGenO.Put(aValue);
    return aGenO;
}

#endif
