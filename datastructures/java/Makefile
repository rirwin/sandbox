DS_DIR=./structures

all: IntQueue.class TestIntQueue.class run_all_tests


IntQueue.class: $(DS_DIR)/IntQueue.java
	javac $(DS_DIR)/IntQueue.java

TestIntQueue.class: TestIntQueue.java
	javac TestIntQueue.java

run_all_tests: run_int_queue_test

run_int_queue_test:
	java -ea TestIntQueue
	echo "TestIntQueue completed successfully"
