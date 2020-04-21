#!/usr/bin/python3

import os
import glob


class color:
    black  = '\u001b[30m'
    red    = '\u001b[31m'
    green  = '\u001b[32m'
    yellow = '\u001b[33m'
    blue   = '\u001b[34m'
    magenta= '\u001b[35m'
    cyan   = '\u001b[36m'
    white  = '\u001b[37m'
    grey   = '\u001b[90m'
    reset  = '\u001b[0m'

def parse_test(test):
    t = open(test)
    pass

def run_test(test):
    parse_test(test)
    return 1

def main():
    print(f'{color.magenta}=== Minimake Testsuite ==={color.reset}')
    print(f'{color.grey}Author: william.lin <william.lin@epita.fr>{color.reset}')


    # Get all the tests recursively
    script_dir = os.path.dirname(__file__)
    test_files = [f for f in 
            glob.glob(script_dir + '/Makefile.*', recursive=True)]

    failed = 0
    passed = 0
    current_test = 0
    total_tests = len(test_files)

    for test in test_files:
        test_name = os.path.basename(test)[len('Makefile.'):]
        current_test += 1
        result = run_test(test)
        if result == 1:
            passed += 1
            print(f'[{color.green}✔{color.reset}] ', end='')
        else :
            failed += 1
            print(f'[{color.red}✗{color.reset}] ', end='')
            # Explain why it failed

        print(test_name.ljust(50, ' '), end='')
        print(f'[{current_test:03}/{total_tests}]')

    print(f'{color.magenta}=== Summary ==={color.reset}')
    print(f'Total tests: {current_test}')
    print(f'Tests passed: {color.green}{passed}{color.reset}')
    print(f'Tests failed: {color.red}{failed}{color.reset}')

    if passed == current_test:
        print('All test passed ! Good job !')
    else:
        print('Some test failed ! Keep working !')


main()
