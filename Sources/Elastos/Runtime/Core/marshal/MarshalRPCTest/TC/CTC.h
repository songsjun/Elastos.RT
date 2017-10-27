#ifndef __TC_H
# define __TC_H

# include "_CTC.h"



CarClass(CTC) {
public:
    CTC(void);

    ~CTC();

    CARAPI constructor(Int32 i);

    CARAPI Get(Int32 *i);

    CARAPI Set(Int32 i);

    CARAPI Increment(void);

    CARAPI Decrement(void);

private:
    Int32 m_i;
};

#endif
