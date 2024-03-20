# Literally LINUX
Student ID : 401171593

# Build and Run?
## Build 
`./build.sh`
## Commands
`bin/command_exec.out`
## VIIM     
`bin/viim.out`

# Usage
*file address should start with /*
## Commands
### createfile
`createfile –file <file name and address>`

creates a new file in the given address.
### insertstr
`insertstr –file <file name> –str <str> —pos <line no>:<start position>`

inserts str in the given position.
### cat
`cat –file <file name>`

prints contents of the given file.
### removestr
`removestr –file <file name> –pos <line no>:<start position> -size <number of characters to remove> -f -b <forward or backward>`

removes text in the given range.
### copy
`copystr –file <file name> –pos <line no>:<start position> -size <num-ber of characters to copy> -f -b <forward or backward>`

copies text in the given range to clipboard.
### cut
`cutstr –file <file name> –pos <line no>:<start position> -size <number of characters to cut> -f -b <forward or backward>`

cuts text in the given range to clipboard.
### paste
`pastestr –file <file name> –pos <line no>:<start position>`

pastes clipboard to given position.
### undo
`undo –file <file>`

used to undo changes applied to the chosen file using the tool.
### tree
`tree <depth>`

draws file tree of root directory up to given depth.
if depth is -1 then it draws entire file tree.
### compare
`compare <file1> <file2>`
compares given files.
### auto-indent
`auto-indent <file>`

fixes indentation of c-like codes.
### grep
`grep [-c | -l] –str <pattern> –files [<file1> <file2> <file3> …]`

searches <pattern> string in given files and prints its locations.
#### -c option
only counts occurences.
#### -l option
only prints containing filenames.
### find
`find –str <str> –file <file name> [-count | -at <nth occurence> | -all] -byword`

searches <str> string in the given file and prints its location.
#### -count option
only counts occurences.
#### -at option
prints the nth occurence location.
#### -all option
prints all occurences.
#### -byword option
prints location based on word count instead of character count.
### replace
`replace –str1 <str> –str2 <str> –file <file name> [-at <num> | -all]`

replaces first str1 occurence with str2.
#### -at option
replaces nth str1 occurence with str2.
#### -all option
replaces all str1 occurences with str2.
## VIIM
mostly similar to the original vim.
### quit
`:quit`

closes the editor.
### saving
`:save`

saves current buffer to already specified file path.

`:saveas <path>`

saves current buffer to the file at the given path.
### open
`:open <path>`

opens the file at the given path.
### find
`/<pattern>`
finds and highlightes occurences of pattern in current buffer.
### replace
`:replace –str1 <str> –str2 <str> [-at <num> | -all]`

replaces first str1 occurence with str2.
#### -at option
replaces nth str1 occurence with str2.
#### -all option
replaces all str1 occurences with str2.
### auto-indent
fixes indentation of the current buffer using '=' key.
### other commands
`:name_of_command`

you can use any command from the previous part in this format and the result of them apear inside the editor.
