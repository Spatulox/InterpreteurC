# InterpreteurC

## Interpreteur only programing in C
> - version : C11
> - Only with Standard Libraries

## Compilation

```shell
# to create executable
cd /build
cmake ..
```

```shell
# to run
cd /build
make
../prog
```

## Basic Functionnalities
> - All basic operation (+, -, *, /, %, ^)
> - Advanced operation (+=, -=, *=, /=, %=, var++, var--)
> - Comments (with "#")
> - Different type : int, float, string
> - "print" function
> - "exit" function
> - Variables
> - Variables scope
> - Basic array (declaration, index access) (can't change an index value)
> - Different type in one array (int, float, string)

## Weird functionnalities :
### String
> - a="test"
> - print(a/2) => ["te", "st"] (split the word in two)
>
> - a="test"
> - print(a%2"t") => 2 (because two "t" in "test")
>
> - a="test"
> - print(a-"t") => es (delete all the "t" in the string)
>
> - Print the corresponding AST by adding "$" at the end of the line
> - Print the corresponding Lexer by adding "~" at the end of the line

## GITHUB Link
[Interpreteur C](https://github.com/Spatulox/InterpreteurC)
