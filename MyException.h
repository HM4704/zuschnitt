#pragma once

class CMyException
{
private:
    unsigned int nSE;
public:
    CMyException() {}
    CMyException( unsigned int n ) : nSE( n ) {}
    ~CMyException() {}
    unsigned int getSeNumber() { return nSE; }
};

void MyExceptInit(void);
