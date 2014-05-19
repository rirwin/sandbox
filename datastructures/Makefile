DS_DIR=./structures

all: datastructures algorithms programs run_all_tests

datastructures: IntQueue.class IntHashTable.class

algorithms: QuickSort.class

programs: TestIntQueue.class TestQuickSort.class TestIntHashTable.class

quicksort: QuickSort.class TestQuickSort.class run_quicksort_test


IntQueue.class: $(DS_DIR)/IntQueue.java
	javac $(DS_DIR)/IntQueue.java


IntHashTable.class: $(DS_DIR)/IntHashTable.java
	javac $(DS_DIR)/IntHashTable.java


QuickSort.class: QuickSort.java
	javac QuickSort.java


TestIntQueue.class: TestIntQueue.java
	javac TestIntQueue.java


TestIntHashTable.class: TestIntHashTable.java
	javac TestIntHashTable.java


TestQuickSort.class: TestQuickSort.java
	javac TestQuickSort.java



run_all_tests: run_intqueue_test run_quicksort_test run_inthashtable_test


run_quicksort_test:
	java -ea TestQuickSort
	echo "TestQuickSort completed successfully"


run_intqueue_test:
	java -ea TestIntQueue
	echo "TestIntQueue completed successfully"


run_inthashtable_test:
	java -ea TestIntHashTable
	echo "TestIntHashTable completed successfully"


clean:
	rm *.class
	rm $(DS_DIR)/*.class
