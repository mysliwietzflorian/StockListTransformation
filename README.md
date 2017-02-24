# StockListTransformation #

## Synopsis ##
A stocklist in \*.csv file format from PYTHA is transformed to a different user-defined-format to fit a program from
[MOSER PROGRAMME](https://www.optmos.at/).

## Usage ##
Basically, three files are used to transform the \*.csv:

- STL.exe	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
(executable)
- STL.conf	&nbsp;&nbsp;&nbsp;&nbsp;
(configuration file)
- \*.csv	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
(input file)

Simply, execute STL.exe in the same folder as the other files and an output file will be generated.

In case of an fatal error or warning - depending on the configuration - the transformation will be aborted and an error message displayed.

## Development ##
This project is still in active development and features are currently limited.

### Planned and in-dev Features ###

- [x] Add configuration file
- [x] Add logic for file handling
- [x] Add error handling and exceptions
- [ ] Create attributed grammar in configuration file
- [ ] Parse input file
- [ ] Add semantic actions based on configuration rules
- [ ] Use semantic actions to transform and write to output file

The current codebase can be found in the
[GitHub Repository](https://github.com/mysliwietzflorian/StockListTransformation).

## Copyright ##
&copy; 2017 Mysliwietz Florian. All rights reserved.
