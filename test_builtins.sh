#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

test_count=0
failed_count=0

# Helper function to run tests
run_test() {
    local test_name=$1
    local command=$2
    local expected_status=$3
    ((test_count++))
    
    echo -n "Testing $test_name: "
    eval "$command"
    actual_status=$?
    
    if [ $actual_status -eq $expected_status ]; then
        echo -e "${GREEN}PASS${NC}"
    else
        echo -e "${RED}FAIL${NC}"
        echo "  Expected status: $expected_status"
        echo "  Actual status: $actual_status"
        echo "  Command: $command"
        ((failed_count++))
    fi
}

# CD Tests
echo "=== Testing CD ==="
run_test "CD to home" "cd" 0
run_test "CD with too many args" "cd dir1 dir2" 1
run_test "CD to nonexistent" "cd /nonexistent" 1
run_test "CD to file" "touch testfile; cd testfile; rm testfile" 1
run_test "CD with no permissions" "cd /root" 1
run_test "CD with empty string" "cd ''" 1
run_test "CD with tilde" "cd ~" 0
run_test "CD with dash" "cd /tmp && cd - " 0

# ECHO Tests
echo -e "\n=== Testing ECHO ==="
run_test "Basic echo" "echo hello" 0
run_test "Echo with -n" "echo -n hello" 0
run_test "Echo with multiple args" "echo hello world" 0
run_test "Echo with quotes" "echo 'hello   world'" 0
run_test "Echo with escape chars" "echo 'hello\nworld'" 0

# PWD Tests
echo -e "\n=== Testing PWD ==="
run_test "Basic pwd" "pwd" 0
run_test "PWD with args" "pwd arg" 1

# EXPORT Tests
echo -e "\n=== Testing EXPORT ==="
run_test "Basic export" "export TEST=123" 0
run_test "Export invalid name" "export 123TEST=123" 1
run_test "Export without value" "export TEST" 0
run_test "Export multiple" "export TEST1=1 TEST2=2" 0

# UNSET Tests
echo -e "\n=== Testing UNSET ==="
run_test "Basic unset" "unset TEST" 0
run_test "Unset multiple" "unset TEST1 TEST2" 0
run_test "Unset invalid" "unset 123TEST" 1

# ENV Tests
echo -e "\n=== Testing ENV ==="
run_test "Basic env" "env" 0
run_test "Env with args" "env arg" 1

# EXIT Tests
echo -e "\n=== Testing EXIT ==="
run_test "Exit without args" "exit" 0
run_test "Exit with number" "exit 42" 42
run_test "Exit with invalid number" "exit abc" 2
run_test "Exit with too many args" "exit 1 2" 1

# Special Cases
echo -e "\n=== Testing Special Cases ==="
run_test "Empty input" "" 0
run_test "Single space" " " 0
run_test "Multiple spaces" "     " 0
run_test "Pipeline" "echo hello | grep hello" 0
run_test "Redirection" "echo hello > test.txt" 0

# Environment Variable Handling
echo -e "\n=== Testing Environment Variables ==="
run_test "Echo \$PATH" "echo \$PATH" 0
run_test "Echo undefined var" "echo \$UNDEFINED_VAR" 0
run_test "Export and echo" "export TEST=123; echo \$TEST" 0

# Cleanup
rm -f test.txt

# Summary
echo -e "\n=== Test Summary ==="
echo "Total tests: $test_count"
echo "Failed tests: $failed_count"