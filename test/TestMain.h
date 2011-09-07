#ifndef TESTMAIN_H
#define TESTMAIN_H

#include "cpptest.h"

class TestMain
{
public:

    /*! \name Run methods */
    //@{
        void Run();
    //@}

    /*! \name Get singelton instance */
    //@{
        static TestMain *GetInstance();
    //@}

private:
    /*! \name Construction / Destruction */
    //@{
        /// constructor
        TestMain();

        /// destructor
        ~TestMain();
    //@}

    /*! \name Private members */
    //@{
        Test::Suite m_TestSuite;
    //@}
};

#endif // TESTMAIN_H
