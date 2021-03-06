#include <stdlib.h>
#include "rbtree.hxx"
#include <vector>
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
        int test_data[] = {2, 5, 3, 9, 15, 6};
        IntVector sortedData;
        for (unsigned int inx = 0; inx < sizeof(test_data) / sizeof(int); ++inx)
        {
            tree.Insert(test_data[inx]);
            sortedData.push_back(test_data[inx]);
        }

        CPPUNIT_ASSERT_EQUAL(tree.Check(), true);
        std::sort(sortedData.begin(), sortedData.end());
        CheckOutput(tree, sortedData);

        IntRBTree::Iterator iter = tree.Find(5);
        CPPUNIT_ASSERT_EQUAL(iter.IsEnd(), false);

        tree.Erase(15);
        sortedData.erase(std::remove(sortedData.begin(), sortedData.end(), 15));
        CheckOutput(tree, sortedData);
        CPPUNIT_ASSERT_EQUAL(tree.Check(), true);
    }
    void CheckOutput(IntRBTree& tree, const IntVector& sortedData)
    {
        int index = 0;
        for (IntRBTree::Iterator iter = tree.Begin();
             !iter.IsEnd(); iter = iter.Next())
        {
            int value = iter.GetValue();
            CPPUNIT_ASSERT_EQUAL(value, sortedData[index++]);
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

