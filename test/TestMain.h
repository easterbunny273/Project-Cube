#pragma once
#ifndef __TESTMAIN_PROJECTCUBE_TEST_HEADER
#define __TESTMAIN_PROJECTCUBE_TEST_HEADER

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

#endif // __TESTMAIN_PROJECTCUBE_TEST_HEADER
