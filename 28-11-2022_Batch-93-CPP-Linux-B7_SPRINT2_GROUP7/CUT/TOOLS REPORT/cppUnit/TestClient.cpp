#include <iostream>
#include <list>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <netinet/in.h>
#include "client.h"

using namespace CppUnit;
//using namespace std;

class TestSocketClient : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestSocketClient);
    CPPUNIT_TEST(testCreateSocket); 
    CPPUNIT_TEST(testSendAndClose);
    CPPUNIT_TEST(testConnect);
    CPPUNIT_TEST_SUITE_END();

    public:
    	void setUp(void);
    	void tearDown(void);

    protected:
    	void testCreateSocket(void);
	void testConnect(void);  
	void testSendAndClose(void);
    private:
      SocketClient *mTestobj;
};


void TestSocketClient::testCreateSocket(void)

{	
	 CPPUNIT_ASSERT(0 == mTestobj->CreateSocket());
	// CPPUNIT_ASSERT(-1 == mTestobj->CreateSocket());
	
}

void TestSocketClient::testConnect(void)

{	
	 //CPPUNIT_ASSERT(-1 == mTestobj->Connect());
	 CPPUNIT_ASSERT(0 == mTestobj->Connect());
	
}
void TestSocketClient::testSendAndClose(void)

{	
	 CPPUNIT_ASSERT(0 == mTestobj->SendAndClose());
	  CPPUNIT_ASSERT(-1 == mTestobj->SendAndClose());
	
}



void TestSocketClient::setUp(void)
{
mTestobj = new SocketClient();


}

void TestSocketClient::tearDown(void)
{
delete mTestobj;
	}

CPPUNIT_TEST_SUITE_REGISTRATION( TestSocketClient);

int main(int argc, char* argv[])
{
    // informs test-listener about testresults
    CPPUNIT_NS::TestResult testresult;

    // register listener for collecting the test-results
    CPPUNIT_NS::TestResultCollector collectedresults;
    testresult.addListener (&collectedresults);

    // register listener for per-test progress output
    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener (&progress);

    // insert test-suite at test-runner by registry
    CPPUNIT_NS::TestRunner testrunner;
    testrunner.addTest (CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest ());
    testrunner.run(testresult);

    // output results in compiler-format
    CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
    compileroutputter.write ();

    // Output XML for Jenkins CPPunit plugin
    ofstream xmlFileOut("client.xml");
    XmlOutputter xmlOut(&collectedresults, xmlFileOut);
    xmlOut.write();

    // return 0 if tests were successful
    return collectedresults.wasSuccessful() ? 0 : 1;
}


