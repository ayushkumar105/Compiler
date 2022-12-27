# The Peep Compiler For Hatchling Machine Language

## Getting Started
To get started with the compiler, you'll need to clone the repository to your local machine.

Here's how:

1. Make sure you have Git installed on your system and are running a Unix environment.
    
2. Open a terminal window and navigate to the directory where you want to clone the repository.
    
3. Run the following command:

    `git clone https://github.com/ayushkumar105/Compiler.git`
    
4. Once the repository has been cloned, navigate into the directory with the following command:    
    
    `cd Compiler`
    
5. Type `make`

6. Type `./peepcc ex1.peep`

This will run a program written in the Peep programming language that takes an interger n as the input and prints out the sum from 1 to n. There are two other example files written in the Peep programming language that can be run. The second example file, called ex2.peep, takes in two intergers from the terminal and prints out the larger of the two integers. The third example file, called ex3.peep, takes an input and prints out its square and then it repeats this process until the user inputs -9999. These examples can be run using the command `./peepcc FILENAME.peep`, replace `FILENAME` with the file you want to run. You can also write your own Peep program in a file with the extension `.peep` and run it with the same command mentioned above. Below is a detalied description of the Peep programming language.  
    
## Introduction
This compiler (Peep Compiler) is a simple, yet powerful tool for converting source code written in Peep programming language into Hatching machine code and the Hatchling machine code's execution can be simulated in C.

## The Peep Programming Language
### Commands
Peep is a simple, yet powerful, high-level language similar to early versions of the popular language BASIC. Every Peep statement consists of a line number and a Peep instruction. Line numbers must appear in ascending order. Each instruction begins with one of the following Peep commands: `rem`, `input`, `let`, `print`, `goto`, `if...goto`, or `end` 

| **Command** | **Example Statement**     | **Description**                                                                                                                                                    |
|-------------|---------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `rem`       | `50 rem this is a remark` | Any text following the command **rem** is for documentation purposes only and is ignored by the compiler.                                                          |
| `input`     | `30 input x`              | Display a question mark prompt to prompt the user to enter an integer.  Read that integer from the keyboard and store the integer in **x**.                        |
| `let`       | `80 let u = 4 * (j - 56)` | Assign **u** the value of **4 * (j - 56)**.  Note that an arbitrarily complex expression can appear to the right of the equal sign.                                |
| `print`     | `10 print w`              | Display the value of **w**.                                                                                                                                        |
| `goto`      | `70 goto 45`              | Transfer control to line 45.                                                                                                                                       |
| `if/goto`   | `35 if y == z goto 80`    | Compare **y** and **z** for equality and transfer  program control to line **80** if the condition is true; otherwise, continue execution with the next statement. |
| `end`       | `99 end`                  | Terminate program execution.                                                                                                                                       |

All commands except end can be used repeatedly. Peep evaluates only integer expressions using the `+`, `-`, `*`, `/`, and `%` operators. These operators have the same precedence as in C. Parentheses can be used to change the order of evaluation of an expression.


### Characters, Variables, and Integers
Peep compiler recognizes only lowercase letters. All characters in a Peep file should be lowercase (uppercase letters result in a syntax error unless they appear in a rem statement in which case they are ignored). A variable name is a single letter. Peep does not allow descriptive variable names, so variables should be explained in remarks to indicate their use in the program. Peep uses only integer variables. Peep does not have variable declarations—merely mentioning a variable name in a program causes the variable to be declared and initialized to zero automatically. The syntax of Peep does not allow string manipulation (reading a string, writing a string, comparing strings, etc.). If a string is encountered in a Peep program (after a command other than rem), the compiler generates a syntax error. Our compiler will assume that Peep programs are entered correctly. 

### Control Flow and Relational Operaators
Peep uses the conditional `if...goto` statement and the unconditional `goto` statement to alter the flow of control during program execution. If the condition in the `if...goto` statement is true, control is transferred to a specific line of the program. The following relational and equality operators are valid in an `if...goto` statement: `<`, `>`, `<=`, `>=`, `==` or `!=`. The precedence of these operators is the same as in C.

## Example 1
The first program reads two integers from the keyboard, stores the values in variables `a` and `b`, and computes and prints their sum (stored in variable `c`).

```basic
10 rem determine and print the sum of two integers
15 rem
20 rem input the two integers
30 input a
40 input b
50 rem add integers and store result in c
60 let c = a + b
65 rem
70 rem print the result
80 print c
90 rem terminate program execution
99 end
```

### Example 2
Here is another example that determines and prints the larger of two integers. The integers are input from the keyboard and stored in `s` and `t`. The `if...goto` statement tests the condition `s >= t`. If the condition is true, control is transferred to line 90 and `s` is output; otherwise, `t` is output and control is transferred to the `end` statement in line 99 where the program terminates.

```basic
10 rem determin the larger of two integers
20 input s
30 input t
32 rem
35 rem test if s >= t
40 if s >= t goto 90
45 rem
50 rem t is greater than s, so print t
60 print t
70 goto 99
75 rem
80 rem s is greater than or equal to t, so print s
90 print s
99 end
```

### Example 3: Looping and Repetition Structures
Peep does not provide a repetition structure (such as C’s `for`, `while` or `do...while`). However, Peep can simulate each of C’s repetition structures using the `if...goto` and `goto` statements. Here is an example of a sentinel-controlled loop to calculate the squares of several integers. Each integer is input from the keyboard and stored in variable `j`. If the value entered is the sentinel **-9999**, control is transferred to line 99 where the program terminates. Otherwise, `k` is assigned the square of `j`, `k` is output to the screen and control is passed to line 20 where the next integer is input.

```basic
10 rem calculate the squares of several integers
20 input j
23 rem
25 rem test for sentinel value
30 if j == -9999 goto 99
33 rem
35 rem calculate square of j and assign result to k
40 let k = j * j
50 print k
53 rem
55 rem loop to get next j
60 goto 20
99 end
```







