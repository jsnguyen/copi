# copi

## Requirements
* [mpfr](https://www.mpfr.org/)

## Description
Computes an arbitrary number of digits of pi. Implementation details are inside `copi.c`.

## Usage
 ```
 make
 ./copi.exe <n digits of pi>
 ```

## Sample Output
```
./copi.exe 100
Requested # digits of precision: 100
   Actual # digits of precision: 156

****************************
PI to 100 digits
****************************
3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067
```
