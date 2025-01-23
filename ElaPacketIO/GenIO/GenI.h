// GenI - This base class defines the interface for the input side of GenIO.
//        It also holds a pointer to the input buffer.

#ifndef GENI_H
#define GENI_H

#include "ElaPacketIO_Export.h"

/// \file GenI.hpp
/// \brief GenI
/// \class GenI GenI.hpp
/// \brief This base class defines the interface for the input side of GenIO.
///
/// It is meant to be derived by a transport class or the GenIO class.  The transport
/// class holds the code that actually moves the data.  If the transport class only
/// receives data then it should inherit directly from this class.  If the transport class
/// sends and receives data then it should inherit from the GenIO class which inherits from
/// this class.
///
/// This class uses the concept that messages are first transported into the receive buffer by calling Receive().
/// Then the user calls the various getValue() methods as needed to remove all the data from the receive buffer.
///
/// The GenIHelper class can be used when receiving messages to correctly manage the receive buffer.

#include "GenBufXferI.h"

class ELA_PACKETIO_EXPORT GenI
{
public:
    virtual ~GenI();

    /// The Gets for built in types.
    /// \param  aValue The variable to be pulled from the receive buffer. The variable can be any built-in non
    ///         pointer type or a char* or a const char*.  For a character pointer the user is responsible
    ///         for providing enough memory for the getValue method to transfer all the characters including the
    ///         null zero.
    // Goofy microsoft compiler will not let function templates be defined
    // outside the class definition, so they are defined here.
    template <class T>
    void Get(T& aValue)
    {
        int status = mGenBufXferI->Get(aValue);
        if (status < 0)
        {
            if (HandleGetError(status, sizeof(T)))
            {
                if ((status = mGenBufXferI->Get(aValue)) < 0)
                {
                    HandleGetError(GenBuf::FixFailed + status, sizeof(T));
                }
            }
        }
    }

    // User is responsible for providing enough memory for aValue.
    // The SGI compiler requires this specialization.
    void Get(char* aValue);

    // CAUTION...  This function will get an extra 8 bytes from the stream
    //             to describe the length of the vector.  Remember to account
    //             for the these extra 8 bytes when computing the length of
    //             your message.
    // Returns a negative number if there is an error, else returns the number of
    // bytes gotten from the stream.

    /// getValue a vector of a built in type from the receive buffer.
    /// \warning  This function will get an extra 8 bytes from the receive buffer
    ///           to describe the length of the vector.  Remember to account
    ///           for the these extra 8 bytes when computing the length of
    ///           your message.
    /// \return   A negative number if there is an error, else returns the number of
    ///           bytes gotten from the receive buffer.
    template <class T, class A>
    int GetVector(std::vector<T, A>& aVec)
    {
        unsigned int size;
        Get(size);
        if (GetInputStatus() != GenBuf::NoError)
        {
            return -1;
        }

        // Use pad just in case we are dealing with doubles and then we will
        // stay on a double word boundary.  Assuming we were called when
        // on a double word boundary.
        unsigned int pad;
        Get(pad);
        if (GetInputStatus() != GenBuf::NoError)
        {
            return -1;
        }

        aVec.resize(size);
        if (size > 0)
        {
            int status = mGenBufXferI->GetVector(aVec);
            if (status < 0)
            {
                if (HandleGetError(status, sizeof(T) * size))
                {
                    if ((status = mGenBufXferI->GetVector(aVec)) < 0)
                    {
                        HandleGetError(GenBuf::FixFailed + status, sizeof(T) * aVec.size());
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

    /// getValue an array of built in types from the receive buffer.
    /// \return A negative number if there is an error, else returns the number of
    ///         bytes gotten from the receive buffer..
    template <class T>
    int GetArray(T aValue[], unsigned int aArrayLen)
    {
        int status = mGenBufXferI->GetArray(aValue, aArrayLen);
        if (status < 0)
        {
            if (HandleGetError(status, sizeof(T) * aArrayLen))
            {
                if ((status = mGenBufXferI->GetArray(aValue, aArrayLen)) < 0)
                {
                    HandleGetError(GenBuf::FixFailed + status, sizeof(T) * aArrayLen);
                }
            }
        }
        return status;
    }

    /// getValue the name of the input interface.
    /// \return The assigned name of the interface.
    const std::string& GetName() const;

    /// getValue a string from the receive buffer.
    /// \warning  This function will get an extra 4 bytes from the receive buffer
    ///           to describe the length of the string.  Remember to account
    ///           for the these extra 4 bytes when computing the length of
    ///            your message.
    /// \return A negative number if there is an error, else returns the number of bytes
    ///         gotten from the receive buffer.
    int GetString(std::string& aString);

    /// getValue method for raw data.  No byte swapping is performed.  This function is not used by most users.
    /// \param aDataPtr        A pointer to where this function will copy to the requested
    ///                        number of bytes from the receive buffer.
    /// \param aDataSizeBytes  Number of bytes this function will copy from the receive buffer.
    /// \return A negative number if there is an error, else returns the number of bytes gotten from the receive buffer..
    int GetRaw(void* aDataPtr, unsigned long aDataSizeBytes);

    /// getValue method for raw data.  No byte swapping is performed.  No data is physically
    /// moved, but the "get" pointers are incremented if there is valid data.
    /// This function is not used by most users.
    /// \param aBufferPtr      Function defines a pointer to the receive buffer.  The user is responsible for
    ///                        getting the data from the receive buffer.
    /// \param aDataSizeBytes  Number of bytes user will get from the receive buffer.
    /// \return If there is an error, returns a negative number and aBufferPtr is set to zero,
    ///         else returns the number of bytes that are expected to be gotten from the receive buffer and
    ///         a valid aBufferPtr.
    int GetGetBufferIfValid(const void*& aBufferPtr, unsigned long aDataSizeBytes);

    /// Receive the data.
    /// \param aWaitTimeInMicroSec Time period in micro seconds the function will wait for data.
    ///                            If "aWaitTimeInSec" is less than zero, then
    ///                            the function will wait forever.
    /// \retval >= 0     Data received.  This value could optionally be the number of bytes received.
    /// \retval  == -1   No data has been received.
    /// \retval <  -1    An error has occurred.
    virtual int Receive(int aWaitTimeInMicroSec);

    /// Set the getValue offset.  This function is not normally used, but can be handy if the
    /// user wants to retrieve data from the receive buffer a second time or skip over some data
    /// in the receive buffer.
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
    int SetGetOffset(long aOffsetBytes, GenBuf::SeekDirType aSeekDir = GenBuf::FromBeg);

    /// getValue the Get offset.  This function is not normally used, but can be handy if the
    /// user wants to know statistics about the receive buffer.
    /// \param aSeeDir      Position from which offset is computed.  Possible choices are:
    /// <table border="0" cellspacing="0" cellpadding="0"><tr><td width="40"></td><td width="170">
    ///                      <i> GenBuf::FromBeg     </i></td><td> Bytes past the beginning      </td></tr>
    ///     <tr><td></td><td><i> GenBuf::FromCur     </i></td><td> Always zero                   </td></tr>
    ///     <tr><td></td><td><i> GenBuf::FromDataEnd </i></td><td> Bytes before the data end     </td></tr>
    ///     <tr><td></td><td><i> GenBuf::FromPhyEnd  </i></td><td> Bytes before the physical end </td></tr>
    /// </table>
    /// \return Offset value. Offset from current location is always zero.
    long GetGetOffset(GenBuf::SeekDirType aSeekDir = GenBuf::FromBeg) const;

    /// Set the current input status
    /// \param aInputStatus Status flag defined in GenBuf.
    void SetInputStatus(GenBuf::IOStatus aInputStatus);

    /// getValue the current input status
    /// \return Status flag defined in GenBuf.
    GenBuf::IOStatus GetInputStatus() const;

protected:
    // This class is meant to be a base class.
    explicit GenI(const char* aName);

    // Set the pointer to the receive buffer
    // \param AGenBufXferI A pointer to a buffer.  This class does not take ownership of the pointer.
    // \warning <b>
    // CAUTION  CAUTION  CAUTION  CAUTION  CAUTION  CAUTION\n
    // The GenBufXferI pointer must be set before this class is used.
    // The GenBufXferI pointer is here as an optimization.  In a
    // more pure OO environment, the pointer would be returned from
    // a virtual function in a derived class.</b>
    void SetGenBufXferI(GenBufXferI* aGenBufXferI);

    // getValue the receive buffer.
    // \return A pointer to the receive buffer.
    GenBufXferI* GetGenBufXferI();

    // Clear any input error flags.
    void ClearInputErrorStatus();

    // Give the derived class a chance to handle an error in a getValue() method.
    // \return Returns a positive number if the error was handled so another getValue
    // can be tried, else returns zero.
    virtual int HandleGetError(int aStatus, unsigned long aVariableSize);

private:
    std::string mName;
    GenBufXferI* mGenBufXferI;
    int mErrorStatus;

    // Must use protected constructor
    GenI();

    // Disallow copy and assignment
    GenI(const GenI& aSrc);
    GenI& operator=(const GenI& aRhs);
};

inline const std::string& GenI::GetName() const
{
    return mName;
}

inline int GenI::SetGetOffset(long aOffsetBytes, GenBuf::SeekDirType aSeekDir)
{
    return mGenBufXferI->SetGetOffset(aOffsetBytes, aSeekDir);
}

inline long GenI::GetGetOffset(GenBuf::SeekDirType aSeekDir) const
{
    return mGenBufXferI->GetGetOffset(aSeekDir);
}

inline void GenI::SetInputStatus(GenBuf::IOStatus aInputStatus)
{
    mErrorStatus = aInputStatus;
}

inline GenBuf::IOStatus GenI::GetInputStatus() const
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

inline GenBufXferI* GenI::GetGenBufXferI()
{
    return mGenBufXferI;
}

inline void GenI::ClearInputErrorStatus()
{
    mErrorStatus = GenBuf::NoError;
}

inline void GenI::Get(char* aValue)
{
    unsigned long bytesSeen;
    int status = mGenBufXferI->Get(aValue, bytesSeen);
    if (status < 0)
    {
        if (HandleGetError(status, bytesSeen))
        {
            if ((status = mGenBufXferI->Get(aValue, bytesSeen)) < 0)
            {
                HandleGetError(GenBuf::FixFailed + status, bytesSeen);
            }
        }
    }
}

// Define inline methods that allow manipulations similar to iostreams.
template <class T>
inline GenI& operator>>(GenI& aGenI, T& aValue)
{
    aGenI.Get(aValue);
    return aGenI;
}

#endif
