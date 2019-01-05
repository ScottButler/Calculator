# Calculator
Calculator made by interfacing ATMEGA644A with LCD and 4 X 4 button matrix
## Progress (12/16):
- Button matrix debounce tested and works
- Button multiplexing routine fixed and tested
### What the program currently does:
- Enter first string to LCD through button matrix
- Press '+', this terminates the first string and begins the second string
- enter second string
- Press Enter once ==> Print first string on line 2 of LCD
- Press Enter again ==> Clears display, LCD position goes to line 1, position 0, process repeats

## Progress (1/5):
- All buttons have a function on 4 X 4 matrix
- Decimal point added
### What the program currently does:
- Enter string of numbers
- Press (+,-,/,*), first string terminated, second string begins
- Press Enter, second string terminated
- Decimal point location for both strings is saved
- Strings with decimal points are saved, no math is done yet
