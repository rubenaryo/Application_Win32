/*----------------------------------------------
Ruben Young (rubenaryo@gmail.com)
Date : 2019/10
Description : Interface for custom exception implementation
----------------------------------------------*/

#include <exception>
#include <string>

// IException is our custom way of packing error information from windows 
// or the graphics system for display upon hitting a runtime error
// In the future, more custom exception classes will follow (GFXException, etc)
class IException : public std::exception
{
public: 
    IException(int a_Line, const char* a_Filename) noexcept;
    const char* what() const noexcept override;
    const char* GetType() const noexcept;
    const std::string& GetFile() const noexcept;
    std::string GetOriginString() const noexcept;

private:
    // Store line number and file name locally for when displaying information
    int m_Line;
    std::string m_File;

protected:
    // Buffer that holds the info for what()
    mutable std::string m_buf;
};