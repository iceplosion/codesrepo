@echo off
:: execute the command in the directory with pom.xml in it
mvn test

:: reference: https://maven.apache.org/guides/getting-started/index.html
:: the following command compile the tests but not execute
:: mvn test-compile