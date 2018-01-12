#ifndef DRAWINGREDIFINEDEXCEPTION_H
#define DRAWINGREDIFINEDEXCEPTION_H

#include <stdexcept>

class DrawingRedifinedException : public std::runtime_error
{
public:
    DrawingRedifinedException();
};

#endif // DRAWINGREDIFINEDEXCEPTION_H
