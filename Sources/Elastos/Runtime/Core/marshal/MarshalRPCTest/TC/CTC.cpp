
#include <stdio.h>

#include "CTC.h"



CTC::CTC(void)
{
}

CTC::~CTC()
{
}

ECode CTC::constructor(Int32 i)
{
    m_i = i;
    return NOERROR;
}

ECode CTC::Get(Int32 *i)
{
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    if (i == NULL)
        return NOERROR;
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    *i = m_i;
    return NOERROR;
}

ECode CTC::Set(Int32 i)
{
    m_i = i;
    return NOERROR;
}

ECode CTC::Increment(void)
{
    ++m_i;
    return NOERROR;
}

ECode CTC::Decrement(void)
{
    --m_i;
    return NOERROR;
}

