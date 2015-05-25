@echo off & setlocal

set ODB_EXE=..\..\ThirdParty\odb\bin\odb.exe

%ODB_EXE% --database sqlite --generate-query --generate-schema person.hxx

endlocal & @echo on