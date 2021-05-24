# **First semester work - interpreter**
<dl>
<dt> What is interpreter? </dt>
<dd> interpreter - is a program (a kind of translator), that performs interpretation. </dd>
<dt> What is interpretation? </dt>
<dd> interpretation - is a analysis, processing and execution of the source code of a program or request. </dd>
</dl>


### What can a program do?
* arithmetic operators (add, sub, mult, div, mod) 
```
input: 1+2
output: 3

input: (8-2)%5
output: 1

input: (2+8)*(12+8) / 10
output: 20
```
* assignment operator (:=)
```
input: x := 5
output: 5

input: y := 2
output: 2

input: x * y
output: 10 
```
* logical operators
```
bit operators (&, |, ^, >>, <<)
comparison operators (>, >=, <, <=, ==, !=)
logical operators (and, or)
```
* transition operator (goto)
```
Example of input:
x := 4
y := 6
goto L
x := x + y
y := 0
L: x * 2 + y
exit

Output: 14
```

* conditional operator (if)
```
Example of input:
i := 0
if i < 3 then
    x := 1
else
    x := 0
endif
exit

Output: x=1
```

* cycles (while)
```
Example of input:
i := 1
s := 0
while i < 4 then
    s := s + i
    i := i + 1
endwhile
i := 0
exit

Output: i=0, s=6
```

* functions (def)
```
Example of input:
def indi
    x := 5
    y := 5
enddef

x := 2
indi
x + y
exit

Output: 10
```
#### To compile program you need to run Makefile by
~~~~
make
~~~~
#### To run program you need to enter
~~~~
./bin/interpreter
~~~~
#### Code must be end with the word "exit"
```
Example:

i := 1
s := 0
while i < 10 then
    if i % 2 == 0 then
        s := s - i
    else
        s := s + i
    endif
    i := i + 1
endwhile
(s + 5) * (s - 2)
exit

Output: 30
s = 5
i = 10
``` 
