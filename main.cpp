#include <stdlib.h>
#include "rbtree.h"
#include <cppunit/TestCase.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>

typedef RBTree<int> IntRBTree;
typedef std::vector<int> IntVector;

class RBTreeTest : public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE(RBTreeTest);
    CPPUNIT_TEST( RunRBTreeTest );
    CPPUNIT_TEST_SUITE_END(); 
public:
    void RunRBTreeTest()
    {
        IntRBTree tree;
        int test_data[] = {2, 5};
        for (unsigned int inx = 0; inx < sizeof(test_data) / sizeof(int); ++inx)
            tree.Insert(test_data[inx]);

        int index = 0;
        for (IntRBTree::Iterator iter = tree.Begin();
                !iter.IsEnd(); iter.Next())
        {
            int value = iter.GetValue();
            CPPUNIT_ASSERT_EQUAL(value, test_data[index++]);
        }
    }
    static std::string GetSuitName()
    {
        return "rbtest";
    }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( RBTreeTest, RBTreeTest::GetSuitName() );

int main(int argc, char* argv[])
{
	CppUnit::TextUi::TestRunner runner;

	CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry(RBTreeTest::GetSuitName());

	runner.addTest( registry.makeTest() );

    runner.run();
	
    return 0;
}

