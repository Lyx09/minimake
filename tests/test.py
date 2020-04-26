#!/usr/bin/python3

import os
import glob
import subprocess
import sys

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


def compare_result(result):
    mk = result[0]
    mnk = result[1]

    if mk.returncode and mnk.returncode:
        print(f'    ↳ {color.red}Return code does not match.')
        print(f'    {color.reset}Expected {color.green}{mk.returncode}')
        print(f'    {color.reset}But got {color.red}{mnk.returncode}{color.reset}')
        return

    if len(mk.stdout) > 0 == len(mnk.stdout) > 0:
        print(f'    ↳ {color.red}Stdout does not match.{color.reset}')
        print(f'    Expected {color.green}{mk.stdout}{color.reset}')
        print(f'    But got {color.red}{mnk.stdout}{color.reset}')
        return

    if len(mk.stderr) > 0 == len(mnk.stderr) > 0:
        print(f'    ↳ {color.red}Stderr does not match.')
        print(f'    Expected {color.green}{mk.stderr}{color.reset}')
        print(f'    But got {color.red}{mnk.stderr}{color.reset}')
        return

    return


def run_test(minimake, test):
    # TODO: Parse test (For env variables, etc...)
    t = open(test, 'r')

    mk = subprocess.run(['make', '--no-print-directory', '-f', test], capture_output=True)
    mnk = subprocess.run([minimake, '-f', test], capture_output=True)

    # TODO: Do some cleaning ?
    return (mk, mnk)


def main():
    print(f'{color.magenta}=== Minimake Testsuite ==={color.reset}')
    print(f'{color.grey}Author: william.lin <william.lin@epita.fr>{color.reset}')


    # TODO: Make a better opt parser later
    minimake = ''
    if len(sys.argv) < 2:
        print(f'{color.red}Error, missing minimake file{color.reset}')
        exit()
    else:
        minimake = os.path.join(os.getcwd(), sys.argv[1])


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
        result = run_test(minimake, test)

        if (result[0].returncode != result[1].returncode or
                (len(result[0].stdout) > 0 == len(result[1].stdout) > 0) or
                (len(result[0].stderr) > 0 == len(result[1].stderr) > 0)):
            failed += 1
            print(f'[{color.red}✗{color.reset}] ', end='')
        else :
            passed += 1
            print(f'[{color.green}✔{color.reset}] ', end='')

        print(test_name.ljust(50, ' '), end='')
        print(f'[{current_test:03}/{total_tests}]')
        compare_result(result)

    print(f'{color.magenta}=== Summary ==={color.reset}')
    print(f'Total tests: {current_test}')
    print(f'Tests passed: {color.green}{passed}{color.reset}')
    print(f'Tests failed: {color.red}{failed}{color.reset}')

    print('Conclusion: ', end='')
    if passed == current_test:
        print('All test passed ! Good job !')
    elif passed == 0:
        print('Did you even try ?')
    else:
        print('Some test failed ! Keep working !')


main()
