/*----------------------------------------------
Ruben Young (rubenaryo@gmail.com)
Date : 2019/10
Description : Implementation of custom exception functions
----------------------------------------------*/
#include "IException.h"
#include <sstream>

// Simple constructor to initialize member fields
IException::IException(int a_Line, const char* a_Filename) noexcept
    : m_Line( a_Line ), m_File(a_Filename) {}

// Collects information about the error and returns the address of our buffer
const char* IException::what() const noexcept
{
    // Write exception information into an output string object
    std::ostringstream oss;
    oss << GetType() << std::endl << GetOriginString();
    
    // Store the value of the string into local buffer
    m_buf = oss.str();

    // Return pointer to c string of the object
    return m_buf.c_str();
}

const char* IException::GetType() const noexcept
{
    // TODO: make GetType() a pure virtual function so other systems can handle
    // their own exceptions their own preferred way
    return "Standard Exception";
}

// Accessor for filename
const std::string& IException::GetFile() const noexcept
{
    return m_File;
}

// Gather information about where the exception happened
std::string IException::GetOriginString() const noexcept
{
    // Write location info into a std::string object 
    std::ostringstream oss;
    oss << "[File] " << m_File << std::endl
        << "[Line] " << m_Line;
    return oss.str();
}
