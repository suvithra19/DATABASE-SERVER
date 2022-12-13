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
#include "server.h"
#include "shared_mem.h"
using namespace CppUnit;
//using namespace std;

class TestSocketServer : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestSocketServer);
    CPPUNIT_TEST(testCreateSocket); 
    CPPUNIT_TEST(testListen);
   // CPPUNIT_TEST(testConnect);
    CPPUNIT_TEST_SUITE_END();

    public:
    	void setUp(void);
    	void tearDown(void);

    protected:
    	void testCreateSocket(void);
	void testListen(void);  
	//void testSendAndClose(void);
    private:
      SocketServer *mTestobj;
};


void TestSocketServer::testCreateSocket(void)

{	
	 //CPPUNIT_ASSERT(-1 == mTestobj->CreateSocket());
	  CPPUNIT_ASSERT(0 == mTestobj->CreateSocket());
	
}

void TestSocketServer::testListen(void)

{	
	 //CPPUNIT_ASSERT(-1== mTestobj->Listen());
	 CPPUNIT_ASSERT(0== mTestobj->Listen());
	
}


void TestSocketServer::setUp(void)
{
mTestobj = new SocketServer();


}

void TestSocketServer::tearDown(void)
{
delete mTestobj;
	}

CPPUNIT_TEST_SUITE_REGISTRATION( TestSocketServer);

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
    ofstream xmlFileOut("server.xml");
    XmlOutputter xmlOut(&collectedresults, xmlFileOut);
    xmlOut.write();

    // return 0 if tests were successful
    return collectedresults.wasSuccessful() ? 0 : 1;
}


