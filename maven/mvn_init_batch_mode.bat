@echo off
if "%1"=="/?" (
:: ^ is escape command
:: reference: https://stackoverflow.com/questions/251557/escape-angle-brackets-in-a-windows-command-prompt
    echo this.bat ^<groupId^> ^<artifactId^>
    echo example: this.bat com.test.app test-app
    exit /B 0
)
if "%1"=="" (
    echo missing groupId
    goto error
)
if "%2" == "" (
    echo missing artifactId
    goto error
)
:: reference: https://maven.apache.org/guides/getting-started/index.html
mvn -B archetype:generate -DgroupId=%1 -DartifactId=%2 -DarchetypeArtifactId=maven-archetype-quickstart -DarchetypeVersion=1.4

exit /B 0

:error
exit /B 1