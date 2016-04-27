#ifndef ALCONDUCTOR_H
#define ALCONDUCTOR_H


class AlManager
{

public:
    AlManager();
    /* use virtual otherwise linker will try to perform static linkage */
    virtual void init();
};

#endif // ALCONDUCTOR_H
