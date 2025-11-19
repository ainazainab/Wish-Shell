# Test Commands

## BATCH MODE TESTS

// Create test file and run
echo -e 'exit' > test.txt && ./wish test.txt
// Expected: [BATCH MODE] then exits immediately

echo -e 'path /bin\necho hello\nexit' > test.txt && ./wish test.txt
// Expected: [BATCH MODE] then prints "hello"

echo -e 'cd /tmp\nexit' > test.txt && ./wish test.txt
// Expected: [BATCH MODE] then exits (cd works silently)

echo -e 'cd\nexit' > test.txt && ./wish test.txt
//Expected: [BATCH MODE] then "An error has occurred" then exits

echo -e 'exit bad\nexit' > test.txt && ./wish test.txt
//expected: [BATCH MODE] then "An error has occurred" then exits

echo -e 'path /bin\nls > out.txt\nexit' > test.txt && ./wish test.txt
// Expected: [BATCH MODE] then creates out.txt file

echo -e 'path /bin\necho h & echo w\nexit' > test.txt && ./wish test.txt
// Expected: [BATCH MODE] then prints both "h" and "w"

// Run automated tests (may have exit code capture issues with test framework)
bash test-wish.sh
// Tests run but are failing due to test framework exit code, i rewrote the code for tests to comapere capture method, not shell bugs

// Manual batch tests above work correctly i

## INTERACTIVE MODE TESTS

// Start interactive shell
./wish
// Expected: Shows Aina welcome message, then [INTERACTIVE MODE], then wish> prompt

exit
// Expected: Shell exits immediately

path /bin
// Expected: Sets path (silentll)

ls
// Expected: Lists files in current directory

cd /tmp
// Expected: Changes to /tmp directory

cd
// Expected: Prints "An error has occurred"

exit bad
// Expected: Prints "An error has occurred", stays in shell

path
// Expected: Clears path (silenyt)

ls
// Expected: Prints "An error has occurred" (no path set)

path /bin
// Expected: Sets path again

ls > output.txt
// Expected: Creates output.txt file with ls output

echo hello & echo world
// Expected: Prints both "hello" and "world"

path /bin /usr/bin
// Expected: Sets path to both directories

cd nonexistent_dir
// Expected: Prints "An error has occurred"

