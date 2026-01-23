#include <gtest/gtest.h>
#include "fibwrapper.h"
#include "LRU.h"
#include "LFU.h"
#include "node.h"
#include <stdexcept>

TEST(NodeTest, ConstructorInitializesCorrectly) {
    Node node(1, 42);
    EXPECT_EQ(node.key, 1);
    EXPECT_EQ(node.value, 42);
    EXPECT_EQ(node.cnt, 1);
    EXPECT_EQ(node.next, nullptr);
    EXPECT_EQ(node.prev, nullptr);
}

class LRUCacheTest : public ::testing::Test {
protected:
    void SetUp() override {
        lru = new LRUCache(3);
    }

    void TearDown() override {
        delete lru;
    }

    LRUCache* lru;
};

TEST_F(LRUCacheTest, PutAndGet) {
    lru->put(1, 10);
    lru->put(2, 20);
    lru->put(3, 30);

    EXPECT_EQ(lru->get(1), 10);
    EXPECT_EQ(lru->get(2), 20);
    EXPECT_EQ(lru->get(3), 30);
}

TEST_F(LRUCacheTest, GetNonExistentKey) {
    EXPECT_EQ(lru->get(1), -1);
}

TEST_F(LRUCacheTest, LRUEviction) {
    lru->put(1, 10);
    lru->put(2, 20);
    lru->put(3, 30);
    lru->put(4, 40); 

    EXPECT_EQ(lru->get(1), -1);
    EXPECT_EQ(lru->get(2), 20);
    EXPECT_EQ(lru->get(3), 30);
    EXPECT_EQ(lru->get(4), 40);
}

TEST_F(LRUCacheTest, OperatorBracket) {
    lru->put(1, 10);
    EXPECT_EQ((*lru)[1], 10);
    EXPECT_EQ((*lru)[2], -1);
}

TEST_F(LRUCacheTest, UpdateExistingKey) {
    lru->put(1, 10);
    lru->put(1, 20);
    EXPECT_EQ(lru->get(1), 20);
}

class LFUCacheTest : public ::testing::Test {
protected:
    void SetUp() override {
        lfu = new LFUCache(3);
    }

    void TearDown() override {
        delete lfu;
    }

    LFUCache* lfu;
};

TEST_F(LFUCacheTest, PutAndGet) {
    lfu->put(1, 10);
    lfu->put(2, 20);
    lfu->put(3, 30);

    EXPECT_EQ(lfu->get(1), 10);
    EXPECT_EQ(lfu->get(2), 20);
    EXPECT_EQ(lfu->get(3), 30);
}

TEST_F(LFUCacheTest, GetNonExistentKey) {
    EXPECT_EQ(lfu->get(1), -1);
}

TEST_F(LFUCacheTest, LFUEviction) {
    lfu->put(1, 10);
    lfu->put(2, 20);
    lfu->put(3, 30);

    lfu->get(2);
    lfu->get(2);
    lfu->get(2);

    lfu->put(4, 40);

    EXPECT_EQ(lfu->get(2), 20);

    EXPECT_EQ(lfu->get(4), 40);

    bool key1Exists = (lfu->get(1) != -1);
    bool key3Exists = (lfu->get(3) != -1);

    EXPECT_TRUE((key1Exists && !key3Exists) || (!key1Exists && key3Exists));

    int count = 0;
    if (lfu->get(1) != -1) count++;
    if (lfu->get(2) != -1) count++;
    if (lfu->get(3) != -1) count++;
    if (lfu->get(4) != -1) count++;
    EXPECT_EQ(count, 3);
}

TEST_F(LFUCacheTest, OperatorBracket) {
    lfu->put(1, 10);
    EXPECT_EQ((*lfu)[1], 10);
    EXPECT_EQ((*lfu)[2], -1);
}

TEST_F(LFUCacheTest, UpdateExistingKey) {
    lfu->put(1, 10);
    lfu->put(1, 20);
    EXPECT_EQ(lfu->get(1), 20);
}

TEST_F(LFUCacheTest, FrequencyUpdate) {
    lfu->put(1, 10);
    lfu->put(2, 20);

    lfu->get(1);
    lfu->get(1);


    lfu->put(3, 30);

    lfu->put(4, 40);

    EXPECT_EQ(lfu->get(1), 10); 
    EXPECT_EQ(lfu->get(4), 40); 

    bool key2Exists = (lfu->get(2) != -1);
    bool key3Exists = (lfu->get(3) != -1);

    EXPECT_TRUE((key2Exists && !key3Exists) || (!key2Exists && key3Exists));
}

class FibonacciWrapperTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(FibonacciWrapperTest, ConstructorValidTypes) {
    EXPECT_NO_THROW(FibonacciWrapper wrapper1(1));
    EXPECT_NO_THROW(FibonacciWrapper wrapper2(2));
}

TEST_F(FibonacciWrapperTest, ConstructorInvalidType) {
    EXPECT_THROW(FibonacciWrapper wrapper(0), std::invalid_argument);
    EXPECT_THROW(FibonacciWrapper wrapper(3), std::invalid_argument);
}

TEST_F(FibonacciWrapperTest, CalculateWithLRU) {
    FibonacciWrapper calculator(1);

    EXPECT_EQ(calculator.calculate(0), 0);
    EXPECT_EQ(calculator.calculate(1), 1);
    EXPECT_EQ(calculator.calculate(2), 1);
    EXPECT_EQ(calculator.calculate(3), 2);
    EXPECT_EQ(calculator.calculate(4), 3);
    EXPECT_EQ(calculator.calculate(5), 5);
    EXPECT_EQ(calculator.calculate(6), 8);
    EXPECT_EQ(calculator.calculate(7), 13);
}

TEST_F(FibonacciWrapperTest, CalculateWithLFU) {
    FibonacciWrapper calculator(2);

    EXPECT_EQ(calculator.calculate(0), 0);
    EXPECT_EQ(calculator.calculate(1), 1);
    EXPECT_EQ(calculator.calculate(2), 1);
    EXPECT_EQ(calculator.calculate(3), 2);
    EXPECT_EQ(calculator.calculate(4), 3);
    EXPECT_EQ(calculator.calculate(5), 5);
    EXPECT_EQ(calculator.calculate(6), 8);
    EXPECT_EQ(calculator.calculate(7), 13);
}

TEST_F(FibonacciWrapperTest, CalculateNegativeNumber) {
    FibonacciWrapper calculator(1);
    EXPECT_THROW(calculator.calculate(-1), std::invalid_argument);
}

TEST_F(FibonacciWrapperTest, CacheReuse) {
    FibonacciWrapper calculator(1);

    int firstResult = calculator.calculate(10);

    int secondResult = calculator.calculate(10);

    EXPECT_EQ(firstResult, secondResult);
    EXPECT_EQ(firstResult, 55); 
}

TEST_F(FibonacciWrapperTest, LargeNumberWithCache) {
    FibonacciWrapper calculator(1);

    EXPECT_EQ(calculator.calculate(15), 610);
    EXPECT_EQ(calculator.calculate(15), 610); 

TEST(IntegrationTest, MultipleFibonacciCalculations) {
    FibonacciWrapper lruCalculator(1);
    FibonacciWrapper lfuCalculator(2);

    for (int i = 0; i <= 10; ++i) {
        EXPECT_EQ(lruCalculator.calculate(i), lfuCalculator.calculate(i));
    }
}

TEST(IntegrationTest, CachePerformance) {
    FibonacciWrapper calculator(1);

    std::vector<int> results1, results2;

    for (int i = 0; i <= 10; ++i) {
        results1.push_back(calculator.calculate(i));
    }

    for (int i = 0; i <= 10; ++i) {
        results2.push_back(calculator.calculate(i));
    }

    EXPECT_EQ(results1, results2);
}

TEST(FibonacciCorrectnessTest, First20Numbers) {
    FibonacciWrapper calculator(1);

    int expected[] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181};

    for (int i = 0; i < 20; ++i) {
        EXPECT_EQ(calculator.calculate(i), expected[i]);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
