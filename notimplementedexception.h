#ifndef NOTIMPLEMENTEDEXCEPTION_H
#define NOTIMPLEMENTEDEXCEPTION_H

#include <exception>

class NotImplementedException : std::exception
{
public:
    NotImplementedException();
};

#endif // NOTIMPLEMENTEDEXCEPTION_H
