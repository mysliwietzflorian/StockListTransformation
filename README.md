# StockListTransformation #

## Synopsis ##
A stocklist in \*.csv file format from PYTHA is transformed to a different user-defined-format to fit a program from
[MOSER PROGRAMME](https://www.optmos.at/).

[//]: # (## Documentation)
[//]: # (The full documentation - only available in German - can be found)
[//]: # ([here](https://github.com/mysliwietzflorian/StockListTransformation/).)

## Usage ##
Basically, three files are used to transform the \*.csv:

- SLT.exe	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
(executable)
- SLT.conf	&nbsp;&nbsp;&nbsp;&nbsp;
(configuration file)
- \*.csv	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
(input file)

Simply, execute SLT.exe in the same folder as the other files and an output file will be generated.

In case of a fatal error or warning - depending on the configuration - the transformation will be aborted and an error message displayed.

## Development ##
This project is still in active development and features are currently limited.

### Planned and in-dev Features ###
In next release:
- [x] Modify program that in- and output files can be in separate directories
- [x] Write defined header to output file
- [x] Parse input file
- [x] Use semantic actions to transform and write to output file

in-def features:
- [ ] Recognize umlauts and the sharp s
- [ ] Implement semantic actions based on configuration rules

The current codebase can be found on
[GitHub](https://github.com/mysliwietzflorian/StockListTransformation).

## Copyright ##
&copy; 2017 Mysliwietz Florian. All rights reserved.
