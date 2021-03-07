@echo off
:: test command: this.bat
:: test command: this.bat 123
:: test command: this.bat 123 456
:: references https://blog.csdn.net/hoohoney/article/details/7545986
:: references https://blog.csdn.net/u012474286/article/details/18220719
:: %1 is the first parameter
:: %2 is the second parameter
:: note: don't add space after or before '='
set first=%1
set second=%2
echo %first%
:: check first parameter
:: use "if /?" in command line to get help
if "%first%"=="" (
    echo missing parameter
:: exit [/B] [exitCode]
    exit /B 1
) else (
    echo get parameter %first%
)

:: check second parameter
if "%second%" neq "" (
    echo too many parameters
    exit /B 1
) else (
    echo ok.
)

echo get %first%